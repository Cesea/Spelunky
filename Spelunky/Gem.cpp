#include "stdafx.h"
#include "Gem.h"

Gem::Gem(ObjectId id)
	:PassiveItem(id)
{
}

Gem::~Gem()
{
}

HRESULT Gem::Init(ArcheType type, int value)
{
	_sprite = new D2DSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"gem"), 0, 0, 80, 80, IntVector2(-40, -80));

	_type = type;


	//_rect = RectMake(0, 0, 48, 48);
	//_rectOffset = Vector2(-24, -48);
	_value = value;

	return S_OK;
}

void Gem::Release(void)
{
	SAFE_RELEASE_AND_DELETE(_sprite);
}

void Gem::Update(float deltaTime)
{
	if (_actorOn)
	{
		Apply(_onActorId);
	}
	if (!_valid)
	{
		EVENTMANAGER->QueueEvent(new DestroyObjectEvent(_id));
	}
}

void Gem::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position.UnTilelize() - camPos;
	_sprite->Render(renderTarget, drawingPos.x, drawingPos.y);

}


GameObject * Gem::Copy(ObjectId id)
{
	return nullptr;
}


void Gem::Apply(ObjectId id)
{
	EVENTMANAGER->QueueEvent(new CollectMoneyEvent(_id, _onActorId, _value));
	_valid = false;
}
