#include "stdafx.h"
#include "GamePlayScene.h"

GamePlayScene::GamePlayScene()
{
}

GamePlayScene::~GamePlayScene()
{
}

HRESULT GamePlayScene::LoadContent()
{
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\minetile.png", L"minetile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\woodtile.png", L"woodtile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\jungletile.png", L"jungletile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\templetile.png", L"templetile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\char_orange.png", L"char_orange");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\bordertile.png", L"bordertile");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\gem.png", L"gem");

	int idleArray[1] = {0};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_idle", L"char_orange", 80, 80, idleArray, 1, 10, false);

	int walkArray[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_walk", L"char_orange", 80, 80, walkArray, 8, 15, true);

	int crawlArray[] = {12, 13};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_crawl", L"char_orange", 80, 80, crawlArray, 2, 12, false);

	int crawlIdleArray[] = {14};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_crawlIdle", L"char_orange", 80, 80, crawlIdleArray, 1, 12, false);

	int standUpArray[] = {15, 16};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_standUp", L"char_orange", 80, 80, standUpArray, 2, 12, false);

	int crawlMoveArray[] = {17, 18, 19, 20, 21, 22, 23};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_crawlMove", L"char_orange", 80, 80, crawlMoveArray, 7, 14, true);

	int lookUpArray[] = {96, 97, 98, 99};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_lookUp", L"char_orange", 80, 80, lookUpArray, 4, 14, false);

	int lookRevertArray[] = {100, 101, 102};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_lookRevert", L"char_orange", 80, 80, lookRevertArray, 3, 14, false);

	int jumpArray[] = {108, 109, 110, 111, 112};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_jump", L"char_orange", 80, 80, jumpArray, 5, 14, false);

	int fallingArray[] = {113, 114, 115};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_falling", L"char_orange", 80, 80, fallingArray, 3, 14, false);

	int ladderIdleArray[] = {72};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_ladderIdle", L"char_orange", 80, 80, ladderIdleArray, 1, 1, true);

	int ladderClimbArray[] = {72, 73, 74, 75, 76, 77};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_ladderClimb", L"char_orange", 80, 80, ladderClimbArray, 6, 10, true);

	int onLedgeArray[] = {36, 37, 38, 39, 40, 41, 42, 43};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_onLedge", L"char_orange", 80, 80, onLedgeArray, 8, 15, true);

	int grabLedgeArray[] = {29, 30, 31, 32, 33, 34};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_ledgeGrab", L"char_orange", 80, 80, grabLedgeArray, 6, 10, false);

	int grabArray[] = {44, 45, 46, 47};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_grab", L"char_orange", 80, 80, grabArray, 4, 10, false);


	return S_OK;
}

void GamePlayScene::CreateAndPlaceObject(ArcheType type, const TilePosition & position)
{
	GameObject *object = OBJECTMANAGER->CreateObject(this, type);
	object->position = position;
	object->desiredPosition = position;
	if (type == ArcheType::Player)
	{
		_playerId = object->GetId();
	}
}

HRESULT GamePlayScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

	CreateAndPlaceObject(ArcheType::Player, TilePosition(5, 5));
	CreateAndPlaceObject(ArcheType::Gem, TilePosition(6, 9));

	_camera.Init();
	_camera.SetTarget(OBJECTMANAGER->FindObjectId(_playerId));
	STAGEMANAGER->Init();

	STAGEMANAGER->SetCameraLink(&_camera);

	return S_OK;
}

void GamePlayScene::Release(void)
{
	STAGEMANAGER->Release();
	//SAFE_RELEASE_AND_DELETE(_obstacle);
}

void GamePlayScene::Update(void)
{
	float deltaTime = TIMEMANAGER->GetElapsedTime();

	Win32RawInputState rawInput = {};
	PullRawInput(&rawInput);
	ControlCommand playerCommand =  _inputMapper.InterpretRawInput(&rawInput);
	if (playerCommand.fire)
	{
		EVENTMANAGER->QueueEvent(new PlayerInputEvent(_playerId, playerCommand));
	}

	float camSpeed = 200.0f;

	for (auto &object : OBJECTMANAGER->GetObjectMapRef())
	{
		object.second->Update(deltaTime);
	}

	//STAGEMANAGER->

	_camera.Update();

}

void GamePlayScene::Render(void)
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));

	STAGEMANAGER->RenderTileLayer();
	
	Vector2 unTiledCamPos = _camera.GetPosition().UnTilelize();
	for (auto &object : OBJECTMANAGER->GetObjectMapRef())
	{
		object.second->Render(gRenderTarget, unTiledCamPos);
	}

	STAGEMANAGER->RenderMaskLayer();
	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}