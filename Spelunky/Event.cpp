#include "stdafx.h"
#include "Event.h"

EventType CreateEntityEvent::_type = EVENT_CREATE_ENTITY;
EventType NotifyAfterCreateEvent::_type = EVENT_NOTIFY_AFTER_CREATE;
EventType DestroyObjectEvent::_type = EVENT_DESTROY_OBJECT;
EventType DestroyAllObjectEvent::_type = EVENT_DESTROY_ALL_OBJECT;
EventType PlayerInputEvent::_type = EVENT_PLAYER_INPUT;
EventType PlayerPositionEvent::_type = EVENT_PLAYER_POSITION;

EventType FrameEndedEvent::_type = EVENT_FRAME_ENDED;
EventType CollectMoneyEvent::_type = EVENT_COLLECT_MONEY;
EventType ObjectDeadEvent::_type = EVENT_OBJECT_DEAD;


//EventType CollisionEvent::_type = EVENT_COLLISION;

BaseEvent::BaseEvent(float timeStamp)
	:_timeStamp(timeStamp)
{
}

BaseEvent::~BaseEvent()
{
}

#pragma region Entity Create And Destroy Related
CreateEntityEvent::CreateEntityEvent(ArcheType objectType)
	:_objectType(objectType)
{
}

CreateEntityEvent::~CreateEntityEvent()
{
}

IEvent * CreateEntityEvent::Copy() const
{
	return new CreateEntityEvent(_objectType);
}

const WCHAR * CreateEntityEvent::GetName() const
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

PlayerPositionEvent::PlayerPositionEvent(ObjectId id, const TilePosition & position, const Rect & rect, const Vector2 & rectOffset)
	:_id(id), _position(position), _rect(rect), _rectOffset(rectOffset)
{

}

PlayerPositionEvent::~PlayerPositionEvent()
{
}

IEvent * PlayerPositionEvent::Copy() const
{
	return new PlayerPositionEvent(_id, _position, _rect, _rectOffset);
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
