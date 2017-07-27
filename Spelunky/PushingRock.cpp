#include "stdafx.h"
#include "PushingRock.h"

#include "CollisionComponent.h"

PushingRock::PushingRock(ObjectId id)
	:Obstacle(id)
{
}

PushingRock::~PushingRock()
{
}

HRESULT PushingRock::Init(BaseProperty * property)
{
	_sourceIndex = IntVector2(0, 0);
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

	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<PushingRock, &PushingRock::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_OBSTACLE_POSITION, EventDelegate::FromFunction<PushingRock, &PushingRock::HandleObstaclePositionEvent>(this));

	return S_OK;
}

void PushingRock::Release(void)
{
	SAFE_RELEASE_AND_DELETE(_sprite);
	SAFE_DELETE(_collisionComp);

	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<PushingRock, &PushingRock::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_OBSTACLE_POSITION, EventDelegate::FromFunction<PushingRock, &PushingRock::HandleObstaclePositionEvent>(this));
}

void PushingRock::Update(float deltaTime)
{
	_accel.y += GRAVITY;
	if (_onObject)
	{
		_accel.y -= GRAVITY;
		_velocity.y = 0;
	}
	_velocity += _accel * deltaTime;
	_onObject = false;

	_accel = Vector2();
	desiredPosition.AddToTileRel(_velocity * deltaTime);

	TilePosition centerPos = desiredPosition;
	centerPos.AddToTileRelY(-16.0f);
	_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));
	_collisionComp->Update(this, deltaTime, &_nearTiles);
	EVENTMANAGER->QueueEvent(new ObstaclePositionEvent(_id, position, _collisionComp->GetRect(), _collisionComp->GetOffset()));
}

void PushingRock::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->FrameRender(renderTarget, drawPos.x, drawPos.y, _sourceIndex.x, _sourceIndex.y);

	const Vector2 itemUntiledPosition = position.UnTilelize();
	Rect itemAbsRect =
		RectMake(itemUntiledPosition.x, itemUntiledPosition.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	itemAbsRect += _collisionComp->GetOffset();

	DrawBox(renderTarget, itemAbsRect.x - camPos.x, itemAbsRect.y - camPos.y, itemAbsRect.width, itemAbsRect.height, D2D1::ColorF(1.0f, 0.0, 0.0, 1.0f));

	Vector2 pos = position.UnTilelize();

	DrawBox(renderTarget, pos.x - camPos.x, pos.y - camPos.y, 5, 5, D2D1::ColorF(1.0f, 1.0, 0.0, 1.0f));
}

GameObject * PushingRock::Copy(ObjectId id)
{
	return nullptr;
}

void PushingRock::HandlePlayerPositionEvent(const IEvent * event)
{
	PlayerPositionEvent *convertedEvent = (PlayerPositionEvent *)(event);
	const TilePosition &playerTilePosition = convertedEvent->GetPosition();
	int tileXDiff = playerTilePosition.tileX - position.tileX;
	int tileYDiff = playerTilePosition.tileY - position.tileY;

	if (abs(tileXDiff) >= 3 || abs(tileYDiff) >= 3)
	{
		_prevXOverlapAmount = 0;
		return;
	}

	Player *pPlayer = (Player *)OBJECTMANAGER->FindObjectId(convertedEvent->GetId());

	const Vector2 &playerUntiled = playerTilePosition.UnTilelize();
	const Vector2 untiled = position.UnTilelize();
	const Rect &playerRect = convertedEvent->GetRect();

	Rect playerAbsRect = RectMake(playerUntiled.x, playerUntiled.y,
		playerRect.width, playerRect.height);
	playerAbsRect += convertedEvent->GetRectOffset();

	Rect itemAbsRect =
		RectMake(untiled.x, untiled.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	itemAbsRect += _collisionComp->GetOffset();

	float relXDiff = playerUntiled.x - untiled.x;
	float relYDiff = playerUntiled.y - untiled.y;

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
				pPlayer->position.AddToTileRelX(overlapRect.width / 2);
				pPlayer->desiredPosition = pPlayer->position;
				position.AddToTileRelX(-overlapRect.width / 2);
				desiredPosition = position;
				pPlayer->SetVelocityX(pPlayer->GetVelocity().x * 0.9f);
				if (overlapRect.width >= _prevXOverlapAmount)
				{
					EVENTMANAGER->QueueEvent(new PushingObjectEvent(_id));
				}
			}
			else
			{
				pPlayer->position.AddToTileRelX(-overlapRect.width / 2);
				pPlayer->desiredPosition = pPlayer->position;

				position.AddToTileRelX(overlapRect.width / 2);
				desiredPosition = position;
				pPlayer->SetVelocityX(pPlayer->GetVelocity().x * 0.9f);
				if (overlapRect.width >= _prevXOverlapAmount)
				{
					EVENTMANAGER->QueueEvent(new PushingObjectEvent(_id));
				}
			}
			_prevXOverlapAmount = overlapRect.width;
		}
	}
	else
	{
		_prevXOverlapAmount = 0;
	}
}


void PushingRock::HandleObstaclePositionEvent(const IEvent * event)
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
				pObstacle->position.AddToTileRelX(overlapRect.width/2);
				pObstacle->desiredPosition = pObstacle->position;
				pObstacle->SetVelocity(Vector2(0, 0));
			}
			else
			{
				position.AddToTileRelX(overlapRect.width / 2);
				desiredPosition = position;
				pObstacle->position.AddToTileRelX(-overlapRect.width/2);
				pObstacle->desiredPosition = pObstacle->position;
				pObstacle->SetVelocity(Vector2(0, 0));
			}
		}
	}
}
