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

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\monsters.png", L"monsters");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\particles.png", L"particles");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\crates.png", L"crates");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\board.png", L"board");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\tunneltitle.png", L"tunneltitle");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\scrollroll.png", L"scrollroll");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\scrollpaper.png", L"scrollpaper");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\uiElement.png", L"uiElement");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\objectSprite.png", L"objectSprite");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\bookbg.png", L"bookbg");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\bookgameover.png", L"bookgameover");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\buttonSprite.png", L"buttonSprite");

#pragma region Animation For Player
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
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_attack", L"char_orange", 80, 80, attackArray, 6, 16, false);

	int throwArray[] = {54, 55, 56, 57, 58};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_throw", L"char_orange", 80, 80, throwArray, 5, 10, false);

	int exitArray[] = {60, 61, 62, 63, 64, 65};
	KEYANIMANAGER->AddArrayFrameAnimation(L"char_orange_exit", L"char_orange", 80, 80, exitArray, 6, 10, true);

#pragma endregion


#pragma region Animation Weapons
	int whipArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"weapon_whip", L"weaponanimation", 80, 80, whipArray, 11, 24, false);

	int mattockArray[] = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"weapon_mattock", L"weaponanimation", 80, 80, mattockArray, 11, 24, false);

	int macheteArray[] = {22, 23, 24, 25, 26 , 27, 28, 29, 30, 31, 32};
	KEYANIMANAGER->AddArrayFrameAnimation(L"weapon_machete", L"weaponanimation", 80, 80, macheteArray, 11, 24, false);
#pragma endregion

#pragma region Animation Bomb
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

#pragma region Animations for Monsters

	int snakeWalkArary[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	KEYANIMANAGER->AddArrayFrameAnimation(L"snake_walk", L"monsters", 80, 80, snakeWalkArary, 11, 12, true);
	int snakeAttackArray[] = {12, 13, 14, 15, 16, 17, 18};
	KEYANIMANAGER->AddArrayFrameAnimation(L"snake_attack", L"monsters", 80, 80, snakeAttackArray, 7, 12, false);

	int spiderFlipArray[] = { 24, 25, 26, 27, 28, 29 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"spider_flip", L"monsters", 80, 80, spiderFlipArray, 6, 10, false);
	int spiderOnGroundArray[] = {30, 31, 32, 33};
	KEYANIMANAGER->AddArrayFrameAnimation(L"spider_ground", L"monsters", 80, 80, spiderOnGroundArray, 4, 8, true);
	int spiderJumpArray[] = { 19, 20, 21, 22 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"spider_jump", L"monsters", 80, 80, spiderJumpArray, 4, 10, false);

	int primitiveWalkArray[] = {36, 37, 38, 39, 40, 41, 42, 43, 44};
	KEYANIMANAGER->AddArrayFrameAnimation(L"primitive_walk", L"monsters", 80, 80, primitiveWalkArray, 9, 12, true);
	int primitiveDeadArray[] = {45, 46, 47, 48, 49};
	KEYANIMANAGER->AddArrayFrameAnimation(L"primitive_dead", L"monsters", 80, 80, primitiveWalkArray, 5, 12, false);

	int batOnTopArray[] = { 60, 61, 62, 63, 64, 65, 66, 67 };
	KEYANIMANAGER->AddArrayFrameAnimation(L"bat_on_top", L"monsters", 80, 80, batOnTopArray, 8, 10, true);
	int batFlyArray[] = {72, 73, 74, 75, 76, 77, 78};
	KEYANIMANAGER->AddArrayFrameAnimation(L"bat_fly", L"monsters", 80, 80, batFlyArray, 7, 16, true);

	int strongSnakeWalkArray[] = {84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95};
	KEYANIMANAGER->AddArrayFrameAnimation(L"strong_snake_walk", L"monsters", 80, 80, strongSnakeWalkArray, 12, 12, true);
	int strongSnakeAttackArray[] = { 96, 97, 98, 99, 100};
	KEYANIMANAGER->AddArrayFrameAnimation(L"strong_snake_attack", L"monsters", 80, 80, strongSnakeAttackArray, 5, 12, false);
	int strongSnakeSpitArray[] = {101, 102, 103, 104, 105, 106, 107};
	KEYANIMANAGER->AddArrayFrameAnimation(L"strong_snake_spit", L"monsters", 80, 80, strongSnakeSpitArray, 7, 12, false);

#pragma endregion

	return S_OK;
}

