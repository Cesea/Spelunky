#include "stdafx.h"
#include "UIManager.h"

#include "Player.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

HRESULT UIManager::Init()
{
	_dWrite.CreateTextFormat(&_whiteText, L"Dunkin", 30);
	_dWrite.CreateTextFormat(&_pinkText, L"Dunkin", 44);
	_dWrite.CreateTextFormat(&_smallWhiteText, L"Dunkin", 24);

	_smallWhiteText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);	
	_smallWhiteText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	_whiteText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);	
	_whiteText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);


	_pinkText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	_pinkText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);


	_playerHudSprite = new D2DFrameSprite();
	_playerHudSprite->Init(IMAGEMANAGER->GetImage(L"playerhud"), 340.0f, 85.0f, IntVector2(0, 0));

	_moneyHudSprite = new D2DSprite();
	_moneyHudSprite->Init(IMAGEMANAGER->GetImage(L"moneyhud"), 0, 0, 320.0f, 80.0f, IntVector2(0, 0));

	_exitSprite = new D2DSprite();
	_exitSprite->Init(IMAGEMANAGER->GetImage(L"exitSprite"), 0, 0, 74, 35, IntVector2(0, 0));

	_board.Init(L"board", 0, 0, 1152, 384, IntVector2(0, 0));
	_board.position = Vector2(64, 32);
	_tunnelTitle.Init(L"tunneltitle", 0, 0, 640, 160, IntVector2(0, 0));
	_tunnelTitle.position = Vector2(96, 16);
	_scroll.Init(L"scrollroll", 0, 0, 512, 128, IntVector2(0, 0));
	_scroll.position = Vector2(736, 0);
	_paper.Init(L"scrollpaper", 0, 0, 512, 512, IntVector2(0, 0));
	_paper.position = Vector2(736, 96);

	_uiElements = new D2DFrameSprite();
	_uiElements->Init(IMAGEMANAGER->GetImage(L"uiElement"), 64, 64, IntVector2(0, 0));

	_objectElements = new D2DFrameSprite();
	_objectElements->Init(IMAGEMANAGER->GetImage(L"objectSprite"), 80, 80, IntVector2(0, 0));

	_showTimer.Init(1.0f);

	RegisterDelegates();

	return S_OK;
}

void UIManager::Release()
{
	UnRegisterDelegates();
	SAFE_RELEASE_AND_DELETE(_playerHudSprite);
	SAFE_RELEASE_AND_DELETE(_moneyHudSprite);

	SAFE_RELEASE_AND_DELETE(_objectElements);
	SAFE_RELEASE_AND_DELETE(_uiElements);
}

void UIManager::Update(float deltaTime)
{
}

void UIManager::Render(const Vector2 &camPos)
{
	float deltaTime = TIMEMANAGER->GetElapsedTime();
	_playerHudSprite->FrameRender(gRenderTarget, 80, 40, 0, 0);
	_moneyHudSprite->Render(gRenderTarget, 0, 140);

	int playerHp = _pPlayer->GetHp();
	int playerBomb = _pPlayer->GetBombCount();
	int playerRope = _pPlayer->GetRopeCount();
	int playerMoney = _pPlayer->GetMoney();

	_dWrite.PrintTextFromFormat(gRenderTarget, 74, 40, 100, 50, std::to_wstring(playerHp).c_str(), 
		D2D1::ColorF(1.0, 0.6, 0.6, 1.0f), _pinkText);

	_dWrite.PrintTextFromFormat(gRenderTarget, 196, 50, 100, 50, std::to_wstring(playerBomb).c_str(),
		D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _whiteText);

	_dWrite.PrintTextFromFormat(gRenderTarget, 290, 50, 100, 50, std::to_wstring(playerRope).c_str(), 
		D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _whiteText);

	_dWrite.PrintTextFromFormat(gRenderTarget, 130, 150, 160, 50, std::to_wstring(playerMoney).c_str(), 
		D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _whiteText);

	if (_drawExitText)
	{
		Vector2 drawPos = _exitPosition.UnTilelize() - camPos;
		_exitSprite->Render(gRenderTarget, drawPos.x, drawPos.y - 32);
	}
	_drawExitText = false;

	//Console::Log("%d\n", _drawMiddleStageUI);

	if (_drawMiddleStageUI)
	{
		_board.Render(gRenderTarget, Vector2());
		_tunnelTitle.Render(gRenderTarget, Vector2());
		_scroll.Render(gRenderTarget, Vector2());
		_paper.Render(gRenderTarget, Vector2());

		RenderMiddleStageStatus(deltaTime);
	}
}

