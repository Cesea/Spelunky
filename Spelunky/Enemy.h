#ifndef ENEMY_H
#define ENEMY_H

#include "MovingObject.h"

class Enemy : public MovingObject
{
public:
	Enemy(ObjectId id);
	virtual ~Enemy();

	virtual HRESULT Init(BaseProperty *property);
	virtual void PostInit();
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	void HandlePlayerPositionEvent(const IEvent *event);
	void HandlePlayerAttackEvent(const IEvent *event);
	void HandleDamageEvent(const IEvent *event);
	void HandleObstaclePositionEvent(const IEvent *event);

	D2DSprite *GetCurrentGraphics() { return _currentSprite; }
	void SetGraphics(const std::wstring &key);

	EnemyType GetEnemyType() { return _enemyType; }

protected :
	void Damaged(int damage, Direction hitDirection);

protected :

	CollisionComponent *_collisionComp{};
	DataSet<D2DSprite *> _graphics;
	D2DSprite *_currentSprite{};

	ReturnTile _nearTiles;
	EnemyType _enemyType;

	Timer _eventDispatchTimer;

	int _hp{ 0 };

};

#endif