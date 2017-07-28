#include "stdafx.h"
#include "ArrowRock.h"

#include "CollisionComponent.h"

ArrowRock::ArrowRock(ObjectId id)
	:Obstacle(id)
{
}

ArrowRock::~ArrowRock()
{

}

HRESULT ArrowRock::Init(BaseProperty * property)
{
	_sourceIndex = IntVector2(1, 0);
	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"obstacles"), 64, 64, IntVector2(-32, -64));


	_collisionComp = new CollisionComponent;
	_collisionComp->Init(RectMake(0, 0, 64, 64), Vector2(-32, -64));

	ObstacleProperty *convertedProperty = (ObstacleProperty *)property;
	_obstacleType = OBSTACLE_BombCrate;
	position.tileX = convertedProperty->position.x;
	position.tileY = convertedProperty->position.y;
	position.AddToTileRel(32, 64);
	desiredPosition = position;

	_facingDirection = convertedProperty->facingDirection;

	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<ArrowRock, &ArrowRock::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_OBSTACLE_POSITION, EventDelegate::FromFunction<ArrowRock, &ArrowRock::HandleObstaclePositionEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_ENEMY_POSITION, EventDelegate::FromFunction<ArrowRock, &ArrowRock::HandleEnemyPositionEvent>(this));
	_dispatchTimer.Init(0.04);

	return S_OK;
}

void ArrowRock::Release(void)
{
	SAFE_RELEASE_AND_DELETE(_sprite);
	SAFE_DELETE(_collisionComp);

	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<ArrowRock, &ArrowRock::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_OBSTACLE_POSITION, EventDelegate::FromFunction<ArrowRock, &ArrowRock::HandleObstaclePositionEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_ENEMY_POSITION, EventDelegate::FromFunction<ArrowRock, &ArrowRock::HandleEnemyPositionEvent>(this));
}

void ArrowRock::Update(float deltaTime)
{
	_accel.y += GRAVITY;
	if (_onObject)
	{
		_accel.y -= GRAVITY;
		_velocity.y = 0;
	}
	_prevYVel = _velocity.y;
	_velocity += _accel * deltaTime;

	_accel = Vector2();
	desiredPosition.AddToTileRel(_velocity * deltaTime);

	TilePosition centerPos = desiredPosition;
	centerPos.AddToTileRelY(-16.0f);
	_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));
	_collisionComp->Update(this, deltaTime, &_nearTiles);
	if (_prevYVel > 50 && _velocity.y < 50)
	{
		SOUNDMANAGER->Play(L"crush_block");
	}

	if (_dispatchTimer.Tick(deltaTime))
	{
		EVENTMANAGER->QueueEvent(new ObstaclePositionEvent(_id, position, _collisionComp->GetRect(), _collisionComp->GetOffset()));
	}
}

void ArrowRock::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	if (_facingDirection == Direction::Left)
	{
		_sprite->FrameRenderFlip(renderTarget, drawPos.x, drawPos.y, _sourceIndex.x, _sourceIndex.y);
	}
	else if (_facingDirection == Direction::Right)
	{
		_sprite->FrameRender(renderTarget, drawPos.x, drawPos.y, _sourceIndex.x, _sourceIndex.y);
	}
}

GameObject * ArrowRock::Copy(ObjectId id)
{
	return nullptr;
}

