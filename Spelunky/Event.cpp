#include "stdafx.h"
#include "Event.h"

EventType CreateEntityEvent::_type = EVENT_CREATE_ENTITY;
EventType NotifyAfterCreateEvent::_type = EVENT_NOTIFY_AFTER_CREATE;
EventType DestroyEntityEvent::_type = EVENT_DESTROY_ENTITY;
EventType DestroyAllEntityEvent::_type = EVENT_DESTROY_ALL_ENTITY;
EventType PlayerInputEvent::_type = EVENT_PLAYER_INPUT;
EventType ChangeDirectionEvent::_type = EVENT_CHANGE_DIRECTION;

EventType MoveEntityEvent::_type = EVENT_MOVE_ENTITY;
EventType DashEntityEvent::_type = EVENT_DASH_ENTITY;
EventType JumpOffEntityEvent::_type = EVENT_JUMP_OFF_ENTITY;
EventType FallingEvent::_type = EVENT_FALLING;
EventType StopAndPushEntityEvent::_type = EVENT_STOP_AND_PUSH_ENTITY;

EventType ChangedStateEvent::_type = EVENT_CHANGED_STATE;
EventType ShouldChangeStateEvent::_type = EVENT_SHOULD_CHANGE_STATE;

EventType FrameEndedEvent::_type = EVENT_FRAME_ENDED;

EventType CollisionEvent::_type = EVENT_COLLISION;


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

const char * CreateEntityEvent::GetName() const
{
	return "Create Entity Event";
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

const char * DestroyEntityEvent::GetName() const
{
	return "Destroy Entity Event";
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

const char * DestroyAllEntityEvent::GetName() const
{
	return "Destroy All Entity Event";
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

const char * NotifyAfterCreateEvent::GetName() const
{
	return "Notify After Create Event";
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

const char * PlayerInputEvent::GetName() const
{
	return "Control Entity Event";
}

const ControlCommand & PlayerInputEvent::GetControlCommand()
{
	return _controlCommand;
}
#pragma endregion


ChangeDirectionEvent::ChangeDirectionEvent(ObjectId id)
	:_id(id)
{
}

ChangeDirectionEvent::~ChangeDirectionEvent()
{
}

IEvent * ChangeDirectionEvent::Copy() const
{
	return new ChangeDirectionEvent(_id);
}

const char * ChangeDirectionEvent::GetName() const
{
	return "Change Direction Event";
}

MoveEntityEvent::MoveEntityEvent(ObjectId id)
	:_id(id)
{
}

MoveEntityEvent::~MoveEntityEvent()
{
}

IEvent * MoveEntityEvent::Copy() const
{
	return new MoveEntityEvent(_id);
}

const char * MoveEntityEvent::GetName() const
{
	return "Move Entity Event";
}

#pragma region State Related
ChangedStateEvent::ChangedStateEvent(ObjectId id, EntityState state)
	:_id(id), _state(state)
{
}

ChangedStateEvent::~ChangedStateEvent()
{
}

IEvent * ChangedStateEvent::Copy() const
{
	return new ChangedStateEvent(_id, _state);
}

const char * ChangedStateEvent::GetName() const
{
	return "Change State Event";
}


ShouldChangeStateEvent::ShouldChangeStateEvent(ObjectId id, EntityState state)
	:_id(id), _state(state)
{
}

ShouldChangeStateEvent::~ShouldChangeStateEvent()
{
}

IEvent * ShouldChangeStateEvent::Copy() const
{
	return new ShouldChangeStateEvent(_id, _state);
}

const char * ShouldChangeStateEvent::GetName() const
{
	return "Should Change State Event";
}

#pragma endregion

DashEntityEvent::DashEntityEvent(ObjectId id, bool isOn)
	:_id(id), _isOn(isOn)
{
}

DashEntityEvent::~DashEntityEvent()
{
}

IEvent * DashEntityEvent::Copy() const
{
	return new DashEntityEvent(_id, _isOn);
}

const char * DashEntityEvent::GetName() const
{
	return "Dash Entity Event";
}


StopAndPushEntityEvent::StopAndPushEntityEvent(ObjectId id, bool direction, int amount)
	:_id(id), _direction(direction), _amount(amount)
{
}

StopAndPushEntityEvent::~StopAndPushEntityEvent()
{
}

IEvent * StopAndPushEntityEvent::Copy() const
{
	return new StopAndPushEntityEvent(_id, _direction, _amount);
}

const char * StopAndPushEntityEvent::GetName() const
{
	return "Stop And PushEntity Event";
}

JumpOffEntityEvent::JumpOffEntityEvent(ObjectId id)
	:_id(id)
{
}

JumpOffEntityEvent::~JumpOffEntityEvent()
{
}

IEvent * JumpOffEntityEvent::Copy() const
{
	return new JumpOffEntityEvent(_id);
}

const char * JumpOffEntityEvent::GetName() const
{
	return "Jump Off Entity Event";
}

FallingEvent::FallingEvent(ObjectId id)
	:_id(id)
{
}

FallingEvent::~FallingEvent()
{
}

IEvent * FallingEvent::Copy() const
{
	return new FallingEvent(_id);
}

const char * FallingEvent::GetName() const
{
	return "Falling Event";
}

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

const char * FrameEndedEvent::GetName() const
{
	return "Frame Ended Event";
}

CollisionEvent::CollisionEvent(ObjectId id, const TileProperty *properties, const Vector2 *overlapSize)
	:_id(id)
{
	for (int i = 0; i < 8; ++i)
	{
		_properties[i] = properties[i];
		_overlapSize[i] = overlapSize[i];
	}
}

CollisionEvent::~CollisionEvent()
{
}

IEvent * CollisionEvent::Copy() const
{
	return new CollisionEvent(_id, _properties, _overlapSize);
}

const char * CollisionEvent::GetName() const
{
	return "Collision Event";
}

