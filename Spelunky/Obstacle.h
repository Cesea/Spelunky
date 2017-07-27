#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "MovingObject.h"

class Obstacle : public MovingObject
{
public :
	Obstacle(ObjectId id);
	virtual ~Obstacle();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void HandlePlayerPositionEvent(const IEvent *event);
	virtual void HandlePlayerAttackEvent(const IEvent *event);

protected:
	D2DSprite *_sprite{};

	ReturnTile _nearTiles{};

	CollisionComponent *_collisionComp{};
	IntVector2 _sourceIndex;
};


#endif