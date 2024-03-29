#ifndef PUSHING_ROCK_H
#define PUSHING_ROCK_H

#include "Obstacle.h"

class PushingRock : public Obstacle
{
public:
	PushingRock(ObjectId id);
	virtual ~PushingRock();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void HandlePlayerPositionEvent(const IEvent *event);
	virtual void HandleObstaclePositionEvent(const IEvent *event);

protected:
	ObstacleType _obstacleType;
	bool32 _beeingPushed{ false };
	float _prevXOverlapAmount{ 0 };

	float _prevYVel{};
};

#endif