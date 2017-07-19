#ifndef ROCK_H
#define ROCK_H

#include "EquipItem.h"

class Rock : public EquipItem
{
public:
	Rock(ObjectId id);
	virtual ~Rock();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual void Use(const ControlCommand &commands);

	virtual GameObject *Copy(ObjectId id);

	virtual void Apply(ObjectId id);

private:

};

#endif