void GamePlayScene::RegisterDelegates()
{
	EVENTMANAGER->RegisterDelegate(EVENT_LAYER_ON, 
		EventDelegate::FromFunction<GamePlayScene, &GamePlayScene::HandleLayerOnEvent>(this));
	//EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_DEAD, 
	//	EventDelegate::FromFunction<GamePlayScene, &GamePlayScene::HandlePlayerDeadEvent>(this));
}

void GamePlayScene::UnRegisterDelegates()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_LAYER_ON, 
		EventDelegate::FromFunction<GamePlayScene, &GamePlayScene::HandleLayerOnEvent>(this));
	//EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_DEAD, 
	//	EventDelegate::FromFunction<GamePlayScene, &GamePlayScene::HandlePlayerDeadEvent>(this));
}

void GamePlayScene::HandleLayerOnEvent(const IEvent * event)
{
	//gRenderTarget->BeginDraw();
	//gRenderTarget->Clear(D2D1::ColorF(0.0, 0.0f, 0.0f, 1.0f));

	//gRenderTarget->EndDraw();

	LayerOnEvent *convertedEvent = (LayerOnEvent *)event;
	//넓히는것
	if (convertedEvent->GetWiden())
	{
		_enterTheStage = true;
		_startLayeredRadius = 3;
		_targetLayeredRadius = 1000;
		_camera.Update(0.016);
	}
	else
	{
		_enterTheStage = false;
		_startLayeredRadius = 1000;
		_targetLayeredRadius = 3;

		//if (convertedEvent->GetPlayerDead())
		//{
		//	_sceneState = PlaySceneState::PlayerDead;
		//}
	}
	_layerRenderOn = true;

	_layeredCenter = convertedEvent->GetPosition().UnTilelize() - _camera.GetPosition().UnTilelize();
	_updateOthers = convertedEvent->GetUpdateOthers();

}

HRESULT GamePlayScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));

	RegisterDelegates();

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

	OBJECTMANAGER->SetCurrentScene(this);

	PlayerProperty playerProperty{};
	_pPlayer = (Player *)OBJECTMANAGER->CreateObject(L"player", &playerProperty);

	_dWrite.CreateTextFormat(&_smallText, L"Dunkin", 25);
	_dWrite.CreateTextFormat(&_bigText, L"Dunkin", 25);

	_dWrite.CreateTextFormat(&_deadOverText, L"Tekton", 30);
	_dWrite.CreateTextFormat(&_deadBlackText, L"Tekton", 20);
	_dWrite.CreateTextFormat(&_deadWhiteText, L"Dunkin", 19);


	_deadOverText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	_deadOverText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	_deadWhiteText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	_deadWhiteText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	_deadBlackText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	_deadBlackText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

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

	_layerRenderTimer.Init(1.0f);

	_sceneState = PlaySceneState::OnDungeon;

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\bookbg.png", L"bookbg");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\bookgameover.png", L"bookgameover");

	_deadBackground = new D2DSprite;
	_deadBackground->Init(IMAGEMANAGER->GetImage(L"bookbg"), 0, 0, WINSIZEX, WINSIZEY, IntVector2(-WINSIZEX / 2, -WINSIZEY / 2));
	_deadBook = new D2DSprite;
	_deadBook->Init(IMAGEMANAGER->GetImage(L"bookgameover"), 0, 0, 1024, 512, IntVector2(-512, -256));

	_buttonSprite = new D2DSprite;
	_buttonSprite->Init(IMAGEMANAGER->GetImage(L"buttonSprite"), 0, 0, 32, 32, IntVector2(0, 0));

	return S_OK;
}

