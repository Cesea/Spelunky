#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject(ObjectId id)
	:_id(id)
{
}

GameObject::~GameObject()
{
}

HRESULT GameObject::Init(ArcheType type)
{
	_type = type;
	return S_OK;
}

void GameObject::Release(void)
{
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos)
{
}
