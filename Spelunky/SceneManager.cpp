#include "stdafx.h"
#include "SceneManager.h"

#include "IScene.h"


DWORD CALLBACK LoadingThread(LPVOID prc)
{
	//교체될 init 함수를 실행한다
	SceneManager::_readyScene->Init();

	//여기로 오면 교체 될 씬의 초기화 끝난 것임.

	//현재 씬을 교체될 씬으로 바꾼다
	SceneManager::_currentScene = SceneManager::_readyScene;

	SceneManager::_loadingScene->Release();
	SceneManager::_loadingScene = NULL;
	SceneManager::_readyScene = NULL;

	return NULL;
}

IScene* SceneManager::_currentScene = NULL;
IScene* SceneManager::_loadingScene = NULL;
IScene* SceneManager::_readyScene = NULL;


HRESULT SceneManager::Init(void)
{
	_currentScene = NULL;
	_loadingScene = NULL;
	_readyScene = NULL;

	return S_OK;
}

void SceneManager::Release(void)
{
	if (_currentScene) _currentScene->Release();
}

void SceneManager::Update(void)
{
	if (_currentScene) _currentScene->Update();
}

void SceneManager::Render(ID2D1HwndRenderTarget *renderTarget)
{
	if (_currentScene) _currentScene->Render(renderTarget);
}

IScene * SceneManager::AddScene(std::wstring sceneName, IScene * scene)
{
	if (!scene) return NULL;
	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

IScene * SceneManager::AddLoadingScene(std::wstring loadingSceneName, IScene * scene)
{
	if (!scene) return NULL;
	_mSceneList.insert(make_pair(loadingSceneName, scene));

	return scene;
}

HRESULT SceneManager::ChangeScene(std::wstring sceneName)
{
	//교체할 씬을 찾는다
	mapSceneIter find = _mSceneList.find(sceneName);

	//교체 불가능하면 뻘스 리떤
	if (find == _mSceneList.end()) return E_FAIL;

	//새롭게 교체되는 씬을 초기화 한다
	if (SUCCEEDED(find->second->Init()))
	{
		//기존에 씬이 있으면 릴리즈...
		if (_currentScene) _currentScene->Release();
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}

HRESULT SceneManager::ChangeScene(std::wstring sceneName, std::wstring loadingSceneName)
{
	//교체할 씬을 찾는다
	mapSceneIter find = _mSceneList.find(sceneName);

	//교체 불가능하면 뻘스 리떤
	if (find == _mSceneList.end()) return E_FAIL;

	//교체할 로딩 씬 찾는다
	mapSceneIter findLoading = _mLoadingSceneList.find(loadingSceneName);

	//찾지 못하면 일반 로딩...
	if (findLoading == _mLoadingSceneList.end())
	{
		return ChangeScene(loadingSceneName);
	}

	//새롭게 교체되는 씬을 초기화 한다
	if (SUCCEEDED(findLoading->second->Init()))
	{
		//기존에 씬이 있으면 릴리즈...
		if (_currentScene) _currentScene->Release();

		//로딩씬!!
		_loadingScene = findLoading->second;

		//로딩 끝난 후 변경될 씬
		_readyScene = find->second;

		//로딩씬 호출한다
		CloseHandle(CreateThread(NULL, 0, LoadingThread, NULL, 0, &_loadThreadID));

		return S_OK;
	}

	return E_FAIL;
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}


