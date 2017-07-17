#ifndef COLLIDING_OBJECT_H
#define COLLIDING_OBJECT_H

#include "GameObject.h"

class CollidingObject : public GameObject
{
public :
	CollidingObject(ObjectId id);
	virtual ~CollidingObject();

	virtual HRESULT Init(ArcheType type);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id) = 0;

	Rect GetRect() { return _rect; }

protected  :
	Rect _rect;


};

#endif