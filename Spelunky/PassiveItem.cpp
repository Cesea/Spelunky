#include "stdafx.h"
#include "PassiveItem.h"

PassiveItem::PassiveItem(ObjectId id)
	:Item::Item(id)
{
}

PassiveItem::~PassiveItem()
{
}

HRESULT PassiveItem::Init(ArcheType type)
{
	return S_OK;
}

void PassiveItem::Release(void)
{
}

void PassiveItem::Update(float deltaTime)
{
}

void PassiveItem::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
}

void PassiveItem::Use()
{
}

GameObject * PassiveItem::Copy(ObjectId id)
{
	return nullptr;
}

void PassiveItem::HandleMessage(const IEvent * event)
{
}

void PassiveItem::Apply(GameObject * object)
{
}