void UIManager::HandleOnTunnelEvent(const IEvent * event)
{
	OnTunnelEvent *convertedEvent = (OnTunnelEvent *)event;
	_drawExitText = true;
	_exitPosition.tileX = convertedEvent->GetPosition().x;
	_exitPosition.tileY = convertedEvent->GetPosition().y;
}

void UIManager::HandleOnMiddleStageEvent(const IEvent * event)
{
	OnMiddleStageEvent *convertedEvent = (OnMiddleStageEvent *)(event);
	_drawMiddleStageUI = true;

	_timeText.clear();
	_moneyText.clear();

	int currentStageElapsedTime = (int)STAGEMANAGER->GetCurrentStageElapsedTime();
	int currentMinute = currentStageElapsedTime / 60;
	int currentSecond = currentStageElapsedTime - (currentMinute * 60);
	_timeText += std::to_wstring(currentMinute);
	_timeText += L":";
	if (currentSecond < 10)
	{
		_timeText += L"0" + std::to_wstring(currentStageElapsedTime - (currentMinute * 60));
	}
	else
	{
		_timeText += std::to_wstring(currentStageElapsedTime - (currentMinute * 60));
	}
	_timeText += L"/";
	int totalStageElapseTime = (int)STAGEMANAGER->GetTotalStageElapsedTime();
	int totalMinute = totalStageElapseTime / 60;
	int totalSecond = totalStageElapseTime - (currentMinute * 60);

	_timeText += std::to_wstring(totalMinute);
	_timeText += L":";
	if (totalSecond < 10)
	{
		_timeText += L"0" + std::to_wstring(totalStageElapseTime - (totalMinute * 60));
	}
	else
	{
		_timeText += std::to_wstring(totalStageElapseTime - (totalMinute * 60));
	}
	_moneyText += std::to_wstring(STAGEMANAGER->GetNormalStageMoneyCollected());
	_moneyText += L"/";
	_moneyText += std::to_wstring(_pPlayer->GetMoney());
	
}

void UIManager::HandleExitMiddleStageEvent(const IEvent * event)
{
	_drawMiddleStageUI = false;
}

