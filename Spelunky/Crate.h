#ifndef CRATE_H
#define CRATE_H

#include "EquipItem.h"

class Crate : public EquipItem
{
public:
	Crate(ObjectId id);
	virtual ~Crate();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual void Use(const ControlCommand &commands);

	virtual GameObject *Copy(ObjectId id);

	virtual void Apply(ObjectId id);

	virtual void HandlePlayerAttackEvent(const IEvent *event);
	void HandleDamageEvent(const IEvent *event);

	inline BreakType GetBreakType() { return _breakType; }
	inline CrateType GetCrateType() { return _crateType; }

	void SpawnItem();

private:

	void operator= (const CrateProperty *property);

	IntVector2 _sourceIndex{};
	bool32 _throwed{ false };
	BreakType _breakType;

	CrateType _crateType;
};

#endif