void GamePlayScene::Release(void)
{
	STAGEMANAGER->Release();
	UnRegisterDelegates();

	SAFE_RELEASE_AND_DELETE(_deadBackground);
	SAFE_RELEASE_AND_DELETE(_deadBook);
}

void GamePlayScene::Update(void)
{
	float deltaTime = TIMEMANAGER->GetElapsedTime();

	//던전씬에서의 업데이트
	if (_sceneState == PlaySceneState::OnDungeon)
	{
		if (_updateOthers)
		{
			Win32RawInputState rawInput = {};
			PullRawInput(&rawInput);
			ControlCommand playerCommand = _inputMapper.InterpretRawInput(&rawInput);
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
		}

		if (_layerRenderOn)
		{
			if (_layerRenderTimer.Tick(deltaTime))
			{
				_layerRenderTimer.Reset();
				if (_enterTheStage)
				{
					_layerRenderOn = false;
					_updateOthers = true;
				}
				else
				{
					_pPlayer->Reset();
					if (_pPlayer->GetDead())
					{
						EVENTMANAGER->DiscardAllEvents();
						_sceneState = PlaySceneState::PlayerDead;
					}
					else
					{
						_layerRenderOn = false;
						EVENTMANAGER->DiscardAllEvents();
						EVENTMANAGER->FireEvent(new StageTransitionEvent());
					}
				}
			}
			else
			{
				_t = _layerRenderTimer.GetCurrentSecond() / _layerRenderTimer.GetTargetSecond();
			}
		}
		if (KEYMANAGER->IsOnceKeyDown(VK_ESCAPE))
		{
			EVENTMANAGER->DiscardAllEvents();
			SCENEMANAGER->ChangeScene(L"MenuScene");
		}
	}
	//플레이어 죽은 상태에서 업데이트
	else if (_sceneState == PlaySceneState::PlayerDead)
	{
		if (KEYMANAGER->IsOnceKeyDown('Z'))
		{
			STAGEMANAGER->Reset();
			EVENTCOLLECTOR->Reset();
			SCENEMANAGER->ChangeScene(L"MenuScene");
		}
		else if (KEYMANAGER->IsOnceKeyDown('X'))
		{
			//ResetToLobyStage();
		}
	}
}

