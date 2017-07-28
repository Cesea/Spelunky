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
	_loading = new Loading;

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\title.png", L"title");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\titlebat.png", L"titlebat");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\titlebatalpha.png", L"titlebatalpha");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\titlelayers.png", L"titlelayers");
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
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_doorandrocks.png", L"mainmenu_doorandrocks");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\menu\\mainmenu_alpha.png", L"mainmenu_alpha");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\dustring.png", L"dustring");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\smokering.png", L"smokering");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\explosion.png", L"explosion");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\bomb.png", L"bomb");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\particles.png", L"particles");


	//_loading->LoadImageResources(L"resources\\gfx\\particles.png", L"particles");

	int batArray[] = {0, 1, 2, 3, 4, 5};
	KEYANIMANAGER->AddArrayFrameAnimation(L"titlebat", L"titlebat", 256, 256, batArray, 6, 15, true);

	KEYANIMANAGER->AddDefPlayFrameAnimation(L"explosion", L"explosion", 128, 128, 20, false, false);
	int normalBombArray[] = { 0, 1, 2 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"bomb", L"bomb", 80, 80, normalBombArray, 3, 12, true);

	int stickyBombArray[] = { 3, 4, 5 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"bomb", L"bomb", 80, 80, stickyBombArray, 3, 12, true);


	SOUNDMANAGER->AddSound(L"mm_amb", L"resources\\sfx\\Sound\\mm_amb.wav", true, false);
	SOUNDMANAGER->AddSound(L"mm_door1", L"resources\\sfx\\Sound\\mm_door1.wav", false, false);
	SOUNDMANAGER->AddSound(L"mm_door2", L"resources\\sfx\\Sound\\mm_door2.wav", false, false);
	SOUNDMANAGER->AddSound(L"mm_door3", L"resources\\sfx\\Sound\\mm_door3.wav", false, false);
	SOUNDMANAGER->AddSound(L"tikispike", L"resources\\sfx\\Sound\\tikispike.wav", false, false);
	SOUNDMANAGER->AddSound(L"mm_bgm", L"resources\\sfx\\Music\\Menu.ogg", true, true);



	_loading->LoadImageResources(L"resources\\gfx\\common\\minetile.png", L"minetile");
	_loading->LoadImageResources(L"resources\\gfx\\common\\woodtile.png", L"woodtile");
	_loading->LoadImageResources(L"resources\\gfx\\common\\jungletile.png", L"jungletile");
	_loading->LoadImageResources(L"resources\\gfx\\common\\templetile.png", L"templetile");
	_loading->LoadImageResources(L"resources\\gfx\\common\\bordertile.png", L"bordertile");
	_loading->LoadImageResources(L"resources\\gfx\\maptool\\property.png", L"property");

	_loading->LoadImageResources(L"resources\\gfx\\common\\minetile.png", L"minetile");
	_loading->LoadImageResources(L"resources\\gfx\\common\\woodtile.png", L"woodtile");
	_loading->LoadImageResources(L"resources\\gfx\\common\\jungletile.png", L"jungletile");
	_loading->LoadImageResources(L"resources\\gfx\\common\\templetile.png", L"templetile");
	_loading->LoadImageResources(L"resources\\gfx\\common\\bordertile.png", L"bordertile");

	_loading->LoadImageResources(L"resources\\gfx\\char_orange.png", L"char_orange");
	_loading->LoadImageResources(L"resources\\gfx\\minebg.png", L"minebg");

	_loading->LoadImageResources(L"resources\\gfx\\exitdoors.png", L"exitdoors");
	_loading->LoadImageResources(L"resources\\gfx\\gems.png", L"gems");
	_loading->LoadImageResources(L"resources\\gfx\\throws.png", L"throws");

	_loading->LoadImageResources(L"resources\\gfx\\playerhud.png", L"playerhud");
	_loading->LoadImageResources(L"resources\\gfx\\moneyhud.png", L"moneyhud");

	_loading->LoadImageResources(L"resources\\gfx\\exitText.png", L"exitSprite");

	_loading->LoadImageResources(L"resources\\gfx\\weaponanimation.png", L"weaponanimation");

	_loading->LoadImageResources(L"resources\\gfx\\monsters.png", L"monsters");

	_loading->LoadImageResources(L"resources\\gfx\\particles.png", L"particles");
	_loading->LoadImageResources(L"resources\\gfx\\crates.png", L"crates");

	_loading->LoadImageResources(L"resources\\gfx\\board.png", L"board");
	_loading->LoadImageResources(L"resources\\gfx\\tunneltitle.png", L"tunneltitle");
	_loading->LoadImageResources(L"resources\\gfx\\scrollroll.png", L"scrollroll");
	_loading->LoadImageResources(L"resources\\gfx\\scrollpaper.png", L"scrollpaper");

	_loading->LoadImageResources(L"resources\\gfx\\uiElement.png", L"uiElement");
	_loading->LoadImageResources(L"resources\\gfx\\objectSprite.png", L"objectSprite");

	_loading->LoadImageResources(L"resources\\gfx\\bookbg.png", L"bookbg");
	_loading->LoadImageResources(L"resources\\gfx\\bookgameover.png", L"bookgameover");

	_loading->LoadImageResources(L"resources\\gfx\\buttonSprite.png", L"buttonSprite");
	_loading->LoadImageResources(L"resources\\gfx\\eatables.png", L"eatables");

	_loading->LoadImageResources(L"resources\\gfx\\bookbg.png", L"bookbg");
	_loading->LoadImageResources(L"resources\\gfx\\bookgameover.png", L"bookgameover");

	_loading->LoadImageResources(L"resources\\gfx\\obstacles.png", L"obstacles");



	_loading->LoadSoundResources(L"gp_bgm0", L"resources\\sfx\\Music\\A01_A.ogg", true, true);

	_loading->LoadSoundResources(L"gp_bgm1", L"resources\\sfx\\Music\\A01_B.ogg", true, true);
	_loading->LoadSoundResources(L"gp_bgm2", L"resources\\sfx\\Music\\A01_C.ogg", true, true);
	_loading->LoadSoundResources(L"gp_bgm3", L"resources\\sfx\\Music\\A01_first.ogg", true, true);

	_loading->LoadSoundResources(L"bat_one_flap", L"resources\\sfx\\Sound\\batoneflap.wav", false, false);
	_loading->LoadSoundResources(L"bat_flap", L"resources\\sfx\\Sound\\batflap.wav", false, false);

	_loading->LoadSoundResources(L"bomb_explosion", L"resources\\sfx\\Sound\\kaboom.wav", false, false);
	_loading->LoadSoundResources(L"bomb_timer", L"resources\\sfx\\Sound\\bomb_timer.wav", false, false);

	_loading->LoadSoundResources(L"into_door", L"resources\\sfx\\Sound\\intodoor.wav", false, false);
	_loading->LoadSoundResources(L"fade_in", L"resources\\sfx\\Sound\\fadein.wav", false, false);
	_loading->LoadSoundResources(L"fade_out", L"resources\\sfx\\Sound\\fadeout.wav", false, false);

	_loading->LoadSoundResources(L"gem0", L"resources\\sfx\\Sound\\gem1.wav", false, false);
	_loading->LoadSoundResources(L"gem1", L"resources\\sfx\\Sound\\gem2.wav", false, false);
	_loading->LoadSoundResources(L"gem2", L"resources\\sfx\\Sound\\gem3.wav", false, false);
	_loading->LoadSoundResources(L"gem3", L"resources\\sfx\\Sound\\gem4.wav", false, false);
	_loading->LoadSoundResources(L"gem4", L"resources\\sfx\\Sound\\gem5.wav", false, false);

	_loading->LoadSoundResources(L"jump", L"resources\\sfx\\Sound\\jump.wav", false, false);
	_loading->LoadSoundResources(L"land", L"resources\\sfx\\Sound\\land.wav", false, false);
	_loading->LoadSoundResources(L"grab", L"resources\\sfx\\Sound\\grab.wav", false, false);
	_loading->LoadSoundResources(L"on_ledge", L"resources\\sfx\\Sound\\on_ledge.wav", false, true);

	_loading->LoadSoundResources(L"boulder_hit_0", L"resources\\sfx\\Sound\\boulderhit.wav", false, false);
	_loading->LoadSoundResources(L"boulder_hit_1", L"resources\\sfx\\Sound\\boulderhit2.wav", false, false);
	_loading->LoadSoundResources(L"boulder_hit_2", L"resources\\sfx\\Sound\\boulderhit3.wav", false, false);
	_loading->LoadSoundResources(L"boulder_hit_3", L"resources\\sfx\\Sound\\boulderhit4.wav", false, false);

	_loading->LoadSoundResources(L"shatter", L"resources\\sfx\\Sound\\newshatter.wav", false, false);
	_loading->LoadSoundResources(L"bone_shatter", L"resources\\sfx\\Sound\\bone_shatter.wav", false, false);

	_loading->LoadSoundResources(L"rubble0", L"resources\\sfx\\Sound\\rubble1.wav", false, false);
	_loading->LoadSoundResources(L"rubble1", L"resources\\sfx\\Sound\\rubble2.wav", false, false);
	_loading->LoadSoundResources(L"rubble2", L"resources\\sfx\\Sound\\rubble3.wav", false, false);

	_loading->LoadSoundResources(L"rubble_bone0", L"resources\\sfx\\Sound\\rubble_bone1.wav", false, false);
	_loading->LoadSoundResources(L"rubble_bone1", L"resources\\sfx\\Sound\\rubble_bone2.wav", false, false);
	_loading->LoadSoundResources(L"rubble_bone2", L"resources\\sfx\\Sound\\rubble_bone3.wav", false, false);

	_loading->LoadSoundResources(L"rubble_vase0", L"resources\\sfx\\Sound\\rubble_vase1.wav", false, false);
	_loading->LoadSoundResources(L"rubble_vase1", L"resources\\sfx\\Sound\\rubble_vase2.wav", false, false);
	_loading->LoadSoundResources(L"rubble_vase2", L"resources\\sfx\\Sound\\rubble_vase3.wav", false, false);

	_loading->LoadSoundResources(L"collect", L"resources\\sfx\\Sound\\collect.wav", false, false);
	_loading->LoadSoundResources(L"crate_open", L"resources\\sfx\\Sound\\crateopen.wav", false, false);

	_loading->LoadSoundResources(L"whip", L"resources\\sfx\\Sound\\whip.wav", false, false);
	_loading->LoadSoundResources(L"mattock", L"resources\\sfx\\Sound\\mattock.wav", false, false);

	_loading->LoadSoundResources(L"bounce", L"resources\\sfx\\Sound\\bounce.wav", false, false);

	_loading->LoadSoundResources(L"hit", L"resources\\sfx\\Sound\\hit.wav", false, false);

	_loading->LoadSoundResources(L"item_drop", L"resources\\sfx\\Sound\\item_drop.wav", false, false);
	_loading->LoadSoundResources(L"throw", L"resources\\sfx\\Sound\\throw_item.wav", false, false);
	_loading->LoadSoundResources(L"pick_up", L"resources\\sfx\\Sound\\pickup.wav", false, false);

	_loading->LoadSoundResources(L"spike_hit", L"resources\\sfx\\Sound\\spike_hit.wav", false, false);

	_loading->LoadSoundResources(L"bat_flap", L"resources\\sfx\\Sound\\bat_flap.wav", false, false);
	_loading->LoadSoundResources(L"snake_bite", L"resources\\sfx\\Sound\\snakebite.wav", false, false);

	_loading->LoadSoundResources(L"chime", L"resources\\sfx\\Sound\\chime.wav", false, false);

	_loading->LoadSoundResources(L"crush_block", L"resources\\sfx\\Sound\\crushblock.wav", false, false);
	_loading->LoadSoundResources(L"push_block", L"resources\\sfx\\Sound\\pushblock.wav", false, true);

	_loading->LoadSoundResources(L"arrow_shot", L"resources\\sfx\\Sound\\arrowshot.wav", false, false);
	_loading->LoadSoundResources(L"arrow_hit_wall", L"resources\\sfx\\Sound\\arrowhitwall.wav", false, false);


	return S_OK;
}

