#include "stdafx.h"
#include "Gem.h"

Gem::Gem(ObjectId id)
	:PassiveItem(id)
{
}

Gem::~Gem()
{
}

HRESULT Gem::Init(ArcheType type)
{
	_sprite = new D2DSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"gem"), 0, 0, 80, 80, IntVector2(-40, -80));

	_type = type;

	_rect = RectMake(0, 0, 48, 48);
	_rectOffset = Vector2(-24, -48);

	return S_OK;
}

void Gem::Release(void)
{
	SAFE_RELEASE_AND_DELETE(_sprite);
}

void Gem::Update(float deltaTime)
{
}

void Gem::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position.UnTilelize() - camPos;
	_sprite->Render(renderTarget, drawingPos.x, drawingPos.y);

}

void Gem::Use()
{
}

GameObject * Gem::Copy(ObjectId id)
{
	return nullptr;
}

void Gem::HandleMessage(const IEvent * event)
{
}

void Gem::Apply(GameObject * object)
{
}
