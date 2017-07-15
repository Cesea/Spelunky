#include "stdafx.h"
#include "Player.h"

#include "IdleState.h"

Player::Player(ObjectId id)
	:MovingObject::MovingObject(id)
{
	_seeingDirection = Direction::Left;

	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<Player, &Player::HandlePlayerInputEvent>(this));
}

Player::~Player()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<Player, &Player::HandlePlayerInputEvent>(this));
}

HRESULT Player::Init(ArcheType type)
{
	Animation *idleAnimation = new Animation;
	idleAnimation->InitCopy(KEYANIMANAGER->FindAnimation(L"char_orange_idle"));
	D2DSprite *idleSprite = new D2DAnimationSprite;
	idleSprite->Init(IMAGEMANAGER->GetImage(L"char_orange"), idleAnimation, IntVector2(-40, -72));

	Animation *walkAnimation = new Animation;
	walkAnimation->InitCopy(KEYANIMANAGER->FindAnimation(L"char_orange_walk"));
	D2DSprite *walkSprite = new D2DAnimationSprite;
	walkSprite->Init(IMAGEMANAGER->GetImage(L"char_orange"), walkAnimation, IntVector2(-40, -72));

	Animation *crawlAnimation = new Animation;
	crawlAnimation->InitCopy(KEYANIMANAGER->FindAnimation(L"char_orange_crawl"));
	D2DSprite *crawlSprite = new D2DAnimationSprite;
	crawlSprite->Init(IMAGEMANAGER->GetImage(L"char_orange"), crawlAnimation, IntVector2(-40, -72));

	Animation *standUpAnimation = new Animation;
	standUpAnimation->InitCopy(KEYANIMANAGER->FindAnimation(L"char_orange_standUp"));
	D2DSprite *standUpSprite = new D2DAnimationSprite;
	standUpSprite->Init(IMAGEMANAGER->GetImage(L"char_orange"), standUpAnimation, IntVector2(-40, -72));

	_graphics.AddData(L"idle", idleSprite);
	_graphics.AddData(L"walk", walkSprite);
	_graphics.AddData(L"crawl", crawlSprite);
	_graphics.AddData(L"standUp", standUpSprite);

	SetGraphics(L"idle");

	_stateManager.Init(this, new IdleState);

	return S_OK;
}

void Player::Release(void)
{
	_graphics.Release();
}

void Player::Update(float deltaTime)
{
	_stateManager.Update(deltaTime);
}

void Player::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position.UnTilelize() -camPos;
	_currentSprite->Render(renderTarget, drawingPos.x, drawingPos.y);
}

void Player::Serialize(FileUtils::File & file)
{
}

void Player::DeSerialize(FileUtils::File & file)
{
}

GameObject * Player::Copy(ObjectId id)
{
	return nullptr;
}

void Player::HandlePlayerInputEvent(const IEvent * event)
{
	PlayerInputEvent *convertedEvent = (PlayerInputEvent *)(event);
	const ControlCommand &controlCommand = convertedEvent->GetControlCommand();
	_stateManager.HandleCommand(controlCommand);
}

void Player::HandleFrameEndEvent(const IEvent * event)
{
	//지금 생각으로는 이 이벤트를 오브젝트 메니져에서 관리를 해야 할 것 같다
	//각각의 클래스에서 처리하면 어쨌던 찾아야 한다...
	FrameEndedEvent *convertedEvent = (FrameEndedEvent *)(event);
	_stateManager.HandleFrameEndEvent();
}

void Player::HandleMessage(const IEvent * event)
{
}

void Player::SetGraphics(const std::wstring & key)
{
	D2DSprite *found = _graphics.FindData(key);
	if (found != nullptr)
	{
		if (_currentSprite)
		{
			_currentSprite->GetAnimation()->Stop();
		}
		_currentSprite = found;
		_currentSprite->GetAnimation()->Start();
		_currentSprite->SyncFlip(_seeingDirection);
	}
}
