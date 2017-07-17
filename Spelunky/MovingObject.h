#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H

#include "GameObject.h"

#include "ColliisonComponent.h"
#include "MoveComponent.h"


class MovingObject : public GameObject
{
public :
	MovingObject(ObjectId id);
	virtual ~MovingObject();

	virtual HRESULT Init(ArcheType type);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id)  = 0;


protected  :

	Vector2 _speed{};
	Vector2 _accel{};
	Vector2 _velocity{};
	Vector2 _prevVelocity{};
	Vector2 _maxVelocity{};

	bool _onGround{ false };

	CollisionComponent *_collisionComp{};

	friend class MoveComponent;
	friend class SimpleMoveComponent;
	friend class CollisionComponent;
	friend class RectCollisionComponent;
	
};


#endif