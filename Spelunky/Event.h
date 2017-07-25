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

	EVENT_ON_TUNNEL = 0xf69ed5c1,

	EVENT_ITEM_BREAK = 0x60740c30,
	EVENT_PUT_DOWN = 0xdc7c30a9,

	EVENT_PLAYER_ATTACK = 0x1586e71c,
	EVENT_PLAYER_GO_EXIT = 0x97d481c8,

	EVENT_THROW_BOMB = 0x49500922,
	EVENT_ENEMY_INPUT = 0x17246236,
	
	EVENT_ENEMY_DEAD = 0xaa04c0a1,
	EVENT_PLAYER_UPPER_JUMP = 0x4fdb83af,

	EVENT_DAMAGE = 0x1739229c,

	EVENT_ON_MIDDLE_STAGE = 0xd4ed69c2,
	EVENT_EXIT_MIDDLE_STAGE = 0xc68b0567,

	EVENT_PLAYER_DAMAGED = 0x8b6aca10,
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
	virtual IEvent *Copy() const = 0;

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
	explicit PlayerPositionEvent(ObjectId id, const TilePosition &position, 
		const Rect &rect, const Vector2 &rectOffset, const bool32 isFalling);
	virtual ~PlayerPositionEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }

	const TilePosition &GetPosition() { return _position; }
	const Rect &GetRect() { return _rect; }
	const Vector2 &GetRectOffset() { return _rectOffset; }
	const bool32  &GetFalling() { return _isFalling; }

private:
	ObjectId _id;

	TilePosition _position;
	Rect _rect;
	Vector2 _rectOffset;
	bool32  _isFalling;

	static EventType _type;
};

class CollectMoneyEvent : public BaseEvent
{
public :
	explicit CollectMoneyEvent(ObjectId id, ObjectId targetId, int value, GemType type);
	virtual ~CollectMoneyEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	ObjectId GetTargetid() { return _targetId; }
	int GetValue() { return _value; }
	GemType GetGemType() { return _gemType; }
private :
	ObjectId _id;
	ObjectId _targetId;
	int _value;
	static EventType _type;
	GemType _gemType;
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
	explicit HoldingEvent(ObjectId id, ObjectId ownerId, EquipSlot slot);
	virtual ~HoldingEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	ObjectId GetOwnerId() { return _ownerId; }
	EquipSlot GetSlot() { return _slot; }
private :
	ObjectId _id;
	ObjectId _ownerId;
	EquipSlot _slot;
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

class OnTunnelEvent : public BaseEvent
{
public :
	OnTunnelEvent(const IntVector2 &position);
	virtual ~OnTunnelEvent() {}

	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }
	const IntVector2 &GetPosition() { return _position; }
private :
	static EventType _type;
	IntVector2 _position;
};

class ItemBreakEvent : public BaseEvent
{
public :
	ItemBreakEvent(const ObjectId id, BreakType type);
	virtual ~ItemBreakEvent() {}

	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	BreakType GetBreakType() { return _breakType; }
private :
	static EventType _type;
	ObjectId _id{0};
	BreakType _breakType{};
};

class PutDownEvent : public BaseEvent
{
public:
	PutDownEvent(const ObjectId id, Direction direction);
	virtual ~PutDownEvent() {}

	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }
	ObjectId GetId() { return _id; }
	Direction GetDirection() { return _direction; }

private:
	ObjectId _id;
	Direction _direction;
	static EventType _type;
};

class PlayerAttackEvent : public BaseEvent
{
public :
	PlayerAttackEvent(Direction direction, TilePosition position);
	virtual ~PlayerAttackEvent() { }

	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	const TilePosition &GetTilePosition() { return _position; }
	const Direction &GetDirection() { return _direction; }

private :
	TilePosition _position;
	Direction _direction;

	static EventType _type;
};

class PlayerGoExitEvent : public BaseEvent
{
public :
	PlayerGoExitEvent(bool isMiddle);
	virtual ~PlayerGoExitEvent();

	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	bool GetIsMiddle() { return _isMiddle; }

private :
	static EventType _type;
	bool _isMiddle{ false };
};

