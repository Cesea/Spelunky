#ifndef EATABLES_H
#define EATABLES_H

#include "PassiveItem.h"

class Eatables : public PassiveItem
{
public:
	friend class Stage;

	Eatables(ObjectId id);
	virtual ~Eatables();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void Apply(ObjectId id);

	virtual void HandlePlayerAttackEvent(const IEvent *event);

private:
	EatableType _eatableType;

	void operator= (EatableProperty *property);
	Delegate<void, int> _applyFunction;
};


#endif