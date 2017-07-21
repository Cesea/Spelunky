#include "stdafx.h"
#include "MenuScene.h"

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

HRESULT MenuScene::LoadContent()
{
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\title.png", L"title");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\titlebat.png", L"titlebat");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\titlelayers.png", L"titlelayers");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\titlealpha.png", L"titlealpha");

	int batArray[] = {0, 1, 2, 3, 4, 5};
	KEYANIMANAGER->AddArrayFrameAnimation(L"titlebat", L"titlebat", 256, 256, batArray, 6, 15, true);
	return S_OK;
}

HRESULT MenuScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

	_titleObjects.title.Init(L"title", 0, 0, WINSIZEX, WINSIZEY, IntVector2());
	_titleObjects.player.Init(L"titlelayers", 0, 0, 1024, 512, IntVector2());
	_titleObjects.ground.Init(L"titlelayers", 0, 512, 1024, 512, IntVector2());
	_titleObjects.alpha.Init(L"titlealpha", 0, 0, 1280, 720, IntVector2());

	_batTimer.Init(RND->GetFloat(0.5, 1.2));

	for (int i = 0; i < 8; ++i)
	{
		Animation  *batAnimation = new Animation;
		batAnimation->InitCopy(KEYANIMANAGER->FindAnimation(L"titlebat"));
		_titleObjects.bat[i].InitAnimation(L"titlebat", batAnimation, IntVector2(0, 0));
		_titleObjects.bat[i].position = Vector2(RND->GetFloat(1500, 2300), RND->GetFloat(50, 440));
		//_titleObjects.bat[i].MoveTo(Vector2(_titleObjects.bat[i].position.x - 3000, _titleObjects.bat[i].position.y), 4.5f);
	}

	_titleObjects.player.position = Vector2(0, 240);
	_titleObjects.ground.position= Vector2(0, 280);
	
	return S_OK;
}

void MenuScene::Release(void)
{
}

void MenuScene::Update(void)
{
	float deltaTime = TIMEMANAGER->GetElapsedTime();

	Win32RawInputState rawInput = {};
	PullRawInput(&rawInput);

	ControlCommand playerCommand = _inputMapper.InterpretRawInput(&rawInput);

	switch (_currentState)
	{
	case  CutScene :
	{

	}break;
	case Title :
	{
		if (_batTimer.Tick(deltaTime))
		{
			_titleObjects.bat[_currentBatTracker].MoveTo(
				Vector2(_titleObjects.bat[_currentBatTracker].position.x - 3000, _titleObjects.bat[_currentBatTracker].position.y), RND->GetFloat(3.0, 4.0));
			_currentBatTracker++;
			if (_currentBatTracker >= 8)
			{
				_currentBatTracker = 0;
			}
		}
		for (int i = 0; i < 8; ++i)
		{
			_titleObjects.bat[i].Update(deltaTime);
		}
	}break;
	case Menu:
	{

	}break;
	case CharSelect :
	{

	}break;
	}

}

void MenuScene::Render()
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));


	switch (_currentState)
	{
	case  CutScene :
	{

	}break;
	case Title :
	{
		_titleObjects.title.Render(gRenderTarget, Vector2(0, 0));
		_titleObjects.player.Render(gRenderTarget, Vector2(0, 0));
		_titleObjects.ground.Render(gRenderTarget, Vector2(0, 0));
		for (int i = 0; i < 8; ++i)
		{
			_titleObjects.bat[i].Render(gRenderTarget, Vector2(0, 0));
		}
		_titleObjects.alpha.Render(gRenderTarget, Vector2(0, 0), 0.3f);
	}break;
	case Menu:
	{

	}break;
	case CharSelect :
	{

	}break;
	}

	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}

