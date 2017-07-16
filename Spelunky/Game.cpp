#include "stdafx.h"
#include "Game.h"

#include "LoadingScene.h"
#include "MapToolScene.h"
#include "GamePlayScene.h"


Game::Game()
{
}

Game::~Game()
{
}

//�ʱ�ȭ
HRESULT Game::Init(void)
{
	gameNode::Init();
	IMAGEMANAGER->Init();
	FILEMANAGER->Init();
	KEYMANAGER->Init();
	TIMEMANAGER->Init();
	OBJECTMANAGER->Init();

	IScene *gamePlayScene = new GamePlayScene;
	IScene *mapToolScene = new MapToolScene;
	IScene *loadingScene = new LoadingScene;

	//���߰�
	SCENEMANAGER->AddScene(_T("GamePlayScene"), gamePlayScene);
	SCENEMANAGER->AddScene(_T("MapToolScene"), mapToolScene);
	SCENEMANAGER->AddScene(_T("LoadingScene"), loadingScene);

	//���� �� ����
	SCENEMANAGER->ChangeScene(_T("GamePlayScene"));


	return S_OK;
}

//����..
void Game::Release(void)
{
	gameNode::Release();
	IMAGEMANAGER->Release();
	IMAGEMANAGER->releaseSingleton();
	KEYMANAGER->Release();
	KEYMANAGER->releaseSingleton();
	TIMEMANAGER->Release();
	TIMEMANAGER->releaseSingleton();
	SCENEMANAGER->Release();
	SCENEMANAGER->releaseSingleton();
}

//������Ʈ...
void Game::Update(void)
{
	gameNode::Update();
	SCENEMANAGER->Update();
}

//����...
void Game::Render()
{
	SCENEMANAGER->Render();

	//GAMESTATE->Render(renderTarget);
	//_applicationTimer.Update();
}


