#include "stdafx.h"
#include "Event.h"

EventType CreateEntityEvent::_type = EVENT_CREATE_ENTITY;
EventType NotifyAfterCreateEvent::_type = EVENT_NOTIFY_AFTER_CREATE;
EventType DestroyEntityEvent::_type = EVENT_DESTROY_ENTITY;
EventType DestroyAllEntityEvent::_type = EVENT_DESTROY_ALL_ENTITY;
EventType PlayerInputEvent::_type = EVENT_PLAYER_INPUT;
EventType PlayerPositionEvent::_type = EVENT_PLAYER_POSITION;

EventType FrameEndedEvent::_type = EVENT_FRAME_ENDED;

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

DestroyEntityEvent::DestroyEntityEvent(ObjectId id)
	:_id(id)
{
}

DestroyEntityEvent::~DestroyEntityEvent()
{
}

IEvent * DestroyEntityEvent::Copy() const
{
	return new DestroyEntityEvent(_id);
}

const WCHAR * DestroyEntityEvent::GetName() const
{
	return L"Destroy Entity Event";
}

DestroyAllEntityEvent::DestroyAllEntityEvent()
{
}

DestroyAllEntityEvent::~DestroyAllEntityEvent()
{
}

IEvent * DestroyAllEntityEvent::Copy() const
{
	return new DestroyAllEntityEvent;
}

const WCHAR * DestroyAllEntityEvent::GetName() const
{
	return L"Destroy All Entity Event";
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
