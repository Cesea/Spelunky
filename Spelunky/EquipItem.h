#ifndef EQUIP_ITEM_H
#define EQUIP_ITEM_H

#include "Item.h"

class EquipItem : public Item
{
public :
	EquipItem(ObjectId id);
	virtual ~EquipItem();

	virtual HRESULT Init(ArcheType type);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);
	virtual void HandleMessage(const IEvent *event);

	virtual void Apply(GameObject *object);

private :

};

#endif