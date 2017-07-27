#include "stdafx.h"
#include "Eatables.h"

#include "CollisionComponent.h"

Eatables::Eatables(ObjectId id)
	:PassiveItem::PassiveItem(id)
{
}

Eatables::~Eatables()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Item, &Eatables::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<Eatables, &Eatables::HandlePlayerAttackEvent>(this));
}

HRESULT Eatables::Init(BaseProperty * property)
{
	PassiveItem::Init(property);
	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"eatables"), 80, 80, IntVector2(-40, -40));

	EatableProperty *convertedProperty = (EatableProperty *)(property);
	*this = convertedProperty;

	position.tileRel = Vector2(32, 32);
	desiredPosition = position;

	_collisionComp = new CollisionComponent;
	_collisionComp->Init(RectMake(0, 0, 40, 40), Vector2(-20, -20));

	return S_OK;
}

void Eatables::Release(void)
{
	SAFE_RELEASE_AND_DELETE(_sprite);
	SAFE_DELETE(_collisionComp);
}

void Eatables::Update(float deltaTime)
{
	_accel.y += GRAVITY;
	_velocity += _accel * deltaTime;

	_accel = Vector2();
	desiredPosition.AddToTileRel(_velocity * deltaTime);

	TilePosition centerPos = desiredPosition;
	centerPos.AddToTileRelY(-32.0f);
	_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));
	_collisionComp->Update(this, deltaTime, &_nearTiles);

	if (_actorOn)
	{
		Apply(_onActorId);
	}
}

void Eatables::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position.UnTilelize() - camPos;
	_sprite->FrameRender(renderTarget, drawingPos.x, drawingPos.y, _sourceIndex.x, _sourceIndex.y);
}

GameObject * Eatables::Copy(ObjectId id)
{
	return nullptr;
}

void Eatables::Apply(ObjectId id)
{
	if (_eatableType == EATABLE_Bomb)
	{
		playerBombEatFunction(3);
	}
	else if(_eatableType == EATABLE_Rope)
	{
		playerRopeEatFunction(3);
	}
	EVENTMANAGER->QueueEvent(new CollectEatableEvent(_id));
	SOUNDMANAGER->Play(L"collect");
}


void Eatables::HandlePlayerAttackEvent(const IEvent * event)
{
	PlayerAttackEvent *convertedEvent = (PlayerAttackEvent *)event;
	TilePosition playerPos = convertedEvent->GetTilePosition();

	int tileXDiff = playerPos.tileX - position.tileX;
	int tileYDiff = playerPos.tileY - position.tileY;

	if (abs(tileXDiff) >= 3 || abs(tileYDiff) >= 3)
	{
		return;
	}

	Direction seeingDirection = convertedEvent->GetDirection();

	Vector2 positionUntiled = position.UnTilelize();
	const TilePosition & playerPosition = convertedEvent->GetTilePosition();
	Vector2 playerPositionUntiled = playerPosition.UnTilelize();

	bool hitted = false;
	if (seeingDirection == Direction::Left)
	{
		if (positionUntiled.x <= playerPositionUntiled.x + 10 &&
			positionUntiled.x >= playerPositionUntiled.x - 70 &&
			position.tileY == playerPosition.tileY)
		{
			hitted = true;
		}
	}
	else if (seeingDirection == Direction::Right)
	{
		if (positionUntiled.x >= playerPositionUntiled.x - 10 &&
			positionUntiled.x <= playerPositionUntiled.x + 84 &&
			position.tileY == playerPosition.tileY)
		{

			hitted = true;
		}
	}
	if (hitted)
	{
		Console::Log("Gem hitted\n");
		_collisionComp->SetRepulse(true);
		if (seeingDirection == Direction::Left)
		{
			_velocity.x = -140;
		}
		else if (seeingDirection == Direction::Right)
		{
			_velocity.x = 140;
		}
		_velocity.y -= 84;
	}
}

void Eatables::operator=(EatableProperty * property)
{
	_sourceIndex = property->sourceIndex;
	_eatableType = property->type;
}
