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
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\titlebatalpha.png", L"titlebatalpha");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\titlelayers.png", L"titlelayers");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\titlealpha.png", L"titlealpha");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\titlealpha.png", L"titlealpha");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_doorback.png", L"mainmenu_doorback");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_arch.png", L"mainmenu_arch");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_statues.png", L"mainmenu_statues");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_sanddirt.png", L"mainmenu_sanddirt");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_cavesides.png", L"mainmenu_cavesides");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_doorandrocks.png", L"mainmenu_doorandrocks");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_chainsign.png", L"mainmenu_chainsign");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_doorback.png", L"mainmenu_doorback");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_menuspear.png", L"mainmenu_menuspear");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_alpha.png", L"mainmenu_alpha");

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

	_currentState = MenuSceneState::Menu;

#pragma region TitleInit
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
		_titleObjects.bat[i].AlphaInitAnimation(L"titlebatalpha", batAnimation, IntVector2(0, 0));
		_titleObjects.bat[i].SetScale(RND->GetFloat());
		_titleObjects.bat[i].position = Vector2(RND->GetFloat(1500, 2300), RND->GetFloat(50, 440));
		//_titleObjects.bat[i].MoveTo(Vector2(_titleObjects.bat[i].position.x - 3000, _titleObjects.bat[i].position.y), 4.5f);
	}

	_titleObjects.player.position = Vector2(0, 240);
	_titleObjects.ground.position = Vector2(0, 280);
#pragma endregion

#pragma region MenuInit

	_menuObjects.doorBack.Init(L"mainmenu_doorback", 0, 0, 512, 512, IntVector2(-256, -256));
	_menuObjects.doorBack.position = Vector2(WINSIZEX / 2, WINSIZEY / 2 );

	_menuObjects.arch.Init(L"mainmenu_arch", 0, 0, 1024, 512, IntVector2(-512, 0));
	_menuObjects.arch.position = Vector2(WINSIZEX / 2, 0);

	_menuObjects.leftStatues.Init(L"mainmenu_statues", 0, 0, 400, 600, IntVector2(-200, 0));
	_menuObjects.leftStatues.position.x = WINSIZEX / 2 - 400;
	_menuObjects.leftStatues.position.y = 70;
	_menuObjects.rightStatues.Init(L"mainmenu_statues", 400, 0, 400, 600, IntVector2(-200, 0));
	_menuObjects.rightStatues.position.x = WINSIZEX / 2 + 400;
	_menuObjects.rightStatues.position.y = 70;

	_menuObjects.leftCaveSide.Init(L"mainmenu_cavesides", 0, 0, 240, 720, IntVector2(-120, 0));
	_menuObjects.leftCaveSide.position.x = WINSIZEX / 2 - 460;
	_menuObjects.rightCaveSide.Init(L"mainmenu_cavesides", 240, 0, 240, 720, IntVector2(-120, 0));
	_menuObjects.rightCaveSide.position.x = WINSIZEX / 2 + 460;

	_menuObjects.sandDirt.Init(L"mainmenu_sanddirt", 0, 0, 1024, 192, IntVector2(-512, 0));
	_menuObjects.sandDirt.position.x = WINSIZEX / 2;
	_menuObjects.sandDirt.position.y = 550;

	_menuObjects.alpha.Init(L"mainmenu_alpha", 0, 0, WINSIZEX, WINSIZEY, IntVector2(0, 0));
	
#pragma endregion
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

	Vector2 camPos{};


	switch (_currentState)
	{
	case  CutScene :
	{

	}break;
	case Title :
	{
		_titleObjects.title.Render(gRenderTarget, camPos);
		_titleObjects.player.Render(gRenderTarget, camPos);
		_titleObjects.ground.Render(gRenderTarget, camPos);
		for (int i = 0; i < 8; ++i)
		{
			_titleObjects.bat[i].RenderScale(gRenderTarget, camPos);
		}
		_titleObjects.alpha.Render(gRenderTarget, camPos, 0.3f);

	}break;

	case Menu:
	{
		_menuObjects.doorBack.Render(gRenderTarget, camPos);
		_menuObjects.arch.Render(gRenderTarget, camPos);

		_menuObjects.sandDirt.Render(gRenderTarget, camPos);

		_menuObjects.leftStatues.Render(gRenderTarget, camPos);
		_menuObjects.rightStatues.Render(gRenderTarget, camPos);

		_menuObjects.leftCaveSide.Render(gRenderTarget, camPos);
		_menuObjects.rightCaveSide.Render(gRenderTarget, camPos);


		_dWrite.PrintText(gRenderTarget, 100, 100, 500, 500, L"Hello!!!", D2D1::ColorF(1, 1, 1, 1));

		_menuObjects.alpha.Render(gRenderTarget, camPos);
	}break;
	case CharSelect :
	{

	}break;
	}

	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}

