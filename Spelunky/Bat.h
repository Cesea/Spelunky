#ifndef BAT_H
#define BAT_H

#include "Enemy.h"

class Bat : public Enemy
{
public:

	friend class BatOnTopState;
	friend class BatFlyingState;

	Bat(ObjectId id);
	virtual ~Bat();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void HandlePlayerPositionEvent(const IEvent *event);
private:
	void BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 &anchor);

	StateManager<Bat> _stateManager;

	bool32 _flying{ false };
};



#endif