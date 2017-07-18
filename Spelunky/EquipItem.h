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

	virtual void Use(const ControlCommand &commands);

	virtual GameObject *Copy(ObjectId id);

	void HandlePlayerInputEvent(const IEvent *event);

	virtual void Apply(ObjectId id);

	void SetEquiped(bool32 equiped) { _equiped = equiped; }
	bool32 GetEquiped() { return _equiped; }

protected :
	
	bool32 _equiped{ false };
	MovingObject *_pOwner{};

	bool _wasControlled{ false };


};

#endif