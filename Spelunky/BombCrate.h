#ifndef BOMB_CRATE_H
#define BOMB_CRATE_H

#include "Obstacle.h"

class BombCrate : public Obstacle
{
public :
	BombCrate(ObjectId id);
	virtual ~BombCrate();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void HandlePlayerPositionEvent(const IEvent *event);
	virtual void HandlePlayerAttackEvent(const IEvent *event);
	virtual void HandleObstaclePositionEvent(const IEvent *event);
	virtual void HandleDamageEvent(const IEvent *event);

protected:
	Timer _explosionTimer;
	bool _timerOn{ false };
	ObstacleType _obstacleType;
	float _prevYVel{};
};


#endif