void ArrowRock::HandlePlayerPositionEvent(const IEvent * event)
{
	PlayerPositionEvent *convertedEvent = (PlayerPositionEvent *)(event);
	const TilePosition &playerTilePosition = convertedEvent->GetPosition();
	int tileXDiff = playerTilePosition.tileX - position.tileX;
	int tileYDiff = playerTilePosition.tileY - position.tileY;

	if (abs(tileXDiff) >= 6 || abs(tileYDiff) >= 3)
	{
		return;
	}

	Player *pPlayer = (Player *)OBJECTMANAGER->FindObjectId(convertedEvent->GetId());

	Vector2 &playerUntiled = playerTilePosition.UnTilelize();
	const Vector2 untiled = position.UnTilelize();
	const Rect &playerRect = convertedEvent->GetRect();

	Rect playerAbsRect = RectMake(playerUntiled.x, playerUntiled.y,
		playerRect.width, playerRect.height);
	playerAbsRect += convertedEvent->GetRectOffset();

	Rect itemAbsRect =
		RectMake(untiled.x, untiled.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	itemAbsRect += _collisionComp->GetOffset();

	playerUntiled.y -= 16;
	float relXDiff = playerUntiled.x - untiled.x;
	float relYDiff = playerUntiled.y - untiled.y;

	if (!_fired)
	{
		if (_facingDirection == Direction::Left)
		{
			//플레이어가 왼쪽에 있을때 
			if ((relXDiff < 0) && (abs(relYDiff) < 16))
			{
				Fire();
			}
		}
		else
		{
			if ((relXDiff > 0) && (abs(relYDiff) < 16))
			{
				Fire();
			}
		}
	}

	Rect overlapRect;
	if (IsRectangleOverlap(playerAbsRect, itemAbsRect, overlapRect))
	{
		if (overlapRect.width > overlapRect.height)
		{
			if (relYDiff > 0)
			{
				pPlayer->position.AddToTileRelY(overlapRect.height);
				pPlayer->desiredPosition = pPlayer->position;
				pPlayer->SetVelocityY(0);
				pPlayer->SetHeadHit(true);
			}
			else
			{
				pPlayer->position.AddToTileRelY(-overlapRect.height);
				pPlayer->desiredPosition = pPlayer->position;
				pPlayer->SetOnObject(true);
				pPlayer->SetOnGround(true);
			}
		}
		else
		{
			if (relXDiff > 0)
			{
				pPlayer->position.AddToTileRelX(overlapRect.width);
				pPlayer->desiredPosition = pPlayer->position;
				pPlayer->SetVelocityX(0);
			}
			else
			{
				pPlayer->position.AddToTileRelX(-overlapRect.width);
				pPlayer->desiredPosition = pPlayer->position;
				pPlayer->SetVelocityX(0);
			}
		}
	}
}

void ArrowRock::HandleObstaclePositionEvent(const IEvent * event)
{
	ObstaclePositionEvent *convertedEvent = (ObstaclePositionEvent *)(event);
	if (convertedEvent->GetId() == _id)
	{
		return;
	}
	const TilePosition &otherTilePosition = convertedEvent->GetPosition();
	int tileXDiff = otherTilePosition.tileX - position.tileX;
	int tileYDiff = otherTilePosition.tileY - position.tileY;

	if (abs(tileXDiff) >= 2 || abs(tileYDiff) >= 2)
	{
		return;
	}

	Obstacle *pObstacle = (Obstacle *)OBJECTMANAGER->FindObjectId(convertedEvent->GetId());

	const Vector2 &otherUntiled = otherTilePosition.UnTilelize();
	const Vector2 untiled = position.UnTilelize();
	const Rect &otherRect = convertedEvent->GetRect();

	Rect otherAbsRect = RectMake(otherUntiled.x, otherUntiled.y,
		otherRect.width, otherRect.height);
	otherAbsRect += convertedEvent->GetRectOffset();

	Rect itemAbsRect =
		RectMake(untiled.x, untiled.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	itemAbsRect += _collisionComp->GetOffset();

	float relXDiff = otherUntiled.x - untiled.x;
	float relYDiff = otherUntiled.y - untiled.y;

	Rect overlapRect;
	if (IsRectangleOverlap(otherAbsRect, itemAbsRect, overlapRect))
	{
		if (overlapRect.width > overlapRect.height)
		{
			if (relYDiff > 0)
			{
				//pObstacle->SetHeadHit(true);
			}
			else
			{
				pObstacle->position.AddToTileRelY(-overlapRect.height);
				pObstacle->desiredPosition = pObstacle->position;
				pObstacle->SetOnObject(true);
			}
		}
		else
		{
			if (relXDiff > 0)
			{
				position.AddToTileRelX(-overlapRect.width / 2);
				desiredPosition = position;
				pObstacle->position.AddToTileRelX(overlapRect.width / 2);
				pObstacle->desiredPosition = pObstacle->position;
				pObstacle->SetVelocity(Vector2(0, 0));
			}
			else
			{
				position.AddToTileRelX(overlapRect.width / 2);
				desiredPosition = position;
				pObstacle->position.AddToTileRelX(-overlapRect.width / 2);
				pObstacle->desiredPosition = pObstacle->position;
				pObstacle->SetVelocity(Vector2(0, 0));
			}
		}
	}
}

void ArrowRock::HandleEnemyPositionEvent(const IEvent * event)
{
	EnemyPositionEvent *convertedEvent = (EnemyPositionEvent *)(event);
	const TilePosition &enemyTilePosition = convertedEvent->GetPosition();
	int tileXDiff = enemyTilePosition.tileX - position.tileX;
	int tileYDiff = enemyTilePosition.tileY - position.tileY;

	if (abs(tileXDiff) >= 6 || abs(tileYDiff) >= 3)
	{
		return;
	}

	Vector2 &enemyUntiled = enemyTilePosition.UnTilelize();
	const Vector2 untiled = position.UnTilelize();
	const Rect &enemyRect = convertedEvent->GetRect();

	enemyUntiled.y -= 16;
	float relXDiff = enemyUntiled.x - untiled.x;
	float relYDiff = enemyUntiled.y - untiled.y;

	if (!_fired)
	{
		if (_facingDirection == Direction::Left)
		{
			//플레이어가 왼쪽에 있을때 
			if ((relXDiff < 0) && (abs(relYDiff) < 16))
			{
				Fire();
			}
		}
		else
		{
			if ((relXDiff > 0) && (abs(relYDiff) < 16))
			{
				Fire();
			}
		}
	}
}

void ArrowRock::Fire()
{
	_fired = true;
	EVENTMANAGER->QueueEvent(new FireArrowEvent(_id, position, _facingDirection));
	SOUNDMANAGER->Play(L"arrow_shot");
}
