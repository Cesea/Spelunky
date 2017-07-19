#ifndef COLLIDING_OBJECT_H
#define COLLIDING_OBJECT_H

#include "GameObject.h"

class CollidingObject : public GameObject
{

	friend class CollisionComponent;
public :
	CollidingObject(ObjectId id);
	virtual ~CollidingObject();

	virtual HRESULT Init(BaseProperty *property) = 0;
	virtual void Release(void) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos) = 0;

	virtual GameObject *Copy(ObjectId id) = 0;

protected  :

	CollisionComponent *_collisionComp{};

};

#endif