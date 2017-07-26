#ifndef SPIDER_H
#define SPIDER_H

#include "Enemy.h"

class Spider : public Enemy
{
public :

	friend class SpiderJumpState;
	friend class SpiderOnGroundState;
	friend class SpiderOnTopState;

	Spider(ObjectId id);
	virtual ~Spider();

	virtual HRESULT Init(BaseProperty *property);
	virtual void PostInit();
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void HandlePlayerPositionEvent(const IEvent *event);

private :

	void BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 &anchor);

	StateManager<Spider> _stateManager;

	bool32 _onTop{ true };
	bool32 _jumping{ false };

	Tile *_holdingTile{};
};

#endif