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
	position.AddToTileRel(32, 44);
	desiredPosition = position;

	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<PushingRock, &PushingRock::HandlePlayerPositionEvent>(this));

	return S_OK;
}

void PushingRock::Release(void)
{
	SAFE_RELEASE_AND_DELETE(_sprite);
	SAFE_DELETE(_collisionComp);

	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<PushingRock, &PushingRock::HandlePlayerPositionEvent>(this));
}

void PushingRock::Update(float deltaTime)
{
	_accel.y += GRAVITY;
	_velocity += _accel * deltaTime;

	_accel = Vector2();
	desiredPosition.AddToTileRel(_velocity * deltaTime);

	TilePosition centerPos = desiredPosition;
	centerPos.AddToTileRelY(-16.0f);
	_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));
	_collisionComp->Update(this, deltaTime, &_nearTiles);
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
				EVENTMANAGER->QueueEvent(new PushingObjectEvent(_id));
			}
			else
			{
				pPlayer->position.AddToTileRelX(-overlapRect.width / 2);
				pPlayer->desiredPosition = pPlayer->position;

				position.AddToTileRelX(overlapRect.width / 2);
				desiredPosition = position;

				pPlayer->SetVelocityX(pPlayer->GetVelocity().x * 0.9f);
				EVENTMANAGER->QueueEvent(new PushingObjectEvent(_id));
			}
		}
	}
}

void PushingRock::HandlePlayerAttackEvent(const IEvent * event)
{
}
