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
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\common\\minetile.png", L"minetile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\common\\woodtile.png", L"woodtile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\common\\jungletile.png", L"jungletile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\common\\templetile.png", L"templetile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\common\\bordertile.png", L"bordertile");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\char_orange.png", L"char_orange");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\minebg.png", L"minebg");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\exitdoors.png", L"exitdoors");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\gems.png", L"gems");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\throws.png", L"throws");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\playerhud.png", L"playerhud");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\moneyhud.png", L"moneyhud");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\exitText.png", L"exitSprite");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\weaponanimation.png", L"weaponanimation");

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

	int exitArray[] = {60, 61, 62, 63, 64, 65};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_exit", L"char_orange", 80, 80, exitArray, 6, 10, true);


#pragma region Weapons
	int whipArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"weapon_whip", L"weaponanimation", 80, 80, whipArray, 11, 20, false);

	int mattockArray[] = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"weapon_mattock", L"weaponanimation", 80, 80, mattockArray, 11, 20, false);

	int macheteArray[] = {22, 23, 24, 25, 26 , 27, 28, 29, 30, 31, 32};
	KEYANIMANAGER->AddArrayFrameAnimation(L"weapon_machete", L"weaponanimation", 80, 80, macheteArray, 11, 20, false);
#pragma endregion

#pragma region Bomb
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\dustring.png", L"dustring");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\smokering.png", L"smokering");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\explosion.png", L"explosion");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\bomb.png", L"bomb");

	KEYANIMANAGER->AddDefPlayFrameAnimation(L"explosion", L"explosion", 128, 128, 26, false, false);
	int normalBombArray[] = { 0, 1, 2 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"normal_bomb", L"bomb", 80, 80, normalBombArray, 3, 12, true);

	int stickyBombArray[] = {3, 4, 5};
	KEYANIMANAGER->AddArrayFrameAnimation(L"sticky_bomb", L"bomb", 80, 80, stickyBombArray, 3, 12, true);
#pragma endregion

	return S_OK;
}

HRESULT GamePlayScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

	OBJECTMANAGER->SetCurrentScene(this);

	PlayerProperty playerProperty{};
	_pPlayer = (Player *)OBJECTMANAGER->CreateObject(L"player", &playerProperty);

	_camera.Init();
	_camera.SetTarget(_pPlayer);
	STAGEMANAGER->Init();

	STAGEMANAGER->SetCameraLink(&_camera);
	STAGEMANAGER->SetPlayerLink(_pPlayer);

	EFFECTMANAGER->Init();
	EFFECTMANAGER->SetCameraLink(&_camera);

	UIMANAGER->Init();
	UIMANAGER->SetPlayerLink(_pPlayer);

	EVENTMANAGER->QueueEvent(new StageTransitionEvent());

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


	_pPlayer->Update(deltaTime);

	float camSpeed = 200.0f;

	_camera.Update(deltaTime);
	STAGEMANAGER->Update(deltaTime);

	Vector2 absMouseVector = _camera.GetPosition().UnTilelize() + currentMouse;

	if (KEYMANAGER->IsOnceKeyDown('Q'))
	{
		TilePosition mouseTilePos(absMouseVector);
		STAGEMANAGER->DestroyTile(IntVector2(mouseTilePos.tileX, mouseTilePos.tileY));
	}

	EFFECTMANAGER->Update(deltaTime);

	UIMANAGER->Update(deltaTime);


	if (KEYMANAGER->IsOnceKeyDown(VK_ESCAPE))
	{
		EVENTMANAGER->DiscardAllEvents();
		SCENEMANAGER->ChangeScene(L"MenuScene");
	}
}

void GamePlayScene::Render(void)
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));


	Vector2 unTiledCamPos = _camera.GetPosition().UnTilelize();

	STAGEMANAGER->Render();

	_pPlayer->Render(gRenderTarget, unTiledCamPos);

	EFFECTMANAGER->Render();

	UIMANAGER->Render(unTiledCamPos);

	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}