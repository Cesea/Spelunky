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
	SOUNDMANAGER->Play(L"hit");
}

Enemy::Enemy(ObjectId id)
	:MovingObject::MovingObject(id)
{
	_eventDispatchTimer.Init(0.1f);
}

Enemy::~Enemy()
{
}

HRESULT Enemy::Init(BaseProperty * property)
{

	return S_OK;
}

void Enemy::PostInit()
{
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

void Enemy::HandleDamageEvent(const IEvent * event)
{
	DamageEvent *convertedEvent = (DamageEvent *)(event);
	if (_id == convertedEvent->GetAttackerId())
	{
		return;
	}

	const TilePosition &attackerPosition = convertedEvent->GetTilePosition();
	int tileXDiff = attackerPosition.tileX - position.tileX;
	int tileYDiff = attackerPosition.tileY - position.tileY;

	if (abs(tileXDiff) >= 2 || abs(tileYDiff) > 2)
	{
		return;
	}

	const Rect &attackerRect = convertedEvent->GetRect();
	const Vector2 &attackerRectOffset = convertedEvent->GetRectOffset();

	Rect attackerAbsRect = attackerRect + attackerRectOffset + attackerPosition.UnTilelize();
	Rect thisAbsRect = _collisionComp->GetRect() + _collisionComp->GetOffset() + position.UnTilelize();

	Rect overlapRect;
	if (IsRectangleOverlap(attackerAbsRect, thisAbsRect, overlapRect))
	{
		MovingObject *attacker = (MovingObject *)OBJECTMANAGER->FindObjectId(convertedEvent->GetAttackerId());
		EVENTMANAGER->QueueEvent(new DamageEvent(_id, 1, attacker->position, _collisionComp->GetRect(), _collisionComp->GetOffset() ));

		if (attacker == nullptr)
		{
			Damaged(5, (tileXDiff >= 0) ? Direction::Right : Direction::Left);
		}
		else
		{
			Vector2 attackerVel = attacker->GetVelocity();
			attackerVel.x *= -0.5;
			if (attackerVel.y < 0)
			{
				attackerVel.y *= 0.4;
			}
			attacker->SetVelocity(attackerVel);
			Damaged(1, (tileXDiff >= 0) ? Direction::Right : Direction::Left);
		}
	}
}

void Enemy::HandleObstaclePositionEvent(const IEvent * event)
{
	ObstaclePositionEvent *convertedEvent = (ObstaclePositionEvent *)(event);

	const TilePosition &obstacleTilePos = convertedEvent->GetPosition();
	int tileXDiff = obstacleTilePos.tileX - position.tileX;
	int tileYDiff = obstacleTilePos.tileY - position.tileY;

	if (abs(tileXDiff) >= 3 || abs(tileYDiff) >= 3)
	{
		return;
	}
	const Rect &obstacleRect = convertedEvent->GetRect();
	const Vector2 &obstacleUntiledPosition = convertedEvent->GetPosition().UnTilelize();

	const Vector2 itemUntiledPosition = position.UnTilelize();

	Rect obstacleAbsRect = RectMake(obstacleUntiledPosition.x, obstacleUntiledPosition.y,
		obstacleRect.width, obstacleRect.height);
	obstacleAbsRect += convertedEvent->GetRectOffset();

	Rect itemAbsRect =
		RectMake(itemUntiledPosition.x, itemUntiledPosition.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	itemAbsRect += _collisionComp->GetOffset();

	float relXDiff = itemUntiledPosition.x - obstacleUntiledPosition.x;
	float relYDiff = itemUntiledPosition.y - obstacleUntiledPosition.y;

	Rect overlapRect;
	if (IsRectangleOverlap(obstacleAbsRect, itemAbsRect, overlapRect))
	{
		if (overlapRect.width > overlapRect.height)
		{
			if (relYDiff > 0)
			{
				position.AddToTileRelY(overlapRect.height);
				_velocity.y = 0;
				_velocity.x *= 0.5;
			}
			else
			{
				position.AddToTileRelY(-overlapRect.height);
				_velocity.y = 0;
				_velocity.x *= 0.5;
			}
		}
		else
		{
			if (relXDiff > 0)
			{
				position.AddToTileRelX(overlapRect.width);
				_velocity.x *= -0.5;
			}
			else
			{
				position.AddToTileRelX(-overlapRect.width);
				_velocity.x *= -0.5;
			}
		}
		desiredPosition = position;
	}
}
