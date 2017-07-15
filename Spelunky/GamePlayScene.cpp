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

	int idleArray[1] = {0};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_idle", L"char_orange", 80, 80, idleArray, 1, 10, false);

	int walkArray[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_walk", L"char_orange", 80, 80, walkArray, 8, 15, true);

	int crawlArray[] = {12, 13, 14};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_crawl", L"char_orange", 80, 80, crawlArray, 3, 10, false);

	int standUpArray[] = {15, 16};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_standUp", L"char_orange", 80, 80, standUpArray, 2, 10, false);

	return S_OK;
}

void GamePlayScene::CreateAndPlaceObject(ArcheType type, const TilePosition & position)
{
	GameObject *object = OBJECTMANAGER->CreateObject(this, type);
	object->position = position;
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

	CreateAndPlaceObject(ArcheType::Player, TilePosition(2, 2));

	RoomType roomTypes[16]{};
	//_stageRandomizer.Randomize(roomTypes);

	_camera.Init();

	_stage.InitFromRoomTypes(roomTypes);
	//_stage.CalculateMask(0, 0, STAGE_TILE_COUNTX, STAGE_TILE_COUNTY);

	return S_OK;
}

void GamePlayScene::Release(void)
{
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

	//ControlCommand playerCommand = _inputMapper.InterpretRawInput(&rawInput);
	//if (KEYMANAGER->IsStayKeyDown(VK_LEFT))
	//{
	//	_camera.Move(Vector2(-camSpeed, 0) * deltaTime);
	//}
	//else if (KEYMANAGER->IsStayKeyDown(VK_RIGHT))
	//{
	//	_camera.Move(Vector2(camSpeed, 0) * deltaTime);
	//}
	//if (KEYMANAGER->IsStayKeyDown(VK_UP))
	//{
	//	_camera.Move(Vector2(0, -camSpeed) * deltaTime);
	//}
	//else if (KEYMANAGER->IsStayKeyDown(VK_DOWN))
	//{
	//	_camera.Move(Vector2(0, camSpeed) * deltaTime);
	//}
	_camera.Update();

}

void GamePlayScene::Render(void)
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));

	_stage.RenderTileLayer(_camera.GetPosition());
	
	Vector2 unTiledCamPos = _camera.GetPosition().UnTilelize();
	for (auto &object : OBJECTMANAGER->GetObjectMapRef())
	{
		object.second->Render(gRenderTarget, unTiledCamPos);
	}

	_stage.RenderMaskLayer(_camera.GetPosition());
	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}