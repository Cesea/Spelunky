#include "stdafx.h"
#include "EquipItem.h"

EquipItem::EquipItem(ObjectId id)
	:Item(id)
{
}

EquipItem::~EquipItem()
{
}

HRESULT EquipItem::Init(ArcheType type)
{
	return S_OK;
}

void EquipItem::Release(void)
{
}

void EquipItem::Update(float deltaTime)
{
}

void EquipItem::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
}

GameObject * EquipItem::Copy(ObjectId id)
{
	return nullptr;
}

void EquipItem::HandleMessage(const IEvent * event)
{
}

void EquipItem::Apply(GameObject * object)
{
}
