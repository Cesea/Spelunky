#ifndef PLAYER_H
#define PLAYER_H

#include "MovingObject.h"

class Player : public MovingObject
{
public :
	Player(ObjectId id);
	virtual ~Player();

	virtual HRESULT Init(ArcheType type);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);
	virtual void HandleMessage(const IEvent *event);

	Rect GetRect() { return _rect; }


private :

};

#endif