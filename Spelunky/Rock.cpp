#include "stdafx.h"
#include "Rock.h"

Rock::Rock(ObjectId id)
	:EquipItem(id)
{
}

Rock::~Rock()
{
}

HRESULT Rock::Init(ArcheType type)
{
	EquipItem::Init(type);
	_type = type;
	_sprite = new D2DSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"rock"), 0, 0, 80, 80, IntVector2(-40, -50));

	return S_OK;
}

void Rock::Release(void)
{
	EquipItem::Release();
	SAFE_RELEASE_AND_DELETE(_sprite);
}

void Rock::Update(float deltaTime)
{
	EquipItem::Update(deltaTime);
}

void Rock::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	EquipItem::Render(renderTarget, camPos);
}

void Rock::Use()
{
}

GameObject * Rock::Copy(ObjectId id)
{
	return nullptr;
}

void Rock::Apply(ObjectId id)
{
}
