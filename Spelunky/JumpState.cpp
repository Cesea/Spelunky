#include "stdafx.h"
#include "JumpState.h"

#include "Player.h"

#include "FallingState.h"
#include "LadderClimbState.h"

void JumpState::OnEnter(Player * object)
{
	object->SetGraphics(L"jump");
	object->_velocity.y -= 500.0f;

	object->_onGround = false;
}

State<Player>* JumpState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_prevVelocity = object->_velocity;
	object->_velocity += object->_accel * deltaTime;

	if (object->_prevVelocity.y < 0.0f && object->_velocity.y > 0.0f)
	{
		newState = new FallingState;
	}
	ClampFloat(&object->_velocity.x, -object->_maxVelocity.x, object->_maxVelocity.x);
	ClampFloat(&object->_velocity.y, -object->_maxVelocity.y, object->_maxVelocity.y);

	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);
	if (_wasControlled)
	{
		_wasControlled = false;
	}
	else
	{
		if (object->_velocity.x > EPSILON || object->_velocity.x < -EPSILON)
		{
			object->_velocity.x *= 0.2f;
		}
		else
		{
			object->_velocity.x = 0.0f;
		}
	}

	return newState;
}

State<Player>* JumpState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	if (command.horizontal == Command::MoveLeft)
	{
		_wasControlled = true;
		if (object->GetDirection() != Direction::Left)
		{
			object->SetDirection(Direction::Left);
			object->GetCurrentGraphics()->SyncFlip(Direction::Left);
			object->_velocity.x = -40.0f;
		}
		else
		{
			object->_accel.x = -object->_speed.x;
		}
	}
	else if (command.horizontal == Command::MoveRight)
	{
		_wasControlled = true;
		if (object->GetDirection() != Direction::Right)
		{
			object->SetDirection(Direction::Right);
			object->GetCurrentGraphics()->SyncFlip(Direction::Right);
			object->_velocity.x = 40.0f;
		}
		else
		{
			object->_accel.x = object->_speed.x;
		}
	}

	if (command.jump == Command::JumpOff)
	{
		newState = new FallingState;
		object->_velocity.y = 0.0f;
		return newState;
	}

	if (object->_canClimb && 
		command.vertical == Command::MoveUp)
	{
		newState = new LadderClimbState;
		return newState;
	}

	return newState;
}

void JumpState::OnExit(Player * object)
{
}
