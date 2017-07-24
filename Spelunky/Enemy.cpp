#include "stdafx.h"
#include "Enemy.h"

#include "CollisionComponent.h"

void Enemy::SetGraphics(const std::wstring & key)
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

Enemy::Enemy(ObjectId id)
	:MovingObject::MovingObject(id)
{
}

Enemy::~Enemy()
{
	_graphics.Release();
}

HRESULT Enemy::Init(BaseProperty * property)
{
	_collisionComp = new CollisionComponent;
	_collisionComp->Init(RectMake(0, 0, 38, 44), Vector2(-19, -44));

	_speed = Vector2(460, 300);
	_maxVelocity = Vector2(340, 630);

	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<Enemy, &Enemy::HandlePlayerAttackEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<Enemy, &Enemy::HandlePlayerAttackEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<Enemy, &Enemy::HandlePlayerAttackEvent>(this));

	return S_OK;
}

void Enemy::Release(void)
{
}

void Enemy::Update(float deltaTime)
{
}

void Enemy::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
}

GameObject * Enemy::Copy(ObjectId id)
{
	return nullptr;
}

void Enemy::HandleEnemyInputEvent(const IEvent * event)
{
}

void Enemy::HandlePalyerInputEvent(const IEvent * event)
{
}

void Enemy::HandlePlayerAttackEvent(const IEvent * event)
{
}
