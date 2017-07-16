#ifndef EVENT_H
#define EVENT_H

enum  EventType
{
	EVENT_CREATE_ENTITY = 0xba1a49b3,
	EVENT_NOTIFY_AFTER_CREATE = 0xdb75ade7,
	EVENT_DESTROY_ENTITY = 0xad72e4fd,
	EVENT_DESTROY_ALL_ENTITY = 0x9bbb4be2,

	EVENT_PLAYER_INPUT = 0x7a25da9b,
	EVENT_PLAYER_POSITION = 0xa30ef9af,
	EVENT_FRAME_ENDED = 0xeae1adf4,
};

class IEvent
{
public:
	virtual EventType GetType() const = 0;
	virtual float GetTimeStamp() const = 0;
	virtual IEvent *Copy() const = 0;
	virtual const WCHAR *GetName() const = 0;
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
	const WCHAR *GetName() const;
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
	const WCHAR *GetName() const;
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
	const WCHAR *GetName() const;
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
	const WCHAR *GetName() const;
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
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	const ControlCommand &GetControlCommand();
private:
	ControlCommand _controlCommand;
	ObjectId _id;
	static EventType _type;
};


class FrameEndedEvent : public BaseEvent
{
public:
	explicit FrameEndedEvent(ObjectId id);
	virtual ~FrameEndedEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }

private:
	ObjectId _id;
	static EventType _type;
};

class PlayerPositionEvent : public BaseEvent
{
public :
	explicit PlayerPositionEvent(ObjectId id, const TilePosition &position, const Rect &rect, const Vector2 &rectOffset);
	virtual ~PlayerPositionEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }

	const TilePosition &GetPosition() { return _position; }
	const Rect &GetRect() { return _rect; }
	const Vector2 &GetRectOffset() { return _rectOffset; }


private:
	ObjectId _id;

	TilePosition _position;
	Rect _rect;
	Vector2 _rectOffset;

	static EventType _type;
};

#endif