#include "stdafx.h"
#include "LookRevertState.h"

#include "Player.h"

#include "IdleState.h"

void LookRevertState::OnEnter(Player * object)
{
	object->SetGraphics(L"lookRevert");
}

State<Player>* LookRevertState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	if (object->_onObject)
	{
		object->_accel.y -= GRAVITY * 0.99;
	}

	return nullptr;
}

State<Player>* LookRevertState::HandleCommand(Player * object, const ControlCommand & command)
{
	return nullptr;
}

State<Player>* LookRevertState::HandleFrameEndEvent(Player * actor)
{
	State<Player> *newState = nullptr;
	newState = new IdleState;
	return newState;
}

void LookRevertState::OnExit(Player * object)
{
}
