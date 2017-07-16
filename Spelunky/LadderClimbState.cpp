#include "stdafx.h"
#include "LadderClimbState.h"

#include "Player.h"

#include "IdleState.h"
#include "JumpState.h"

void LadderIdleState::OnEnter(Player * object)
{
	object->SetGraphics(L"ladderIdle");
	object->_velocity = Vector2();

	object->desiredPosition.tileRel.x = 32.0f;
	object->_stateClimbing = true;
}

State<Player>* LadderIdleState::Update(Player * object, float deltaTime)
{
	object->_accel.y -= GRAVITY;
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
	object->_stateClimbing = false;
}

void LadderClimbState::OnEnter(Player * object)
{
	object->SetGraphics(L"ladderClimb");
	object->_velocity = Vector2();
	object->_accel.x = 0.0f;
	object->_stateClimbing = true;
	object->desiredPosition.tileRel.x = 32.0f;
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
	object->_stateClimbing = false;
}
