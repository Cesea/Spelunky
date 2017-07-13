#ifndef EVENT_H
#define EVENT_H

enum  EventType
{
	EVENT_CREATE_ENTITY = 0xba1a49b3,
	EVENT_NOTIFY_AFTER_CREATE = 0xdb75ade7,
	EVENT_DESTROY_ENTITY = 0xad72e4fd,
	EVENT_DESTROY_ALL_ENTITY = 0x9bbb4be2,

	EVENT_PLAYER_INPUT = 0x7a06903c,
	EVENT_CHANGE_DIRECTION = 0xf4b8f840,

	EVENT_MOVE_ENTITY = 0xf88245d8,
	EVENT_DASH_ENTITY = 0x84adf590,
	EVENT_STOP_AND_PUSH_ENTITY = 0x7711962d,
	EVENT_JUMP_OFF_ENTITY = 0xb7b3754c,
	EVENT_FALLING = 0x589928b2,

	EVENT_CHANGED_STATE = 0x93522ea6,
	EVENT_SHOULD_CHANGE_STATE = 0xb996d5a5,

	EVENT_FRAME_ENDED = 0xeae1adf4,

	EVENT_COLLISION = 0x29b5b326,
};

class IEvent
{
public:
	virtual EventType GetType() const = 0;
	virtual float GetTimeStamp() const = 0;
	virtual IEvent *Copy() const = 0;
	virtual const char *GetName() const = 0;
};

class BaseEvent : public IEvent
{
public:
	explicit BaseEvent(float timeStamp = 0.0f);
	virtual ~BaseEvent();

	virtual EventType GetType() const = 0;

	float GetTimeStamp() const { return _timeStamp; }

protected:
	float _timeStamp;
};

class CreateEntityEvent : public BaseEvent
{
public:
	explicit CreateEntityEvent(ArcheType objectType);
	virtual ~CreateEntityEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }
private:
	static EventType _type;
	ArcheType _objectType;
};

class NotifyAfterCreateEvent : public BaseEvent
{
public:
	explicit NotifyAfterCreateEvent(ObjectId id);
	virtual ~NotifyAfterCreateEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }
	ObjectId GetId() { return _id; }
private:
	ObjectId _id;
	static EventType _type;
};

class DestroyEntityEvent : public BaseEvent
{
public:
	explicit DestroyEntityEvent(ObjectId id);
	virtual ~DestroyEntityEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	ObjectId GetId() { return _id; };
	EventType GetType() const override { return _type; }
private:
	ObjectId _id;
	static EventType _type;
};

class DestroyAllEntityEvent : public BaseEvent
{
public:
	explicit DestroyAllEntityEvent();
	virtual ~DestroyAllEntityEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }
private:
	static EventType _type;
};

class PlayerInputEvent : public BaseEvent
{
public:
	explicit PlayerInputEvent(ObjectId id, const ControlCommand &controlCommand);
	virtual ~PlayerInputEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	const ControlCommand &GetControlCommand();
private:
	ControlCommand _controlCommand;
	ObjectId _id;
	static EventType _type;
};

class ChangeDirectionEvent : public BaseEvent
{
public:
	explicit ChangeDirectionEvent(ObjectId id);
	virtual ~ChangeDirectionEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
private:
	ObjectId _id;
	static EventType _type;
};

class MoveEntityEvent : public BaseEvent
{
public:
	explicit MoveEntityEvent(ObjectId id);
	virtual ~MoveEntityEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
private:
	ObjectId _id;
	static EventType _type;
};
class DashEntityEvent : public BaseEvent
{
public:
	explicit DashEntityEvent(ObjectId id, bool isOn);
	virtual ~DashEntityEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	bool GetIsOn() { return _isOn; }
private:
	ObjectId _id;
	bool _isOn{ false };
	static EventType _type;
};

class JumpOffEntityEvent : public BaseEvent
{
public:
	explicit JumpOffEntityEvent(ObjectId id);
	virtual ~JumpOffEntityEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
private:
	ObjectId _id;
	static EventType _type;
};

class FallingEvent : public BaseEvent
{
public:
	explicit FallingEvent(ObjectId id);
	virtual ~FallingEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
private:
	ObjectId _id;
	static EventType _type;
};

class StopAndPushEntityEvent : public BaseEvent
{
public:
	explicit StopAndPushEntityEvent(ObjectId id, bool direction, int amount);
	virtual ~StopAndPushEntityEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	bool GetDirection() { return _direction; }
	int GetAmount() { return _amount; }
private:
	ObjectId _id;
	bool _direction;
	int _amount;
	static EventType _type;
};

//이 이벤트는 스테이트가 변화하고 난 후에 발생 될 이벤트이다.
class ChangedStateEvent : public BaseEvent
{
public:
	explicit ChangedStateEvent(ObjectId id, EntityState state);
	virtual ~ChangedStateEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	EntityState GetState() { return _state; }

private:
	ObjectId _id;
	EntityState _state;
	static EventType _type;
};

class ShouldChangeStateEvent : public BaseEvent
{
public:
	explicit ShouldChangeStateEvent(ObjectId id, EntityState state);
	virtual ~ShouldChangeStateEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	EntityState GetState() { return _state; }

private:
	ObjectId _id;
	EntityState _state;
	static EventType _type;
};

class FrameEndedEvent : public BaseEvent
{
public:
	explicit FrameEndedEvent(ObjectId id);
	virtual ~FrameEndedEvent();
	IEvent *Copy() const override;
	const char *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }

private:
	ObjectId _id;
	static EventType _type;
};

//class CollisionEvent : public BaseEvent
//{
//public:
//	explicit CollisionEvent(ObjectId id, const TileProperty *properties, const Vector2 *overlapSize);
//	virtual ~CollisionEvent();
//	IEvent *Copy() const override;
//	const char *GetName() const;
//	EventType GetType() const override { return _type; }
//
//	ObjectId GetId() { return _id; }
//
//	//CollisionDirection GetCollisionDirection() { return _direction; }
//	TileProperty *GetTileProperties() { return _properties; }
//	Vector2 *GetOverlapsSize() { return _overlapSize; }
//	//int *GetOverlapAmount() { return _overlapAmount; }
//
//private:
//	ObjectId _id;
//	TileProperty _properties[8];
//	Vector2 _overlapSize[8];
//	//int _overlapAmount[6]{};
//	//CollisionDirection _direction;
//	static EventType _type;
//};


#endif