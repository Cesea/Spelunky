#include "stdafx.h"
#include "Obstacle.h"

Obstacle::Obstacle(ObjectId id)
	:MovingObject(id)
{
}

Obstacle::~Obstacle()
{
}

HRESULT Obstacle::Init(BaseProperty * property)
{
	return S_OK;
}

void Obstacle::Release(void)
{
}

void Obstacle::Update(float deltaTime)
{
}

void Obstacle::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
}

GameObject * Obstacle::Copy(ObjectId id)
{
	return nullptr;
}

void Obstacle::HandlePlayerPositionEvent(const IEvent * event)
{
}

void Obstacle::HandlePlayerAttackEvent(const IEvent * event)
{
}

void Obstacle::HandleObstaclePositionEvent(const IEvent * event)
{
}