void UIManager::RenderMiddleStageStatus(float deltaTime)
{
	std::wstring completeText;
	completeText += std::to_wstring(STAGEMANAGER->GetCurrentStageCount());
	completeText += L" COMPLETED!";
	_dWrite.PrintTextFromFormat(gRenderTarget, 269, 60, 300, 40, completeText.c_str(), 
		D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _whiteText);

	_dWrite.PrintTextFromFormat(gRenderTarget, 190, 170, 300, 40, _timeText.c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _whiteText);
	_dWrite.PrintTextFromFormat(gRenderTarget, 190, 250, 300, 40, _moneyText.c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _whiteText);

	_uiElements->FrameRender(gRenderTarget, 124, 170, 0, 0);
	_uiElements->FrameRender(gRenderTarget, 124, 244, 1, 0);

	_objectElements->FrameRender(gRenderTarget, 805, 167, 0,  0);

	if (_showTimer.Tick(deltaTime) &&
		!_finishedShowGold && !_finishedShowEnemy)
	{
		if (!_finishedShowGold)
		{
			const IEvent *firstEvent = EVENTCOLLECTOR->GetFirstCollectMoneyEvent();
			if (firstEvent != nullptr)
			{
				CollectMoneyEvent *convertedEvent = (CollectMoneyEvent *)(firstEvent);
				_gemShowValue = convertedEvent->GetValue();
				SetShowIndexCorrespondingToGemType(convertedEvent->GetGemType());
			}
			else
			{
				_finishedShowGold = true;
			}
			SAFE_DELETE(firstEvent);
		}

		if (!_finishedShowEnemy && _finishedShowGold)
		{
			const IEvent *firstEvent = EVENTCOLLECTOR->GetFirstEnemyDeadEvent();
			if (firstEvent != nullptr)
			{
				EnemyDeadEvent *convertedEvent = (EnemyDeadEvent *)(firstEvent);
				SetShowIndexCorrespondingToEnemyType(convertedEvent->GetEnemyType());
			}
			else
			{
				_finishedShowEnemy = true;
			}
			SAFE_DELETE(firstEvent);
		}
	}

	if (_finishedShowEnemy)
	{
		if (_showTimer.Tick(deltaTime))
		{
			_endShowing = true;
		}
	}
	if (_objectShowingIndex.x != -1)
	{
		if (!_endShowing)
		{
			float t = (1.0f - _showTimer.GetCurrentSecond() / _showTimer.GetTargetSecond());
			t *= t;
			_objectElements->FrameRenderMatrix(gRenderTarget, 880, 165, _objectShowingIndex.x, _objectShowingIndex.y,
				D2D1::Matrix3x2F::Scale(t, t, D2D1::Point2F(40, 40)));

			if (!_finishedShowGold)
			{
				_dWrite.PrintTextFromFormat(gRenderTarget, 1008, 150, 140, 100, L"Gold", D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _whiteText);
				_dWrite.PrintTextFromFormat(gRenderTarget, 955, 182, 225, 52, std::to_wstring(_gemShowValue).c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _smallWhiteText);
			}
			else
			{
				_dWrite.PrintTextFromFormat(gRenderTarget, 1008, 150, 140, 100, L"Enemy", D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _whiteText);
			}
		}
	}
}

void UIManager::SetShowIndexCorrespondingToGemType(GemType type)
{
	switch (type)
	{
	case GEM_Emerald : { _objectShowingIndex = IntVector2(0, 1); }break;
	case GEM_Saphire: { _objectShowingIndex = IntVector2(1, 1); }break;
	case GEM_Ruby: { _objectShowingIndex = IntVector2(2, 1); }break;
	case GEM_Diamond: { _objectShowingIndex = IntVector2(3, 1); }break;
	case GEM_Ingot: { _objectShowingIndex = IntVector2(4, 1); }break;
	case GEM_ThreeIngot: { _objectShowingIndex = IntVector2(5, 1); }break;
	case GEM_Nugget: { _objectShowingIndex = IntVector2(5, 1); }break;
	}
}

void UIManager::SetShowIndexCorrespondingToEnemyType(EnemyType type)
{
	switch (type)
	{
	case ENEMY_Snake: { _objectShowingIndex = IntVector2(0, 2); }break;
	case ENEMY_Spider: { _objectShowingIndex = IntVector2(1, 2); }break;
	case ENEMY_Primitive: { _objectShowingIndex = IntVector2(2, 2); }break;
	case ENEMY_Bat: { _objectShowingIndex = IntVector2(3, 2); }break;
	}
}

void UIManager::RegisterDelegates()
{
	EVENTMANAGER->RegisterDelegate(EVENT_ON_TUNNEL, EventDelegate::FromFunction<UIManager, &UIManager::HandleOnTunnelEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_ON_MIDDLE_STAGE, EventDelegate::FromFunction<UIManager, &UIManager::HandleOnMiddleStageEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_EXIT_MIDDLE_STAGE, EventDelegate::FromFunction<UIManager, &UIManager::HandleExitMiddleStageEvent>(this));
}

void UIManager::UnRegisterDelegates()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_ON_TUNNEL, EventDelegate::FromFunction<UIManager, &UIManager::HandleOnTunnelEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_ON_MIDDLE_STAGE, EventDelegate::FromFunction<UIManager, &UIManager::HandleOnMiddleStageEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_EXIT_MIDDLE_STAGE, EventDelegate::FromFunction<UIManager, &UIManager::HandleExitMiddleStageEvent>(this));
}