void MenuScene::ChangeCurrentSceneState(MenuSceneState state)
{
	switch (_currentState)
	{
	case Title :
	{
		
	}break;
	case Menu :
	{

	}break;
	}
}


void MenuScene::ShowTitleText()
{
	_showText = true;

}


HRESULT MenuScene::Init(void)
{
	if (_firstEntered)
	{
		_firstEntered = false;

		_titleTransitionTimer.Init(3.0f);
		_loadStartTimer.Init(2.0f);

		HRESULT result = LoadContent();
		Assert(SUCCEEDED(result));

		std::wstring moduleLocation = Utils::GetWorkingDirectory();
		std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

		_currentState = MenuSceneState::Title;
		EFFECTMANAGER->Init();

		_camera.Init();
		EFFECTMANAGER->SetCameraLink(&_camera);

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

			float randScale = RND->GetFloat();
			_titleObjects.bat[i].scale = Vector2(randScale, randScale);
			_titleObjects.bat[i].position = Vector2(RND->GetFloat(1500, 2300), RND->GetFloat(50, 440));
			//_titleObjects.bat[i].MoveTo(Vector2(_titleObjects.bat[i].position.x - 3000, _titleObjects.bat[i].position.y), 4.5f);
		}

		_titleObjects.player.position = Vector2(0, 240);
		_titleObjects.ground.position = Vector2(0, 280);
#pragma endregion

#pragma region MenuInit

		_dWrite.CreateTextFormat(&_menuNormalText, L"Tekton", 22);
		_dWrite.CreateTextFormat(&_menuYellowText, L"Tekton", 26);

		_menuFirstTrigger.Init(2.0f, Delegate<void>::FromFunction<MenuScene, &MenuScene::MenuAnimationEndFunction>(this));
		_menuFirstTrigger.SetValid(true);
		_menuSecondTrigger.Init(1.0f, Delegate<void>::FromFunction<MenuScene, &MenuScene::MenuAnimationEndFunction>(this));
		_menuSecondTrigger.SetValid(false);

		_menuObjects.doorBack.Init(L"mainmenu_doorback", 0, 0, 512, 512, IntVector2(-256, -256));
		_menuObjects.doorBack.position = Vector2(WINSIZEX / 2, WINSIZEY / 2);

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

		_menuObjects.alpha.Init(L"mainmenu_alpha", 0, 0, 1330, 770, IntVector2(0, 0));
		_menuObjects.alpha.position -= Vector2(25, 25);

		_menuObjects.doors[0].Init(L"mainmenu_doorandrocks", 0, 0, 320, 320, IntVector2(-320, -320));
		_menuObjects.doors[0].position = Vector2(WINSIZEX / 2 + 64, (WINSIZEY / 2 + 10) + 64);
		_menuObjects.doors[0].SetPositionEndFunction(Delegate<void>::FromFunction<MenuScene, &MenuScene::MenuAnimationEndFunction>(this));

		_menuObjects.doors[1].Init(L"mainmenu_doorandrocks", 320, 0, 320, 320, IntVector2(0, -320));
		_menuObjects.doors[1].position = Vector2(WINSIZEX / 2 - 64, (WINSIZEY / 2 + 10) + 64);
		_menuObjects.doors[2].Init(L"mainmenu_doorandrocks", 0, 320, 320, 320, IntVector2(-320, 0));
		_menuObjects.doors[2].position = Vector2(WINSIZEX / 2 + 64, (WINSIZEY / 2 + 10) - 64);
		_menuObjects.doors[3].Init(L"mainmenu_doorandrocks", 320, 320, 320, 320, IntVector2(0, 0));
		_menuObjects.doors[3].position = Vector2(WINSIZEX / 2 - 64, (WINSIZEY / 2 + 10) - 64);

		_menuObjects.body.Init(L"mainmenu_doorandrocks", 640, 0, 320, 320, IntVector2(-160, -320));
		_menuObjects.body.position = Vector2(WINSIZEX / 2, 620);
		_menuObjects.body.SetPositionEndFunction(Delegate<void>::FromFunction<MenuScene, &MenuScene::MenuAnimationEndFunction>(this));
		_menuObjects.head.Init(L"mainmenu_doorandrocks", 704, 384, 192, 192, IntVector2(-96, -96));
		_menuObjects.head.position = Vector2(WINSIZEX / 2, 367);
		_menuObjects.head.SetRotationEndFunction(Delegate<void>::FromFunction<MenuScene, &MenuScene::MenuAnimationEndFunction>(this));

		for (int i = 0; i < 6; ++i)
		{
			_menuObjects.spears[i].Init(L"mainmenu_menuspear", 0, 0, 512, 64, IntVector2(-256, -32));

			float xDelta{ -530 };
			if ((i % 2) == 1)
			{
				_menuObjects.spears[i].SetFlip(true);
				xDelta = -xDelta;
			}
			_menuObjects.spears[i].position = Vector2(WINSIZEX / 2 + xDelta, 300 + i * 50);
		}

		for (int i = 0; i < 6; ++i)
		{
			_menuObjects.ornaments[i].Init(L"mainmenu_menuspear", i * 64, 64, 64, 64, IntVector2(-32, 0));
		}

		_menuTexts[0] = L"Play Game";
		_menuTexts[1] = L"Player Stats";
		_menuTexts[2] = L"MapTool";
		_menuTexts[3] = L"Achievements";
		_menuTexts[4] = L"Help & Options";
		_menuTexts[5] = L"Exit Game";

		//SOUNDMANAGER->Play(L"menu_bgm");
	}
	else
	{
		SOUNDMANAGER->Play(L"mm_bgm");
	}

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

	_camera.Update(deltaTime);

	switch (_currentState)
	{
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

		if (_loadStartTimer.Tick(deltaTime))
		{
			_loadingStart = true;
		}
		if (_loadingStart)
		{
			float percent = _loading->LoadNext();
			_titleObjects.alpha.alpha = 1.0f - percent;
			if (percent >= 1.0f)
			{
				_titleTransitionOn = true;
			}
		}

		if (_titleTransitionOn)
		{
			if (_titleTransitionTimer.Tick(deltaTime))
			{
				_currentState = Menu;
			}
		}

		//if (_loading->LoadNext())
		//{
		//}
	}break;
	case Menu:
	{

		_menuFirstTrigger.Update(deltaTime);
		_menuSecondTrigger.Update(deltaTime);

		if (_canReceiveInput)
		{
			if (KEYMANAGER->IsOnceKeyDown('J'))
			{
				int currentTemp = _menuObjects.selectingMenuIndex;
				Vector2 deltaMove{};

				if (!_menuObjects.spears[currentTemp].GetPositionInterpolating())
				{
					SOUNDMANAGER->Play(L"tikispike");
					if ((currentTemp % 2) == 0) { deltaMove = Vector2(-500.0f, 0.0f); }
					else { deltaMove = Vector2(500.0f, 0.0f); }

					_menuObjects.spears[currentTemp].MoveBy(deltaMove, SPEAR_TIME);

					_menuObjects.selectingMenuIndex++;
					if (_menuObjects.selectingMenuIndex > 5)
					{
						_menuObjects.selectingMenuIndex = 0;
					}
					_menuObjects.spears[_menuObjects.selectingMenuIndex].MoveBy(deltaMove, SPEAR_TIME);
				}
			}
			else if (KEYMANAGER->IsOnceKeyDown('K'))
			{
				int currentTemp = _menuObjects.selectingMenuIndex;
				Vector2 deltaMove{};

				if (!_menuObjects.spears[currentTemp].GetPositionInterpolating())
				{
					SOUNDMANAGER->Play(L"tikispike");
					if ((currentTemp % 2) == 0) { deltaMove = Vector2(-500.0f, 0.0f); }
					else { deltaMove = Vector2(500.0f, 0.0f); }

					_menuObjects.spears[currentTemp].MoveBy(deltaMove, SPEAR_TIME);

					_menuObjects.selectingMenuIndex--;
					if (_menuObjects.selectingMenuIndex < 0)
					{
						_menuObjects.selectingMenuIndex = 5;
					}
					_menuObjects.spears[_menuObjects.selectingMenuIndex].MoveBy(deltaMove, SPEAR_TIME);
				}
			}
			if (KEYMANAGER->IsOnceKeyDown(VK_RETURN))
			{
				HandleSceneChange();
			}
		}

		_menuObjects.head.Update(deltaTime);
		_menuObjects.body.Update(deltaTime);
		for (int i = 0; i < 4; ++i)
		{
			_menuObjects.doors[i].Update(deltaTime);
		}

		for (int i = 0; i < 6; ++i)
		{
			_menuObjects.spears[i].Update(deltaTime);
			_menuObjects.ornaments[i].Update(deltaTime);
		}


		EFFECTMANAGER->Update(deltaTime);
	}break;
	}

}

