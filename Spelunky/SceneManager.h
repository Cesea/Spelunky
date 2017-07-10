#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "singletonBase.h"
#include <map>
#include <string>
#include "stdafx.h"

//���漱��
class IScene;

//=================================================
//================ 20170426 #Ű�Ŵ���# =============
//=================================================
class SceneManager : public singletonBase <SceneManager>
{
public:
	typedef std::map<std::wstring, IScene*> mapSceneList;
	typedef std::map<std::wstring, IScene*>::iterator mapSceneIter;

private:
	static IScene* _currentScene;	//�����
	static IScene* _loadingScene; //�ε���
	static IScene* _readyScene;	//��ü ������� ��

	mapSceneList _mSceneList; //��
	mapSceneList _mLoadingSceneList; //�ε��� ����Ʈ

	DWORD _loadThreadID;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);
	void Render(ID2D1HwndRenderTarget *renderTarget);

	//���߰�
	IScene* AddScene(std::wstring sceneName, IScene* scene);

	//�ε��� �߰�
	IScene* AddLoadingScene(std::wstring loadingSceneName, IScene* scene);

	//��ü����
	HRESULT ChangeScene(std::wstring sceneName);
	HRESULT ChangeScene(std::wstring sceneName, std::wstring loadingSceneName);

	friend DWORD CALLBACK LoadingThread(LPVOID prc);

	SceneManager();
	~SceneManager();
};




#endif