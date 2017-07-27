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
	_sourceIndex = IntVector2(2, 0);
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

	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<ArrowRock, &ArrowRock::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<ArrowRock, &ArrowRock::HandlePlayerAttackEvent>(this));

	return S_OK;
}

void ArrowRock::Release(void)
{
	SAFE_RELEASE_AND_DELETE(_sprite);
	SAFE_DELETE(_collisionComp);

	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<ArrowRock, &ArrowRock::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<ArrowRock, &ArrowRock::HandlePlayerAttackEvent>(this));
}

void ArrowRock::Update(float deltaTime)
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

void ArrowRock::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
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
				pPlayer->SetHeadHit(true);
			}
			else
			{
				pPlayer->position.AddToTileRelY(-overlapRect.height);
				pPlayer->desiredPosition = pPlayer->position;
				pPlayer->SetOnGround(true);
			}
		}
		else
		{
			if (relXDiff > 0)
			{
				pPlayer->position.AddToTileRelX(overlapRect.width);
				pPlayer->desiredPosition = pPlayer->position;
			}
			else
			{
				pPlayer->position.AddToTileRelX(-overlapRect.width);
				pPlayer->desiredPosition = pPlayer->position;
			}
		}
	}
}

void ArrowRock::HandlePlayerAttackEvent(const IEvent * event)
{
}
