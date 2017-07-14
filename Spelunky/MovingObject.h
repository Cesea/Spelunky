#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H

#include "GameObject.h"


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
	virtual void HandleMessage(const IEvent *event) = 0;

	Rect GetRect() { return _rect; }

protected  :
	Rect _rect;
};


#endif