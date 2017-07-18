#include "stdafx.h"
#include "LadderClimbState.h"

#include "Player.h"

#include "IdleState.h"
#include "JumpState.h"
#include "FallingState.h"

#include "AttackState.h"

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

	if (command.horizontal == Command::MoveLeft)
	{
		if (object->_seeingDirection == Direction::Right)
		{
			object->_seeingDirection = Direction::Left;
		}
	}
	else if (command.horizontal == Command::MoveRight)
	{
		if (object->_seeingDirection == Direction::Left)
		{
			object->_seeingDirection = Direction::Right;
		}
	}

	if (command.vertical == Command::MoveUp)
	{
		if (object->_canClimbUp)
		{
			object->_accel.y -= object->_speed.y;
			newState = new LadderClimbState;
			return newState;
		}
		if (command.jump == Command::Jump)
		{
			newState = new JumpState;
			return newState;
		}
	}
	else if(command.vertical == Command::MoveDown)
	{
		if (command.jump == Command::Jump)
		{
			newState = new FallingState;
			object->_stateClimbing = false;
			return newState;
		}

		object->_accel.y -= object->_speed.y;
		newState = new LadderClimbState;

	}

	if (command.jump == Command::Jump)
	{
		newState = new JumpState;
		object->_stateClimbing = false;
		return newState;
	}

	if (command.action == Command::Attack)
	{
		if (object->_holding)
		{
			newState = new ThrowState;
			return newState;
		}
		else
		{
			newState = new AttackState;
			return newState;
		}
	}

	return newState;
}

void LadderIdleState::OnExit(Player * object)
{
	//object->_stateClimbing = false;
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

	if (command.horizontal == Command::MoveLeft)
	{
		if (object->_seeingDirection == Direction::Right)
		{
			object->_seeingDirection = Direction::Left;
		}
	}
	else if (command.horizontal == Command::MoveRight)
	{
		if (object->_seeingDirection == Direction::Left)
		{
			object->_seeingDirection = Direction::Right;
		}
	}

	if (command.vertical == Command::MoveUp)
	{
		if (object->_canClimbUp)
		{
			if (object->GetDirection() == Direction::Down)
			{
				object->_velocity.y = -20.0f;
			}
			_wasControlled = true;
			object->_accel.y = -object->_speed.y;
		}
		if (command.jump == Command::Jump)
		{
			newState = new JumpState;
			object->_stateClimbing = false;
			return newState;
		}
	}
	else if(command.vertical == Command::MoveDown)
	{
		if (object->GetDirection() == Direction::Up)
		{
			object->_velocity.y = 20.0f;
		}
		_wasControlled = true;
		object->_accel.y = object->_speed.y;
		if (command.jump == Command::Jump)
		{
			newState = new FallingState;
			object->_stateClimbing = false;
			return newState;
		}
	}

	if (command.action == Command::Attack)
	{
		if (object->_holding)
		{
			newState = new ThrowState;
			return newState;
		}
		else
		{
			newState = new AttackState;
			return newState;
		}
	}
	return newState;
}

void LadderClimbState::OnExit(Player * object)
{
}
