#include "stdafx.h"
#include "UpperDeathState.h"

#include "Player.h"

void UpperDeathState::OnEnter(Player * object)
{
	object->SetGraphics(L"upperDeath");
}

State<Player>* UpperDeathState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_accel.y -= GRAVITY;

	return nullptr;
}

State<Player>* UpperDeathState::HandleCommand(Player * object, const ControlCommand & command)
{
	return nullptr;
}

void UpperDeathState::OnExit(Player * object)
{
}
