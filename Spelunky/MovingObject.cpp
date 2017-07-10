#include "stdafx.h"
#include "MovingObject.h"

MovingObject::MovingObject(ObjectId id)
	:GameObject::GameObject(id)
{
}

MovingObject::~MovingObject()
{
}

HRESULT MovingObject::Init(ArcheType type)
{
	_type = type;
	return S_OK;
}

void MovingObject::Release(void)
{
}

void MovingObject::Update(float deltaTime)
{
}

void MovingObject::Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos)
{
}

