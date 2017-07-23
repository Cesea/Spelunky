#include "stdafx.h"
#include "TestScene.h"

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}

HRESULT TestScene::LoadContent()
{
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\dustring.png", L"dustring");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\smokering.png", L"smokering");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\explosion.png", L"explosion");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\bomb.png", L"bomb");

	KEYANIMANAGER->AddDefPlayFrameAnimation(L"explosion", L"explosion", 128, 128, 20, false, false);
	int normalBombArray[] = { 0, 1, 2 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"bomb", L"bomb", 80, 80, normalBombArray, 3, 12, true);

	int stickyBombArray[] = {3, 4, 5};
	KEYANIMANAGER->AddArrayFrameAnimation(L"bomb", L"bomb", 80, 80, stickyBombArray, 3, 12, true);

	return S_OK;
}

Vector2 TestScene::GetRandomVector2(float xMax, float yMax)
{
	return Vector2(RND->GetFloat(-xMax, xMax), RND->GetFloat(-yMax, yMax));
}

Vector2 TestScene::GetRandomVector2(float xMin, float xMax, float yMin, float yMax)
{
	return Vector2(RND->GetFloat(xMin, xMax), RND->GetFloat(yMin, yMax));
}

HRESULT TestScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

	OBJECTMANAGER->SetCurrentScene(this);

	EFFECTMANAGER->Init();
	EFFECTMANAGER->SetCameraLink(&cameraTemp);

	//BaseProperty prop;
	//_bomb.Init(&prop);
	//_bomb.position = TilePosition(5, 5);

	BombProperty property;
	property.position = IntVector2(5, 5);
	//property.position
	_bomb = (Bomb *)OBJECTMANAGER->CreateObject(L"bomb", &property);

	return S_OK;
}

void TestScene::Release(void)
{
}

void TestScene::Update(void)
{
	float deltaTime = TIMEMANAGER->GetElapsedTime();

	Win32RawInputState rawInput = {};
	PullRawInput(&rawInput);

	ControlCommand playerCommand = _inputMapper.InterpretRawInput(&rawInput);

	_bomb->Update(deltaTime);
	EFFECTMANAGER->Update(deltaTime);
}

void TestScene::Render()
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(D2D1::ColorF(0.1f, 0.4f, 0.7f, 1.0f));

	Vector2 camPos{};


	_bomb->Render(gRenderTarget, camPos);
	EFFECTMANAGER->Render();

	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}