class ThrowBombEvent : public BaseEvent
{
public :
	ThrowBombEvent(const TilePosition & throwPosition, const Vector2 initialVelocity, bool isSticky);
	virtual ~ThrowBombEvent();

	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	const TilePosition &GetThrowPosition() { return _throwPosition; }
	const Vector2 &GetInitialVelocity() { return _initVel; }
	const bool32 GetIsSticky() { return _isSticky; }

private :
	static EventType _type;
	TilePosition _throwPosition;
	Vector2 _initVel;
	bool32 _isSticky;
};

class EnemyInputEvent : public BaseEvent
{
public:
	explicit EnemyInputEvent(ObjectId id, const ControlCommand &controlCommand);
	virtual ~EnemyInputEvent();
	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetId() { return _id; }
	const ControlCommand &GetControlCommand() { return _controlCommand; }
private:
	ControlCommand _controlCommand;
	ObjectId _id;
	static EventType _type;
};

class EnemyDeadEvent : public BaseEvent
{
public :
	EnemyDeadEvent(const ObjectId id, EnemyType enemytype);
	virtual ~EnemyDeadEvent() {}

	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	EnemyType GetEnemyType() { return _enemyType; }
	ObjectId GetId() { return _id; }
private:
	static EventType _type;
	ObjectId _id{ 0 };
	EnemyType _enemyType;
};

class PlayerUpperJumpEvent : public BaseEvent
{
public :
	PlayerUpperJumpEvent(const Vector2 &otherVelocity);
	virtual ~PlayerUpperJumpEvent() {}

	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	const Vector2 &GetOtherVelocity() { return _otherVelocity; }
private:
	static EventType _type;
	Vector2 _otherVelocity;
};

class DamageEvent : public BaseEvent
{
public :
	DamageEvent(ObjectId id, int damage, const TilePosition &position, const Rect &rect, const Vector2 &rectOffset);
	virtual ~DamageEvent();

	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	const Vector2 &GetOtherVelocity() { return _otherVelocity; }
	int GetDamage() { return _damage; }
	const TilePosition &GetTilePosition() { return _position; }
	const Rect &GetRect() { return _rect; }
	const Vector2 &GetRectOffset() { return _rectOffset; }
private:
	static EventType _type;
	Vector2 _otherVelocity;

	int _id;
	int _damage;
	TilePosition _position;
	Rect _rect;
	Vector2 _rectOffset;
};

class OnMiddleStageEvent : public BaseEvent
{
public:
	OnMiddleStageEvent() {}
	virtual ~OnMiddleStageEvent() {}

	IEvent *Copy() const override
	{
		return new OnMiddleStageEvent();
	}
	const WCHAR *GetName() const
	{
		return L"On Middle Stage Event";
	}
	EventType GetType() const override { return _type; }

private :
	static EventType _type;
};

class ExitMiddleStageEvent : public BaseEvent
{
public:
	ExitMiddleStageEvent() {}
	virtual ~ExitMiddleStageEvent() {}

	IEvent *Copy() const override
	{
		return new ExitMiddleStageEvent();
	}
	const WCHAR *GetName() const
	{
		return L"Exit Middle Stage Event";
	}
	EventType GetType() const override { return _type; }

private :
	static EventType _type;
};

class PlayerDamagedEvent : public BaseEvent
{
public :
	PlayerDamagedEvent(ObjectId attackerId, int damage, const Vector2 &posDiff);
	virtual ~PlayerDamagedEvent() {}

	IEvent *Copy() const override;
	const WCHAR *GetName() const;
	EventType GetType() const override { return _type; }

	ObjectId GetAttackerId() { return _attackerId; }
	int GetDamage() { return _damage; }
	const Vector2 &GetPosDiff() { return _posDiff; }

private:
	static EventType _type;
	ObjectId _attackerId;
	int _damage;
	Vector2 _posDiff;
};


#endif