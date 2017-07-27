#ifndef BOMB_H
#define BOMB_H

#include "MovingObject.h"

class Bomb : public MovingObject
{
public :
	Bomb(ObjectId id);
	virtual ~Bomb();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);
	virtual void HandleObstaclePositionEvent(const IEvent *event);

private :
	D2DSprite *_sprite{};

	Timer _bombTimer;
	int _bombStage{};

	Timer _scaleTimer;

	float _startScale;
	float _targetScale;
	float _scale{1.0f};

	bool _yScaling{false};
	D2D1::Matrix3x2F mat;

	bool _isSticky{ false };
	ReturnTile _nearTiles;

	CollisionComponent *_collisionComp{};
};
#endif