#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "singletonBase.h"
#include <map>
#include <string>
#include "stdafx.h"

//전방선언
class IScene;

//=================================================
//================ 20170426 #키매니져# =============
//=================================================
class SceneManager : public singletonBase <SceneManager>
{
public:
	typedef std::map<std::wstring, IScene*> mapSceneList;
	typedef std::map<std::wstring, IScene*>::iterator mapSceneIter;

private:
	static IScene* _currentScene;	//현재씬
	static IScene* _loadingScene; //로딩씬
	static IScene* _readyScene;	//교체 대기중인 씬

	mapSceneList _mSceneList; //씬
	mapSceneList _mLoadingSceneList; //로딩씬 리스트

	DWORD _loadThreadID;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);
	void Render(ID2D1HwndRenderTarget *renderTarget);

	//씬추가
	IScene* AddScene(std::wstring sceneName, IScene* scene);

	//로딩씬 추가
	IScene* AddLoadingScene(std::wstring loadingSceneName, IScene* scene);

	//씬체인지
	HRESULT ChangeScene(std::wstring sceneName);
	HRESULT ChangeScene(std::wstring sceneName, std::wstring loadingSceneName);

	friend DWORD CALLBACK LoadingThread(LPVOID prc);

	SceneManager();
	~SceneManager();
};




#endif