void MenuScene::Render()
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));

	Vector2 camPos = _camera.GetPosition().UnTilelize();


	switch (_currentState)
	{
	case Title :
	{
		_titleObjects.title.Render(gRenderTarget, camPos);
		_titleObjects.player.Render(gRenderTarget, camPos);
		_titleObjects.ground.Render(gRenderTarget, camPos);
		for (int i = 0; i < 8; ++i)
		{
			_titleObjects.bat[i].RenderScale(gRenderTarget, camPos);
		}
		_titleObjects.alpha.Render(gRenderTarget, camPos);

	}break;

	case Menu:
	{
		_menuObjects.doorBack.Render(gRenderTarget, camPos);

		for (int i = 0; i < 6; ++i)
		{
			_menuObjects.spears[i].Render(gRenderTarget, camPos);
			//_menuObjects.ornaments[i].Render(gRenderTarget, camPos);
		}

		_menuObjects.body.Render(gRenderTarget, camPos);
		_menuObjects.head.RenderRotate(gRenderTarget, camPos);

		for (int i = 0; i < 4; ++i)
		{
			_menuObjects.doors[i].Render(gRenderTarget, camPos);
		}

		_menuObjects.arch.Render(gRenderTarget, camPos);

		_menuObjects.sandDirt.Render(gRenderTarget, camPos);

		_menuObjects.leftStatues.Render(gRenderTarget, camPos);
		_menuObjects.rightStatues.Render(gRenderTarget, camPos);

		_menuObjects.leftCaveSide.Render(gRenderTarget, camPos);
		_menuObjects.rightCaveSide.Render(gRenderTarget, camPos);

		EFFECTMANAGER->Render();

		if (_showText)
		{
			if (_textAlpha < 1)
			{
				_textAlpha += 0.03f;
			}
			else
			{
				_textAlpha = 1.0f;
			}

			for (int i = 0; i < 6; ++i)
			{
				if (i == _menuObjects.selectingMenuIndex)
				{
					_dWrite.PrintTextFromFormat(gRenderTarget, WINSIZEX / 2 - 250, 300 + 50 * (i - 1), 500, 100, _menuTexts[i],
						D2D1::ColorF(1, 1, 0, _textAlpha), _menuYellowText);
				}
				else
				{
					_dWrite.PrintTextFromFormat(gRenderTarget, WINSIZEX / 2 - 250, 300 + 50 * (i - 1), 500, 100, _menuTexts[i],
						D2D1::ColorF(1, 1, 1, _textAlpha), _menuNormalText);
				}
			}
		}

		_menuObjects.alpha.Render(gRenderTarget, camPos);
	}break;
	}

	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}


