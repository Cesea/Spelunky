#include "stdafx.h"
#include "GrabState.h"

#include "Player.h"

#include "JumpState.h"
#include "FallingState.h"

#include "Player.h"


void GrabState::OnEnter(Player * object)
{
	object->SetGraphics(L"grab");
}

State<Player>* GrabState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	return nullptr;
}

State<Player>* GrabState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	if (command.jump == Command::Jump)
	{
		if (command.vertical == Command::MoveDown)
		{
			newState = new FallingState;
			return newState;
		}
		else
		{
			newState = new JumpState;
			return newState;
		}
	}
	return newState;
}

void GrabState::OnExit(Player * object)
{
}
