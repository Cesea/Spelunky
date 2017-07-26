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
	_collisionComp = new CollisionComponent();
	_collisionComp->Init(RectMake(0, 0, 40, 30), Vector2(-20, -24));

	CrateProperty *convertedProperty = (CrateProperty *)(property);
	*this = convertedProperty;

	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"crates"), 80, 80, IntVector2(-40, -40));
	return S_OK;
}

void Crate::Release(void)
{
	SAFE_DELETE(_collisionComp);
	SAFE_DELETE(_sprite);
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<EquipItem, &Crate::HandlePlayerInputEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PUT_DOWN, EventDelegate::FromFunction<EquipItem, &Crate::HandlePutDownEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PICK_UP, EventDelegate::FromFunction<EquipItem, &Crate::HandlePickupEvent>(this));
}

void Crate::Update(float deltaTime)
{
	if (_equiped)
	{
		desiredPosition = _pOwner->desiredPosition;
		desiredPosition.AddToTileRelY(50);
		position = desiredPosition;
	}
	else
	{
		_accel.y += GRAVITY;
		_velocity += _accel * deltaTime;

		_accel = Vector2();
		desiredPosition.AddToTileRel(_velocity * deltaTime);

		TilePosition centerPos = desiredPosition;
		centerPos.AddToTileRelY(5.0f);
		_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));
		_collisionComp->Update(this, deltaTime, &_nearTiles);
	}
}

void Crate::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->Render(renderTarget, drawPos.x, drawPos.y);
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
		SpawnItem();
		EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_Crate));
		//Console::Log("Crate hitted\n");
		//_collisionComp->SetRepulse(true);
		//if (seeingDirection == Direction::Left)
		//{
		//	_velocity.x = -140;
		//}
		//else if (seeingDirection == Direction::Right)
		//{
		//	_velocity.x = 140;
		//}
		//_velocity.y -= 84;
	}

}

void Crate::HandleDamageEvent(const IEvent * event)
{
}

void Crate::SpawnItem()
{
}

void Crate::operator=(const CrateProperty *property)
{
	_sourceIndex = property->sourceIndex;
	_crateType = property->type;
	_breakType = BreakType::BREAK_Crate;
	position.tileX = property->position.x;
	position.tileY = property->position.y;
}
