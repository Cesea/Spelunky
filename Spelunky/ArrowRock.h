#ifndef ARROW_ROCK_H
#define ARROW_ROCK_H


#include "Obstacle.h"

class ArrowRock	 : public Obstacle
{
public:
	ArrowRock(ObjectId id);
	virtual ~ArrowRock();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void HandlePlayerPositionEvent(const IEvent *event);
	virtual void HandlePlayerAttackEvent(const IEvent *event);

protected:
	bool _fired{ false };
	ObstacleType _obstacleType;
};


#endif