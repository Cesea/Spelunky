#ifndef SNAKE_H
#define SANKE_H

#include "Enemy.h"

class Snake : public Enemy
{
public :

	friend class SnakeWalkState;
	friend class SnakeAttackState;

	Snake(ObjectId id);
	virtual ~Snake();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void HandlePlayerPositionEvent(const IEvent *event);

private :

	void BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 &anchor);

	StateManager<Snake> _stateManager;

	bool _attacking{ false };
};



#endif