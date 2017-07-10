#include "stdafx.h"
#include "ColiidingObject.h"

CollidingObject::CollidingObject(ObjectId id)
	:GameObject::GameObject(id)
{
}

CollidingObject::~CollidingObject()
{
}

HRESULT CollidingObject::Init(ArcheType type)
{
	return S_OK;
}

void CollidingObject::Release(void)
{
}

void CollidingObject::Update(float deltaTime)
{
}

void CollidingObject::Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos)
{
}