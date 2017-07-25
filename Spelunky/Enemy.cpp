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

void Enemy::Damaged(int damage, Direction hitDirection)
{
	_hp -= 1;
	if (_hp <= 0)
	{
		EVENTMANAGER->QueueEvent(new EnemyDeadEvent(_id, _enemyType));
	}
	else
	{
		if (hitDirection == Direction::Right)
		{
			_velocity.x = -140;
		}
		else if (hitDirection == Direction::Left)
		{
			_velocity.x = 140;
		}
		_velocity.y -= 80;
	}
}

Enemy::Enemy(ObjectId id)
	:MovingObject::MovingObject(id)
{
}

Enemy::~Enemy()
{
}

HRESULT Enemy::Init(BaseProperty * property)
{

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

void Enemy::HandlePlayerPositionEvent(const IEvent * event)
{
}

void Enemy::HandlePlayerAttackEvent(const IEvent * event)
{
	PlayerAttackEvent *convertedEvent = (PlayerAttackEvent *)event;

	const TilePosition &playerTilePosition =  convertedEvent->GetTilePosition();
	int tileXDiff = playerTilePosition.tileX - position.tileX;
	int tileYDiff = playerTilePosition.tileY - position.tileY;

	if (abs(tileXDiff) >= 3 || abs(tileYDiff) >= 3)
	{
		return;
	}
	
	const Vector2 &playerUntiled = playerTilePosition.UnTilelize();
	const Vector2 untiled = position.UnTilelize();

	Direction seeingDirection = convertedEvent->GetDirection();

	Vector2 positionUntiled = position.UnTilelize();
	Vector2 playerPositionUntiled = playerTilePosition.UnTilelize();

	float xDiff = positionUntiled.x - playerPositionUntiled.x;

	bool hitted = false;
	if (seeingDirection == Direction::Left)
	{
		if (positionUntiled.x <= playerPositionUntiled.x + 15 &&
			positionUntiled.x >= playerPositionUntiled.x - 83 &&
			position.tileY == playerTilePosition.tileY)
		{
			hitted = true;
		}
	}
	else if (seeingDirection == Direction::Right)
	{
		if (positionUntiled.x >= playerPositionUntiled.x - 15 &&
			positionUntiled.x <= playerPositionUntiled.x + 83 &&
			position.tileY == playerTilePosition.tileY)
		{

			hitted = true;
		}
	}
	if (hitted)
	{
		Damaged(1, (xDiff < 0) ? Direction::Right : Direction::Left);
	}
}
