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
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\minebg.png", L"minebg");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\bordertile.png", L"bordertile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\exitdoors.png", L"exitdoors");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\gem.png", L"gem");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\playerhud.png", L"playerhud");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\moneyhud.png", L"moneyhud");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\rock.png", L"rock");

	int idleArray[1] = {0};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_idle", L"char_orange", 80, 80, idleArray, 1, 10, false);

	int walkArray[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_walk", L"char_orange", 80, 80, walkArray, 8, 15, true);

	int crawlArray[] = {12, 13};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_crawl", L"char_orange", 80, 80, crawlArray, 2, 18, false);

	int crawlIdleArray[] = {14};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_crawlIdle", L"char_orange", 80, 80, crawlIdleArray, 1, 1, false);

	int standUpArray[] = {15, 16};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_standUp", L"char_orange", 80, 80, standUpArray, 2, 18, false);

	int crawlMoveArray[] = {17, 18, 19, 20, 21, 22, 23};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_crawlMove", L"char_orange", 80, 80, crawlMoveArray, 7, 14, true);

	int lookUpArray[] = {96, 97, 98, 99};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_lookUp", L"char_orange", 80, 80, lookUpArray, 4, 16, false);

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

	int upperDeathArray[] = {26, 27};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_upperDeath", L"char_orange", 80, 80, upperDeathArray, 2, 2, true);

	//Faint해야함
	int faintArray[] = { 24, 25, 26, 27 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_faint", L"char_orange", 80, 80, faintArray, 4, 10, false);

	int attackArray[] = {48, 49, 50, 51, 52, 53};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_attack", L"char_orange", 80, 80, attackArray, 6, 12, false);

	int throwArray[] = {54, 55, 56, 57, 58};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_throw", L"char_orange", 80, 80, throwArray, 5, 10, false);


	return S_OK;
}

//void GamePlayScene::CreateAndPlaceObject(ArcheType type, const TilePosition & position)
//{
//	GameObject *object = OBJECTMANAGER->CreateObject(type);
//	object->position = position;
//	object->desiredPosition = position;
//	if (type == ArcheType::Player)
//	{
//		_playerId = object->GetId();
//		_pPlayer = (Player *)object;
//	}
//}

HRESULT GamePlayScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

	OBJECTMANAGER->SetCurrentScene(this);

	//CreateAndPlaceObject(ArcheType::Player, TilePosition(7, 5));
	////CreateAndPlaceObject(ArcheType::Gem, TilePosition(6, 9));
	//CreateAndPlaceObject(ArcheType::Rock, TilePosition(6, 9));

	_camera.Init();
	//_camera.SetTarget(OBJECTMANAGER->FindObjectId(_playerId));
	STAGEMANAGER->Init();

	STAGEMANAGER->SetCameraLink(&_camera);

	_playerHudSprite = new D2DFrameSprite();
	_playerHudSprite->Init(IMAGEMANAGER->GetImage(L"playerhud"), 360.0f, 90.0f, IntVector2(0, 0));

	_moneyHudSprite = new D2DSprite();
	_moneyHudSprite->Init(IMAGEMANAGER->GetImage(L"moneyhud"), 0, 0, 320.0f, 80.0f, IntVector2(0, 0));


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

	_camera.Update();

}

void GamePlayScene::Render(void)
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));


	Vector2 unTiledCamPos = _camera.GetPosition().UnTilelize();

	STAGEMANAGER->Render();

	//_playerHudSprite->FrameRender(gRenderTarget, 70, 40, 0, 0);
	//_moneyHudSprite->Render(gRenderTarget, 0, 140);

	//_dWrite.PrintText(gRenderTarget, 105, 155, 110, 30, std::to_wstring(_pPlayer->GetMoney()).c_str(), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));

	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}