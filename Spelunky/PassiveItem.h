#ifndef PASSIVE_ITEM_H
#define PASSIVE_ITEM_H

#include "Item.h"

class PassiveItem : public Item
{
public:
	PassiveItem(ObjectId id);
	virtual ~PassiveItem();

	virtual HRESULT Init(ArcheType type);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);


	virtual GameObject *Copy(ObjectId id);

	virtual void Apply(ObjectId i);

private:

};


#endif