void GamePlayScene::Render(void)
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));

	Vector2 unTiledCamPos = _camera.GetPosition().UnTilelize();

	if (_sceneState == PlaySceneState::OnDungeon)
	{
		if (_layerRenderOn)
		{
			D2D1_GRADIENT_STOP gradientStops[3];
			gradientStops[0].color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
			gradientStops[0].position = 0.0f;
			gradientStops[1].color = D2D1::ColorF(0.0, 0.0, 0.0, 1.0f);
			gradientStops[1].position = 0.98f;
			gradientStops[2].color = D2D1::ColorF(0.0, 0.0, 0.0, 0.0f);
			gradientStops[2].position = 1.0f;

			gRenderTarget->CreateGradientStopCollection(gradientStops,
				3, &_pGradientStopCollection);

			gRenderTarget->CreateRadialGradientBrush(
				D2D1::RadialGradientBrushProperties(D2D1::Point2F(_layeredCenter.x, _layeredCenter.y), D2D1::Point2F(0, 0),
					InterpolateFloat(_startLayeredRadius, _targetLayeredRadius, _t), InterpolateFloat(_startLayeredRadius, _targetLayeredRadius, _t)),
				_pGradientStopCollection,
				&_radialBrush);

			ID2D1Layer *layer{};
			gRenderTarget->CreateLayer(&layer);
			gRenderTarget->PushLayer(D2D1::LayerParameters(
				D2D1::InfiniteRect(),
				NULL,
				D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
				D2D1::IdentityMatrix(),
				1.0f,
				_radialBrush,
				D2D1_LAYER_OPTIONS_NONE),
				layer);

			STAGEMANAGER->Render(Vector2(0, 0));
			_pPlayer->Render(gRenderTarget, unTiledCamPos);
			EFFECTMANAGER->Render();
			UIMANAGER->Render(unTiledCamPos);

			gRenderTarget->PopLayer();

			if (_radialBrush != nullptr)
			{
				_radialBrush->Release();
			}

		}
		else
		{
			STAGEMANAGER->Render(Vector2());

			//_pPlayer->Render(gRenderTarget, unTiledCamPos);

			EFFECTMANAGER->Render();

			UIMANAGER->Render(unTiledCamPos);
		}
	}
	else if (_sceneState == PlaySceneState::PlayerDead)
	{
		Vector2 offset = (_pPlayer->position.UnTilelize() - _camera.GetPosition().UnTilelize()) - Vector2(440, 256);

		STAGEMANAGER->Render(Vector2(offset));

		std::wstring timeText{};

		int totalStageElapseTime = (int)STAGEMANAGER->GetTotalStageElapsedTime();
		int totalMinute = totalStageElapseTime / 60;
		int totalSecond = totalStageElapseTime - (totalMinute * 60);

		timeText += std::to_wstring(totalMinute);
		timeText += L":";
		if (totalSecond < 10)
		{
			timeText += L"0" + std::to_wstring(totalStageElapseTime - (totalMinute * 60));
		}
		else
		{
			timeText += std::to_wstring(totalStageElapseTime - (totalMinute * 60));
		}

		_deadBackground->Render(gRenderTarget, WINSIZEX / 2, WINSIZEY / 2);
		_deadBook->Render(gRenderTarget, WINSIZEX / 2, WINSIZEY / 2);

		_dWrite.PrintTextFromFormat(gRenderTarget, 702, 160, 284, 72, L"GameOver", D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _deadOverText);

		_dWrite.PrintTextFromFormat(gRenderTarget, 728, 226, 116, 32, L"Level:", D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), _deadBlackText);
		_dWrite.PrintTextFromFormat(gRenderTarget, 728, 226 + 39 * 1, 116, 32, L"Money:", D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), _deadBlackText);
		_dWrite.PrintTextFromFormat(gRenderTarget, 728, 226 + 39 * 2, 116, 32, L"Time:", D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), _deadBlackText);
		_dWrite.PrintTextFromFormat(gRenderTarget, 728, 226 + 39 * 3, 116, 32, L"Damsels:", D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), _deadBlackText);
		_dWrite.PrintTextFromFormat(gRenderTarget, 728, 226 + 39 * 4, 116, 32, L"Kills:", D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), _deadBlackText);
		_dWrite.PrintTextFromFormat(gRenderTarget, 728, 226 + 39 * 5, 116, 32, L"TotalDeaths:", D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), _deadBlackText);

		_dWrite.PrintTextFromFormat(gRenderTarget, 728 + 160, 226 + 39 * 0, 116, 32,
			std::to_wstring(STAGEMANAGER->GetCurrentStageCount()).c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _deadWhiteText);
		_dWrite.PrintTextFromFormat(gRenderTarget, 728 + 160, 226 + 39 * 1, 116, 32,
			std::to_wstring(_pPlayer->GetMoney()).c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _deadWhiteText);
		_dWrite.PrintTextFromFormat(gRenderTarget, 728 + 160, 226 + 39 * 2, 116, 32,
			timeText.c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _deadWhiteText);
		_dWrite.PrintTextFromFormat(gRenderTarget, 728 + 160, 226 + 39 * 4, 116, 32,
			std::to_wstring(EVENTCOLLECTOR->GetEnemyDeadCount()).c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _deadWhiteText);

		_dWrite.PrintTextFromFormat(gRenderTarget, 663, 471, 362, 106, 
			L"Continue\nQuick Restart", D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _deadOverText);

		_buttonSprite->Render(gRenderTarget, 750, 474);
		_dWrite.PrintTextFromFormat(gRenderTarget, 728, 476, 40, 40, L"Z", D2D1::ColorF(0, 0, 0, 1), _deadBlackText);
		_buttonSprite->Render(gRenderTarget, 717, 510);
		_dWrite.PrintTextFromFormat(gRenderTarget, 696, 514, 40, 40, L"X", D2D1::ColorF(0,0,0,1), _deadBlackText);

	}

	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}