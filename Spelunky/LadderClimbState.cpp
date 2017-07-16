#include "stdafx.h"
#include "LadderClimbState.h"

#include "Player.h"

#include "IdleState.h"
#include "JumpState.h"

void LadderIdleState::OnEnter(Player * object)
{
	object->SetGraphics(L"ladderIdle");
	object->_velocity.y = 0.0f;
}

State<Player>* LadderIdleState::Update(Player * object, float deltaTime)
{
	return nullptr;
}

State<Player>* LadderIdleState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	if (command.vertical == Command::MoveUp)
	{
		if (object->_canClimbUp)
		{
			object->_accel.y -= object->_speed.y;
			newState = new LadderClimbState;
		}
	}
	else if(command.vertical == Command::MoveDown)
	{
		object->_accel.y -= object->_speed.y;
		newState = new LadderClimbState;
	}

	if (command.jump == Command::Jump)
	{
		newState = new JumpState;
		return newState;
	}

	return newState;
}

void LadderIdleState::OnExit(Player * object)
{
}

void LadderClimbState::OnEnter(Player * object)
{
	object->SetGraphics(L"ladderClimb");
}

State<Player>* LadderClimbState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_accel.y -= GRAVITY;
	object->_velocity += object->_accel * deltaTime;
	ClampFloat(&object->_velocity.y, -object->_climbSpeed, object->_climbSpeed);

	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (object->_onGround)
	{
		newState = new IdleState;
	}

	if (!_wasControlled)
	{
		newState = new LadderIdleState;
	}
	else
	{
		_wasControlled = false;
	}
	object->_accel = Vector2();

	return newState;
}

State<Player>* LadderClimbState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	if (command.vertical == Command::MoveUp)
	{
		if (object->_canClimbUp)
		{
			_wasControlled = true;
			object->_accel.y = -object->_speed.y;
		}
	}
	else if(command.vertical == Command::MoveDown)
	{
		_wasControlled = true;
		object->_accel.y = object->_speed.y;
	}

	if (command.jump == Command::Jump)
	{
		newState = new JumpState;
		return newState;
	}

	return newState;
}

void LadderClimbState::OnExit(Player * object)
{
}
