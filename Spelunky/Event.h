#ifndef EVENT_H
#define EVENT_H


enum  EventType
{
	EVENT_CREATE_OBJECT = 0xba1a49b3,
	EVENT_NOTIFY_AFTER_CREATE = 0xdb75ade7,
	EVENT_DESTROY_OBJECT = 0xad72e4fd,
	EVENT_DESTROY_ALL_OBJECT = 0x9bbb4be2,

	EVENT_PLAYER_INPUT = 0x7a25da9b,
	EVENT_PLAYER_POSITION = 0xa30ef9af,
	EVENT_FRAME_ENDED = 0xeae1adf4,

	EVENT_COLLECT_MONEY = 0xe7d8e46e,
	EVENT_OBJECT_DEAD = 0x8772f719,
	EVENT_PICK_UP = 0x95460042,
	EVENT_HOLDING = 0x8bd7ebf8,

	EVENT_STAGE_TRANSITIOIN = 0xce584715,
	EVENT_DESTROY_TILE = 0xa0535762,
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

class CreateObjectEvent : public BaseEvent
{
public:
	explicit CreateObjectEvent(const std::wstring &key, BaseProperty *property);
	virtual ~CreateObjectEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	const std::wstring &GetKey() { return _key; }
	BaseProperty *GetProperty() { return _property; }
private:
	static EventType _type;

	std::wstring _key{};
	BaseProperty *_property{};
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

class DestroyObjectEvent : public BaseEvent
{
public:
	explicit DestroyObjectEvent(ObjectId id);
	virtual ~DestroyObjectEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	ObjectId GetId() { return _id; };
	EventType GetType() const override { return _type; }
private:
	ObjectId _id;
	static EventType _type;
};

class DestroyAllObjectEvent : public BaseEvent
{
public:
	explicit DestroyAllObjectEvent();
	virtual ~DestroyAllObjectEvent();
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

class CollectMoneyEvent : public BaseEvent
{
public :
	explicit CollectMoneyEvent(ObjectId id, ObjectId targetId, int value);
	virtual ~CollectMoneyEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	ObjectId GetTargetid() { return _targetId; }
	int GetValue() { return _value; }
private :
	ObjectId _id;
	ObjectId _targetId;
	int _value;
	static EventType _type;
};

class ObjectDeadEvent : public BaseEvent
{
public :
	explicit ObjectDeadEvent(ObjectId id, bool32 fromAbove);
	virtual ~ObjectDeadEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	bool32 GetFromAbove() { return _fromAbove; }
private :
	ObjectId _id;
	bool32 _fromAbove;
	static EventType _type;
};

class PickupEvent : public BaseEvent
{
public :
	explicit PickupEvent(ObjectId id);
	virtual ~PickupEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
private :
	ObjectId _id;
	static EventType _type;
};

class HoldingEvent : public BaseEvent
{
public :
	explicit HoldingEvent(ObjectId id, ObjectId ownerId);
	virtual ~HoldingEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	ObjectId GetOwnerId() { return _ownerId; }
private :
	ObjectId _id;
	ObjectId _ownerId;
	static EventType _type;
};

class StageTransitionEvent : public BaseEvent
{
public :
	explicit StageTransitionEvent();
	virtual ~StageTransitionEvent();
	
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }
private :
	static EventType _type;
};

class DestroyTileEvent : public BaseEvent
{
public :
	explicit DestroyTileEvent(const IntVector2 &position);
	virtual ~DestroyTileEvent();
	
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	const IntVector2 &GetPosition() { return _position; }
private :
	static EventType _type;
	IntVector2 _position;
};

#endif