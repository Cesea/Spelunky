#include "stdafx.h"
#include "Crate.h"

#include "CollisionComponent.h"

Crate::Crate(ObjectId id)
	:EquipItem(id)
{
}

Crate::~Crate()
{
	Item::~Item();
}

HRESULT Crate::Init(BaseProperty *property)
{
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<EquipItem, &Crate::HandlePlayerInputEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PICK_UP, EventDelegate::FromFunction<EquipItem, &Crate::HandlePickupEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PUT_DOWN, EventDelegate::FromFunction<EquipItem, &Crate::HandlePutDownEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Crate, &Crate::HandleDamageEvent>(this));
	_collisionComp = new CollisionComponent();
	_collisionComp->Init(RectMake(0, 0, 40, 40), Vector2(-20, -20));

	CrateProperty *convertedProperty = (CrateProperty *)(property);
	*this = convertedProperty;

	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"crates"), 80, 80, IntVector2(-40, -40));
	_equiped = false;
	return S_OK;
}

void Crate::Release(void)
{
	SAFE_DELETE(_collisionComp);
	SAFE_DELETE(_sprite);
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<EquipItem, &Crate::HandlePlayerInputEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PUT_DOWN, EventDelegate::FromFunction<EquipItem, &Crate::HandlePutDownEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PICK_UP, EventDelegate::FromFunction<EquipItem, &Crate::HandlePickupEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Crate, &Crate::HandleDamageEvent>(this));
}

void Crate::Update(float deltaTime)
{
	if (_equiped)
	{
		desiredPosition = _pOwner->desiredPosition;
		desiredPosition.AddToTileRelY(-12);
		position = desiredPosition;
	}
	else
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
}

void Crate::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->FrameRender(renderTarget, drawPos.x, drawPos.y, _sourceIndex.x, _sourceIndex.y);

	const Vector2 itemUntiledPosition = position.UnTilelize();
	Rect itemAbsRect =
		RectMake(itemUntiledPosition.x, itemUntiledPosition.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	itemAbsRect += _collisionComp->GetOffset();
	DrawBox(gRenderTarget, itemAbsRect.x - camPos.x, itemAbsRect.y - camPos.y, itemAbsRect.width, itemAbsRect.height, D2D1::ColorF(1.0f, 1.0f, 0.0f));
}

void Crate::Use(const ControlCommand &commands)
{
	_equiped = false;
	_pOwner = nullptr;
}

GameObject * Crate::Copy(ObjectId id)
{
	return nullptr;
}

void Crate::Apply(ObjectId id)
{
}

void Crate::HandlePlayerAttackEvent(const IEvent * event)
{
	PlayerAttackEvent *convertedEvent = (PlayerAttackEvent *)event;
	Direction seeingDirection = convertedEvent->GetDirection();

	Vector2 positionUntiled = position.UnTilelize();
	const TilePosition & playerPosition = convertedEvent->GetTilePosition();
	Vector2 playerPositionUntiled = playerPosition.UnTilelize();

	bool hitted = false;
	if (seeingDirection == Direction::Left)
	{
		if (positionUntiled.x <= playerPositionUntiled.x + 10 &&
			positionUntiled.x >= playerPositionUntiled.x - 88 &&
			position.tileY == playerPosition.tileY)
		{
			hitted = true;
		}
	}
	else if (seeingDirection == Direction::Right)
	{
		if (positionUntiled.x >= playerPositionUntiled.x - 10 &&
			positionUntiled.x <= playerPositionUntiled.x + 88 &&
			position.tileY == playerPosition.tileY)
		{

			hitted = true;
		}
	}
	if (hitted)
	{
		SpawnItem();
		EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_Crate));
	}

}

void Crate::HandleDamageEvent(const IEvent * event)
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
		EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_Crate));
	}

}

void Crate::SpawnItem()
{
	if (_crateType == CrateType::CRATE_GemSpawn)
	{
		EVENTMANAGER->QueueEvent(new SpawnObjectEvent(L"gem", position));
	}
	else if (_crateType == CrateType::CRATE_ItemSpawn)
	{
		EVENTMANAGER->QueueEvent(new SpawnObjectEvent(L"eatable", position));
	}
}

void Crate::operator=(const CrateProperty *property)
{
	_sourceIndex = property->sourceIndex;
	_crateType = property->type;
	_breakType = BreakType::BREAK_Crate;
	position.tileX = property->position.x;
	position.tileY = property->position.y;
	position.AddToTileRelY(32);
	desiredPosition = position;
}
