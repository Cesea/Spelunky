#include "stdafx.h"
#include "Event.h"

EventType CreateObjectEvent::_type = EVENT_CREATE_OBJECT;
EventType NotifyAfterCreateEvent::_type = EVENT_NOTIFY_AFTER_CREATE;
EventType DestroyObjectEvent::_type = EVENT_DESTROY_OBJECT;
EventType DestroyAllObjectEvent::_type = EVENT_DESTROY_ALL_OBJECT;
EventType PlayerInputEvent::_type = EVENT_PLAYER_INPUT;
EventType PlayerPositionEvent::_type = EVENT_PLAYER_POSITION;

EventType FrameEndedEvent::_type = EVENT_FRAME_ENDED;
EventType CollectMoneyEvent::_type = EVENT_COLLECT_MONEY;
EventType ObjectDeadEvent::_type = EVENT_OBJECT_DEAD;

EventType PickupEvent::_type = EVENT_PICK_UP;
EventType PutDownEvent::_type = EVENT_PUT_DOWN;
EventType HoldingEvent::_type = EVENT_HOLDING;

EventType StageTransitionEvent::_type = EVENT_STAGE_TRANSITIOIN;

EventType DestroyTileEvent::_type = EVENT_DESTROY_TILE;
EventType OnTunnelEvent::_type = EVENT_ON_TUNNEL;

EventType ItemBreakEvent::_type = EVENT_ITEM_BREAK;
EventType PlayerAttackEvent::_type = EVENT_PLAYER_ATTACK;

EventType PlayerGoExitEvent::_type = EVENT_PLAYER_GO_EXIT;

EventType ThrowBombEvent::_type = EVENT_THROW_BOMB;
EventType EnemyInputEvent::_type = EVENT_ENEMY_INPUT;

EventType EnemyDeadEvent::_type = EVENT_ENEMY_DEAD;
EventType PlayerUpperJumpEvent::_type = EVENT_PLAYER_UPPER_JUMP;


BaseEvent::BaseEvent(float timeStamp)
	:_timeStamp(timeStamp)
{
}

BaseEvent::~BaseEvent()
{
}

#pragma region Entity Create And Destroy Related
CreateObjectEvent::CreateObjectEvent(const std::wstring &key, BaseProperty *property)
	:_key(key), _property(property)
{
}

CreateObjectEvent::~CreateObjectEvent()
{
	delete _property;
}

IEvent * CreateObjectEvent::Copy() const
{
	return new CreateObjectEvent(_key, _property);
}

const WCHAR * CreateObjectEvent::GetName() const
{
	return L"Create Entity Event";
}

DestroyObjectEvent::DestroyObjectEvent(ObjectId id)
	:_id(id)
{
}

DestroyObjectEvent::~DestroyObjectEvent()
{
}

IEvent * DestroyObjectEvent::Copy() const
{
	return new DestroyObjectEvent(_id);
}

const WCHAR * DestroyObjectEvent::GetName() const
{
	return L"Destroy Object Event";
}

DestroyAllObjectEvent::DestroyAllObjectEvent()
{
}

DestroyAllObjectEvent::~DestroyAllObjectEvent()
{
}

IEvent * DestroyAllObjectEvent::Copy() const
{
	return new DestroyAllObjectEvent;
}

const WCHAR * DestroyAllObjectEvent::GetName() const
{
	return L"Destroy All Object Event";
}

NotifyAfterCreateEvent::NotifyAfterCreateEvent(ObjectId id)
	:_id(id)
{
}

NotifyAfterCreateEvent::~NotifyAfterCreateEvent()
{
}

IEvent * NotifyAfterCreateEvent::Copy() const
{
	return new NotifyAfterCreateEvent(_id);
}

const WCHAR * NotifyAfterCreateEvent::GetName() const
{
	return L"Notify After Create Event";
}
#pragma endregion

#pragma region Input Related
PlayerInputEvent::PlayerInputEvent(ObjectId id, const ControlCommand & controlCommand)
	:_id(id), _controlCommand(controlCommand)
{
}

PlayerInputEvent::~PlayerInputEvent()
{
}

IEvent * PlayerInputEvent::Copy() const
{
	return new PlayerInputEvent(_id, _controlCommand);
}

const WCHAR * PlayerInputEvent::GetName() const
{
	return L"Control Entity Event";
}

const ControlCommand & PlayerInputEvent::GetControlCommand()
{
	return _controlCommand;
}
#pragma endregion


FrameEndedEvent::FrameEndedEvent(ObjectId id)
	:_id(id)
{
}

FrameEndedEvent::~FrameEndedEvent()
{
}

IEvent * FrameEndedEvent::Copy() const
{
	return new FrameEndedEvent(_id);
}

const WCHAR * FrameEndedEvent::GetName() const
{
	return L"Frame Ended Event";
}

PlayerPositionEvent::PlayerPositionEvent(ObjectId id, const TilePosition &position, 
		const Rect &rect, const Vector2 &rectOffset, const bool32 isFalling)
	:_id(id), _position(position), _rect(rect), _rectOffset(rectOffset), _isFalling(isFalling)
{

}

PlayerPositionEvent::~PlayerPositionEvent()
{
}

IEvent * PlayerPositionEvent::Copy() const
{
	return new PlayerPositionEvent(_id, _position, _rect, _rectOffset, _isFalling);
}

const WCHAR * PlayerPositionEvent::GetName() const
{
	return L"Player Position";
}

CollectMoneyEvent::CollectMoneyEvent(ObjectId id, ObjectId targetId, int value)
	:_id(id), _targetId(targetId), _value(value)
{
}

CollectMoneyEvent::~CollectMoneyEvent()
{
}

