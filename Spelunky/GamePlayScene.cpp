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
	//IMAGEMANAGER->LoadImageFromFile(L"resources/gfx/background.png", L"background");

	return S_OK;
}

HRESULT GamePlayScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

	GameObject *player = OBJECTMANAGER->CreateObject(this, ArcheType::Player);

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

	ControlCommand playerCommand = _inputMapper.InterpretRawInput(&rawInput);

}

void GamePlayScene::Render(ID2D1HwndRenderTarget * renderTarget)
{
	//그리기 전에는 항상 BeginDraw()
	renderTarget->BeginDraw();
	renderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));

	//그린 후에는 항상 EndDraw()
	renderTarget->EndDraw();
}