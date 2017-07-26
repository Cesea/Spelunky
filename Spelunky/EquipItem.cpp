#include "stdafx.h"
#include "EquipItem.h"

#include "CollisionComponent.h"

EquipItem::EquipItem(ObjectId id)
	:Item(id)
{
}

EquipItem::~EquipItem()
{
	Item::~Item();
}

HRESULT EquipItem::Init(BaseProperty *property)
{
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<EquipItem, &EquipItem::HandlePlayerInputEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PICK_UP, EventDelegate::FromFunction<EquipItem, &EquipItem::HandlePickupEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PUT_DOWN, EventDelegate::FromFunction<EquipItem, &EquipItem::HandlePutDownEvent>(this));
	_collisionComp = new CollisionComponent();
	_collisionComp->Init(RectMake(0, 0, 40, 30), Vector2(-20, -24));

	_equiped = 0;

	return S_OK;
}

void EquipItem::Release(void)
{
	SAFE_DELETE(_collisionComp);
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<EquipItem, &EquipItem::HandlePlayerInputEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PUT_DOWN, EventDelegate::FromFunction<EquipItem, &EquipItem::HandlePutDownEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PICK_UP, EventDelegate::FromFunction<EquipItem, &EquipItem::HandlePickupEvent>(this));
}

void EquipItem::Update(float deltaTime)
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

void EquipItem::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->Render(renderTarget, drawPos.x, drawPos.y);
}

void EquipItem::Use(const ControlCommand &commands)
{
	_equiped = false;
	_pOwner = nullptr;
}

void EquipItem::PutDown(Direction direction)
{
	_pOwner = nullptr;
	_equiped = false;
	if (direction == Direction::Left)
	{
		_velocity.x = -140;
	}
	else if (direction == Direction::Right)
	{
		_velocity.x = 140;
	}
}

GameObject * EquipItem::Copy(ObjectId id)
{
	return nullptr;
}

void EquipItem::HandlePlayerInputEvent(const IEvent * event)
{
	if (_equiped)
	{
		PlayerInputEvent *convertedEvent = (PlayerInputEvent *)(event);
		const ControlCommand &commands = convertedEvent->GetControlCommand();

		if (_pOwner)
		{
			if (commands.action == Command::Attack && !_pOwner->GetCrawling())
			{
				_collisionComp->SetRepulse(true);
				Use(commands);
			}
		}
	}
}

void EquipItem::HandlePickupEvent(const IEvent * event)
{
	PickupEvent *convertedEvent = (PickupEvent *)(event);
	if (_onActorId == convertedEvent->GetId())
	{
		_pOwner = (MovingObject *)OBJECTMANAGER->FindObjectId(_onActorId);
		if (!_pOwner->GetHolding())
		{
			_equiped = true;
			_pOwner->SetHolding(true);
			EVENTMANAGER->QueueEvent(new HoldingEvent(_id, _onActorId, _equipSlot));
		}
		else
		{
			_pOwner = nullptr;
		}
	}
}

void EquipItem::HandlePutDownEvent(const IEvent * event)
{
	PutDownEvent *convertedEvent = (PutDownEvent *)(event);
	if (_id == convertedEvent->GetId())
	{
		_collisionComp->SetRepulse(true);
		PutDown(convertedEvent->GetDirection());
	}
}

void EquipItem::Apply(ObjectId id)
{
}
