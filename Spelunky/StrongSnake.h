#ifndef STRONG_SNAKE_H
#define STRONG_SNAKE_H

#include "Enemy.h"

class StrongSnake : public Enemy
{
	friend class StrongSnakeWalkState;
	friend class StrongSnakeSpitState;
	friend class StrongSnakeAttackState;

public :
	StrongSnake(ObjectId id);
	virtual ~StrongSnake();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void HandlePlayerPositionEvent(const IEvent *event);

private :
	void BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 &anchor);

	StateManager<StrongSnake> _stateManager;

	bool _attacking{ false };
};


#endif