void MenuScene::MenuAnimationEndFunction()
{
	_menuObjects.animationEndTracker++;

	if (_menuObjects.animationEndTracker == 1)
	{
		_menuObjects.head.RotateTo(180.0f, 1.0f);
		SOUNDMANAGER->Play(L"mm_door1");
		SOUNDMANAGER->Play(L"mm_amb");
		EFFECTMANAGER->PlayDustParticles(Vector2(WINSIZEX / 2 - 26, 410));
		EFFECTMANAGER->PlayDustParticles(Vector2(WINSIZEX / 2 + 20, 424));
	}
	else if (_menuObjects.animationEndTracker == 2)
	{
		for (int i = 0; i < 4; ++i)
		{
			Vector2 delta{};
			if (i == 0) { delta.y -= 250; }
			else if (i == 1) { delta.x += 250; }
			else if (i == 2) { delta.x -= 250; }
			else if (i == 3) { delta.y += 250; }
			_menuObjects.doors[i].MoveTo(_menuObjects.doors[i].position + delta, 1.8f);
		}
		_camera.Shake(30, 50, 1.8f);
		SOUNDMANAGER->Play(L"mm_door2");
	}
	else if (_menuObjects.animationEndTracker == 3)
	{
		_menuSecondTrigger.SetValid(true);

	}
	else if (_menuObjects.animationEndTracker == 4)
	{
		EFFECTMANAGER->PlayDustParticles(Vector2(WINSIZEX / 2 - 10, 647));
		EFFECTMANAGER->PlayDustParticles(Vector2(WINSIZEX / 2 + 15, 656));
		_menuObjects.body.MoveBy(Vector2(0, 400), 3.6f);
		_menuObjects.head.MoveBy(Vector2(0, 400), 3.6f);
		_camera.Shake(8, 50, 2.0f);
		SOUNDMANAGER->Play(L"mm_door3");
	}
	else if (_menuObjects.animationEndTracker == 5)
	{
		_menuObjects.spears[0].MoveBy(Vector2(500.0f, 0), SPEAR_TIME);
		ShowTitleText();
		_canReceiveInput = true;
		SOUNDMANAGER->Play(L"mm_bgm");
	}
}

