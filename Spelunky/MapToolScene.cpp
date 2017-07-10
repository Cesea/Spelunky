#include "stdafx.h"
#include "MapToolScene.h"

MapToolScene::MapToolScene()
{
}

MapToolScene::~MapToolScene()
{
}

HRESULT MapToolScene::LoadContent()
{
	return S_OK;
}

HRESULT MapToolScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);
	return S_OK;
}

void MapToolScene::Release(void)
{
}

void MapToolScene::Update(void)
{
	float deltaTime = TIMEMANAGER->GetElapsedTime();

	Win32RawInputState rawInput = {};
	PullRawInput(&rawInput);

	ControlCommand playerCommand = _inputMapper.InterpretRawInput(&rawInput);

}

void MapToolScene::Render(ID2D1HwndRenderTarget * renderTarget)
{
	//그리기 전에는 항상 BeginDraw()
	renderTarget->BeginDraw();
	renderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));


	//그린 후에는 항상 EndDraw()
	renderTarget->EndDraw();
}

