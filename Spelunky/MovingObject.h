#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H

#include "GameObject.h"

#include "MoveComponent.h"

class MovingObject : public GameObject
{
protected :
	friend class CollisionComponent;
public :
	MovingObject(ObjectId id);
	virtual ~MovingObject();

	virtual HRESULT Init(BaseProperty *property) = 0;
	virtual void Release(void) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos) = 0;

	virtual GameObject *Copy(ObjectId id)  = 0;

	Direction GetDirection() { return _seeingDirection; }
	void SetDirection(Direction direction) { _seeingDirection = direction; }
	inline bool GetCrawling() { return _crawling; }
	inline bool GetHolding() { return _holding; }
	inline void SetHolding(bool b) { _holding = b; }

	void SetVelocity(const Vector2 &v) { _velocity = v; };
	Vector2 GetVelocity() { return _velocity; }

protected  :
	Vector2 _speed{};
	Vector2 _accel{};
	Vector2 _velocity{};
	Vector2 _prevVelocity{};
	Vector2 _maxVelocity{};
	Direction _seeingDirection{};

	bool _onGround{ false };
	bool _crawling{ false };
	bool _holding{ false };
};


#endif