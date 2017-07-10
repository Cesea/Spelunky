#include "stdafx.h"
#include "Player.h"

Player::Player(ObjectId id)
	:MovingObject::MovingObject(id)
{
}

Player::~Player()
{
}

HRESULT Player::Init(ArcheType type)
{
	return S_OK;
}

void Player::Release(void)
{
}

void Player::Update(float deltaTime)
{
}

void Player::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
}

GameObject * Player::Copy(ObjectId id)
{
	return nullptr;
}

void Player::HandleMessage(const IEvent * event)
{
}
