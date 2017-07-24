#include "stdafx.h"
#include "ExitState.h"

#include "Player.h"

void ExitState::OnEnter(Player * object)
{
	object->SetGraphics(L"exit");
}

State<Player>* ExitState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);
	return nullptr;
}

State<Player>* ExitState::HandleCommand(Player * object, const ControlCommand & command)
{
	return nullptr;
}

void ExitState::OnExit(Player * object)
{
}
