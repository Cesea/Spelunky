#include "stdafx.h"
#include "LoadingScene.h"

LoadingScene::LoadingScene()
{
}

LoadingScene::~LoadingScene()
{
}

HRESULT LoadingScene::LoadContent()
{
	return S_OK;
}

HRESULT LoadingScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);
	return S_OK;
}

void LoadingScene::Release(void)
{
}

void LoadingScene::Update(void)
{
	float deltaTime = TIMEMANAGER->GetElapsedTime();

	Win32RawInputState rawInput = {};
	PullRawInput(&rawInput);

	ControlCommand playerCommand = _inputMapper.InterpretRawInput(&rawInput);

}

void LoadingScene::Render()
{
	//�׸��� ������ �׻� BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));




	//�׸� �Ŀ��� �׻� EndDraw()
	gRenderTarget->EndDraw();
}

