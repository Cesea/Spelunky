#include "stdafx.h"
#include "SceneManager.h"

#include "IScene.h"


DWORD CALLBACK LoadingThread(LPVOID prc)
{
	//��ü�� init �Լ��� �����Ѵ�
	SceneManager::_readyScene->Init();

	//����� ���� ��ü �� ���� �ʱ�ȭ ���� ����.

	//���� ���� ��ü�� ������ �ٲ۴�
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
	//��ü�� ���� ã�´�
	mapSceneIter find = _mSceneList.find(sceneName);

	//��ü �Ұ����ϸ� ���� ����
	if (find == _mSceneList.end()) return E_FAIL;

	//���Ӱ� ��ü�Ǵ� ���� �ʱ�ȭ �Ѵ�
	if (SUCCEEDED(find->second->Init()))
	{
		//������ ���� ������ ������...
		if (_currentScene) _currentScene->Release();
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}

HRESULT SceneManager::ChangeScene(std::wstring sceneName, std::wstring loadingSceneName)
{
	//��ü�� ���� ã�´�
	mapSceneIter find = _mSceneList.find(sceneName);

	//��ü �Ұ����ϸ� ���� ����
	if (find == _mSceneList.end()) return E_FAIL;

	//��ü�� �ε� �� ã�´�
	mapSceneIter findLoading = _mLoadingSceneList.find(loadingSceneName);

	//ã�� ���ϸ� �Ϲ� �ε�...
	if (findLoading == _mLoadingSceneList.end())
	{
		return ChangeScene(loadingSceneName);
	}

	//���Ӱ� ��ü�Ǵ� ���� �ʱ�ȭ �Ѵ�
	if (SUCCEEDED(findLoading->second->Init()))
	{
		//������ ���� ������ ������...
		if (_currentScene) _currentScene->Release();

		//�ε���!!
		_loadingScene = findLoading->second;

		//�ε� ���� �� ����� ��
		_readyScene = find->second;

		//�ε��� ȣ���Ѵ�
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


