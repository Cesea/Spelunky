#include "stdafx.h"
#include "LookUpState.h"

void LookUpState::OnEnter(Player * object)
{
}

State<Player>* LookUpState::Update(Player * object, float deltaTime)
{
	return nullptr;
}

State<Player>* LookUpState::HandleCommand(Player * object, const ControlCommand & command)
{
	return nullptr;
}

void LookUpState::OnExit(Player * object)
{
}
