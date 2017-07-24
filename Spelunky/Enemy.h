#ifndef ENEMY_H
#define ENEMY_H

#include "MovingObject.h"

class Enemy : public MovingObject
{
public :
	Enemy(ObjectId id);
	virtual ~Enemy();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	void HandlePlayerPositionEvent(const IEvent *event);
	virtual void HandlePlayerAttackEvent(const IEvent *event) {}

protected :


};
	 


#endif