#ifndef THROWS_H
#define THROWS_H

#include "EquipItem.h"

class Throws : public EquipItem
{
public:
	Throws(ObjectId id);
	virtual ~Throws();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual void Use(const ControlCommand &commands);

	virtual GameObject *Copy(ObjectId id);

	virtual void Apply(ObjectId id);

	virtual void HandlePlayerAttackEvent(const IEvent *event);

	inline BreakType GetBreakType() { return _breakType; }

private:

	void operator= (const ThrowProperty *property);

	IntVector2 _sourceIndex{};
	bool32 _breakable{};
	bool32 _throwed{false};

	BreakType _breakType;
};

#endif