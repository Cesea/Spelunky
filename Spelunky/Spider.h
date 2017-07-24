#ifndef SPIDER_H
#define SPIDER_H

#include "Enemy.h"

class Spider : public Enemy
{
public :

	friend class SnakeWalkState;
	friend class SnakeAttackState;

	Spider(ObjectId id);
	virtual ~Spider();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void HandlePlayerPositionEvent(const IEvent *event);

private :

	void BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 &anchor);

	StateManager<Spider> _stateManager;

	bool _onTop{ true };
	bool _jumping{ false };
};

#endif