void MenuScene::HandleSceneChange()
{
	if (_menuObjects.selectingMenuIndex == 0)
	{
		SOUNDMANAGER->Stop(L"mm_bgm");
		SOUNDMANAGER->Stop(L"mm_amb");
		SOUNDMANAGER->Stop(L"mm_door1");
		SOUNDMANAGER->Stop(L"mm_door2");
		SOUNDMANAGER->Stop(L"mm_door3");
		SOUNDMANAGER->Stop(L"tikispide");

		SCENEMANAGER->ChangeScene(L"GamePlayScene");
	}
	else if (_menuObjects.selectingMenuIndex == 1)
	{

	}
	else if (_menuObjects.selectingMenuIndex == 2)
	{
		SCENEMANAGER->ChangeScene(L"MapToolScene");
	}
	else if (_menuObjects.selectingMenuIndex == 3)
	{

	}
	else if (_menuObjects.selectingMenuIndex == 4)
	{

	}
	else if (_menuObjects.selectingMenuIndex == 5)
	{

	}
}

void ImageLoadItem::Load()
{
}

void SoundLoadItem::Load()
{
}

void Loading::LoadImageResources(const std::wstring & fileName, const std::wstring & keyName)
{
	ImageLoadItem *newItem = new ImageLoadItem(fileName, keyName);
	_imageLoadItems.push_back(newItem);
	_totalSize++;
}

void Loading::LoadSoundResources(const std::wstring & fileName, const std::wstring & keyName, bool bgm, bool loop)
{
	SoundLoadItem *newItem = new SoundLoadItem(keyName, fileName, bgm, loop);
	_soundLoadItems.push_back(newItem);
	_totalSize++;
}

float Loading::LoadNext(void)
{
	float result = 0.0f;

	if (!_loadSound)
	{
		if (_imageCurrent >= _imageLoadItems.size())
		{
			_loadSound = true;
		}
		else
		{
			ImageLoadItem* item = _imageLoadItems[_imageCurrent];
			IMAGEMANAGER->LoadImageFromFile(item->GetResources().fileName, item->GetResources().keyName);
			_imageCurrent++;
		}

	}
	else
	{
		if (_soundCurrent >= _soundLoadItems.size())
		{
			result = true;
		}
		else
		{
			SoundLoadItem* item = _soundLoadItems[_soundCurrent];
			SOUNDMANAGER->AddSound(item->GetResources().fileName, item->GetResources().keyName,
				item->GetResources().isBackground, item->GetResources().loop);
			_soundCurrent++;
		}
	}

	result = ((float)(_imageCurrent + _soundCurrent) / (float)_totalSize);


	return result;

}