IEvent * CollectMoneyEvent::Copy() const
{
	return new CollectMoneyEvent(_id, _targetId, _value);
}

const WCHAR * CollectMoneyEvent::GetName() const
{
	return L"Collect Money Event";
}

ObjectDeadEvent::ObjectDeadEvent(ObjectId id, bool32 fromAbove)
	:_id(id), _fromAbove(fromAbove)
{
}

ObjectDeadEvent::~ObjectDeadEvent()
{
}

IEvent * ObjectDeadEvent::Copy() const
{
	return new ObjectDeadEvent(_id, _fromAbove);
}

const WCHAR * ObjectDeadEvent::GetName() const
{
	return L"Object Dead Event";
}

PickupEvent::PickupEvent(ObjectId id)
	:_id(id)
{
}

PickupEvent::~PickupEvent()
{
}

IEvent * PickupEvent::Copy() const
{
	return new PickupEvent(_id);
}

const WCHAR * PickupEvent::GetName() const
{
	return L"Pickup Event";
}

HoldingEvent::HoldingEvent(ObjectId id, ObjectId ownerId, EquipSlot slot)
	:_id(id), _ownerId(ownerId), _slot(slot)
{
}

HoldingEvent::~HoldingEvent()
{
}

IEvent * HoldingEvent::Copy() const
{
	return new HoldingEvent(_id, _ownerId, _slot);
}

const WCHAR * HoldingEvent::GetName() const
{
	return L"Holding Event";
}

StageTransitionEvent::StageTransitionEvent()
{
}

StageTransitionEvent::~StageTransitionEvent()
{
}

IEvent * StageTransitionEvent::Copy() const
{
	return new StageTransitionEvent();
}

const WCHAR * StageTransitionEvent::GetName() const
{
	return L"Stage Transition Event";
}

DestroyTileEvent::DestroyTileEvent(const IntVector2 &position)
	:_position(position)
{
}

DestroyTileEvent::~DestroyTileEvent()
{
}

IEvent * DestroyTileEvent::Copy() const
{
	return new DestroyTileEvent(_position);
}

const WCHAR * DestroyTileEvent::GetName() const
{
	return L"Destroy Tile Event";
}

OnTunnelEvent::OnTunnelEvent(const IntVector2 & position)
	:_position(position)
{
}

IEvent * OnTunnelEvent::Copy() const
{
	return new OnTunnelEvent(_position);
}

const WCHAR * OnTunnelEvent::GetName() const
{
	return L"On Tunnel Event";
}

ItemBreakEvent::ItemBreakEvent(const ObjectId id, BreakType type)
	:_id(id), _breakType(type)
{
}

IEvent * ItemBreakEvent::Copy() const
{
	return new ItemBreakEvent(_id,_breakType);
}

const WCHAR * ItemBreakEvent::GetName() const
{
	return L"Item Break Event";
}

PutDownEvent::PutDownEvent(const ObjectId id, Direction direction)
	:_id(id), _direction(direction)
{
}

IEvent * PutDownEvent::Copy() const
{
	return new PutDownEvent(_id, _direction);
}

const WCHAR * PutDownEvent::GetName() const
{
	return L"Put Down Event";
}

PlayerAttackEvent::PlayerAttackEvent(Direction direction, TilePosition position)
	:_position(position), _direction(direction)
{
}

IEvent * PlayerAttackEvent::Copy() const
{
	return new PlayerAttackEvent(_direction, _position);
}

const WCHAR * PlayerAttackEvent::GetName() const
{
	return L"Player Attack Event";
}

PlayerGoExitEvent::PlayerGoExitEvent(bool isMiddle)
	:_isMiddle(isMiddle)
{
}

PlayerGoExitEvent::~PlayerGoExitEvent()
{
}

IEvent * PlayerGoExitEvent::Copy() const
{
	return new PlayerGoExitEvent(_isMiddle);
}

const WCHAR * PlayerGoExitEvent::GetName() const
{
	return L"Player Go Exit Event";
}

ThrowBombEvent::ThrowBombEvent(const TilePosition & throwPosition, const Vector2 initialVelocity, bool isSticky)
	:_throwPosition(throwPosition), _initVel(initialVelocity), _isSticky(isSticky)
{
}

ThrowBombEvent::~ThrowBombEvent()
{
}

IEvent * ThrowBombEvent::Copy() const
{
	return new ThrowBombEvent(_throwPosition, _initVel, _isSticky);
}

const WCHAR * ThrowBombEvent::GetName() const
{
	return L"Throw Bomb Event";
}

EnemyInputEvent::EnemyInputEvent(ObjectId id, const ControlCommand & controlCommand)
	:_id(id), _controlCommand(controlCommand)
{
}

EnemyInputEvent::~EnemyInputEvent()
{
}

IEvent * EnemyInputEvent::Copy() const
{
	return new EnemyInputEvent(_id, _controlCommand);
}

const WCHAR * EnemyInputEvent::GetName() const
{
	return L"Enemy Input Event";
}

EnemyDeadEvent::EnemyDeadEvent(const ObjectId id)
	:_id(id)
{
}

IEvent * EnemyDeadEvent::Copy() const
{
	return new EnemyDeadEvent(_id);
}

const WCHAR * EnemyDeadEvent::GetName() const
{
	return L"Enemy Dead Event";
}

PlayerUpperJumpEvent::PlayerUpperJumpEvent(const Vector2 & otherVelocity)
	:_otherVelocity(otherVelocity)
{
}

IEvent * PlayerUpperJumpEvent::Copy() const
{
	return new PlayerUpperJumpEvent(_otherVelocity);
}

const WCHAR * PlayerUpperJumpEvent::GetName() const
{
	return L"Player Upper Jump Event";
}
