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
	_dWrite.CreateTextFormat(&_smallBlackText, L"Tekton", 30);
	_dWrite.CreateTextFormat(&_bigBlackText, L"Tekton", 38);
	_dWrite.CreateTextFormat(&_whiteText, L"Tekton", 26);
	_dWrite.CreateTextFormat(&_pinkText, L"Tekton", 34);

	_playerHudSprite = new D2DFrameSprite();
	_playerHudSprite->Init(IMAGEMANAGER->GetImage(L"playerhud"), 340.0f, 85.0f, IntVector2(0, 0));

	_moneyHudSprite = new D2DSprite();
	_moneyHudSprite->Init(IMAGEMANAGER->GetImage(L"moneyhud"), 0, 0, 320.0f, 80.0f, IntVector2(0, 0));

	_exitSprite = new D2DSprite();
	_exitSprite->Init(IMAGEMANAGER->GetImage(L"exitSprite"), 0, 0, 74, 35, IntVector2(0, 0));

	RegisterDelegates();

	return S_OK;
}

void UIManager::Release()
{
	UnRegisterDelegates();
	SAFE_RELEASE_AND_DELETE(_playerHudSprite);
	SAFE_RELEASE_AND_DELETE(_moneyHudSprite);
}

void UIManager::Update(float deltaTime)
{
}

void UIManager::Render(const Vector2 &camPos)
{
	_playerHudSprite->FrameRender(gRenderTarget, 80, 40, 0, 0);
	_moneyHudSprite->Render(gRenderTarget, 0, 140);

	_dWrite.PrintTextFromFormat(gRenderTarget, 105, 155, 110, 30, std::to_wstring(_pPlayer->GetMoney()).c_str(), 
		D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), _smallBlackText);
	_dWrite.PrintTextFromFormat(gRenderTarget, 105, 155, 110, 30, std::to_wstring(_pPlayer->GetMoney()).c_str(), 
		D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), _whiteText);

	if (_drawExitText)
	{
		Vector2 drawPos = _exitPosition.UnTilelize() - camPos;
		_exitSprite->Render(gRenderTarget, drawPos.x, drawPos.y - 32);
	}
	_drawExitText = false;
}

void UIManager::HandleOnTunnelEvent(const IEvent * event)
{
	OnTunnelEvent *convertedEvent = (OnTunnelEvent *)event;
	_drawExitText = true;
	_exitPosition.tileX = convertedEvent->GetPosition().x;
	_exitPosition.tileY = convertedEvent->GetPosition().y;
}

void UIManager::RegisterDelegates()
{
	EVENTMANAGER->RegisterDelegate(EVENT_ON_TUNNEL, EventDelegate::FromFunction<UIManager, &UIManager::HandleOnTunnelEvent>(this));
}

void UIManager::UnRegisterDelegates()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_ON_TUNNEL, EventDelegate::FromFunction<UIManager, &UIManager::HandleOnTunnelEvent>(this));
}
