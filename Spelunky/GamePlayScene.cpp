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

	return S_OK;
}

HRESULT GamePlayScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

	GameObject *player = OBJECTMANAGER->CreateObject(this, ArcheType::Player);

	RoomType roomTypes[16]{};
	//_stageRandomizer.Randomize(roomTypes);

	_camera.Init();

	_stage.InitFromRoomTypes(roomTypes);
	_stage.CalculateMask(0, 0, STAGE_TILE_COUNTX, STAGE_TILE_COUNTY);

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

	float camSpeed = 200.0f;

	ControlCommand playerCommand = _inputMapper.InterpretRawInput(&rawInput);
	if (KEYMANAGER->IsStayKeyDown(VK_LEFT))
	{
		_camera.Move(Vector2(-camSpeed, 0) * deltaTime);
	}
	else if (KEYMANAGER->IsStayKeyDown(VK_RIGHT))
	{
		_camera.Move(Vector2(camSpeed, 0) * deltaTime);
	}
	if (KEYMANAGER->IsStayKeyDown(VK_UP))
	{
		_camera.Move(Vector2(0, -camSpeed) * deltaTime);
	}
	else if (KEYMANAGER->IsStayKeyDown(VK_DOWN))
	{
		_camera.Move(Vector2(0, camSpeed) * deltaTime);
	}
	_camera.Update();

}

void GamePlayScene::Render(void)
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));

	_stage.RenderTileLayer(_camera.GetPosition());

	_stage.RenderMaskLayer(_camera.GetPosition());
	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}