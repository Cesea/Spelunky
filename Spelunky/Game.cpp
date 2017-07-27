#include "stdafx.h"
#include "Game.h"

#include "LoadingScene.h"
#include "MapToolScene.h"
#include "GamePlayScene.h"
#include "MenuScene.h"

#include "TestScene.h"


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
	SOUNDMANAGER->Init();
	FILEMANAGER->Init();
	KEYMANAGER->Init();
	TIMEMANAGER->Init();
	OBJECTMANAGER->Init();
	PROPERTYFACTORY->Init();
	EVENTCOLLECTOR->Init();
	RANKINGMANAGER->Init();

	IScene *gamePlayScene = new GamePlayScene;
	IScene *mapToolScene = new MapToolScene;
	IScene *loadingScene = new LoadingScene;
	IScene *menuScene = new MenuScene;

	//���߰�
	SCENEMANAGER->AddScene(_T("GamePlayScene"), gamePlayScene);
	SCENEMANAGER->AddScene(_T("MapToolScene"), mapToolScene);
	SCENEMANAGER->AddScene(_T("LoadingScene"), loadingScene);
	SCENEMANAGER->AddScene(_T("MenuScene"), menuScene);

	//���� �� ����
	SCENEMANAGER->ChangeScene(_T("MapToolScene"));

	return S_OK;
}

//����..
void Game::Release(void)
{
	gameNode::Release();
	IMAGEMANAGER->Release();
	IMAGEMANAGER->releaseSingleton();
	SOUNDMANAGER->Release();
	SOUNDMANAGER->releaseSingleton();
	KEYMANAGER->Release();
	KEYMANAGER->releaseSingleton();
	TIMEMANAGER->Release();
	TIMEMANAGER->releaseSingleton();
	SCENEMANAGER->Release();
	SCENEMANAGER->releaseSingleton();
	EVENTCOLLECTOR->Release();
	EVENTCOLLECTOR->releaseSingleton();

	PROPERTYFACTORY->Release();
	PROPERTYFACTORY->releaseSingleton();

	RANKINGMANAGER->Release();
	RANKINGMANAGER->releaseSingleton();
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


