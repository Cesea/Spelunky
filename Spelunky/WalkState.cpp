#include "stdafx.h"
#include "WalkState.h"

#include "Player.h"

#include "IdleState.h"
#include "FallingState.h"
#include "JumpState.h"

#include "LadderClimbState.h"

#include "OnLedgeState.h"
#include "AttackState.h"

void WalkState::OnEnter(Player * object)
{
	object->SetGraphics(L"walk");
}

State<Player>* WalkState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_velocity += object->_accel * deltaTime;
	ClampFloat(&object->_velocity.x, -object->_maxVelocity.x, object->_maxVelocity.x);
	ClampFloat(&object->_velocity.y, -object->_maxVelocity.y, object->_maxVelocity.y);

	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (!object->_onGround)
	{
		newState = new FallingState;
	}

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
			newState = new IdleState;
			Console::Log("Im Idle\n");
		}
	}
	return newState;
}

State<Player>* WalkState::HandleCommand(Player * object, const ControlCommand & command)
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

	if (command.jump == Command::Jump)
	{
		newState = new JumpState;
		return newState;
	}

	if (object->_canClimb && 
		command.vertical != Command::None)
	{
		newState = new LadderClimbState;
		return newState;
	}
	if (command.action == Command::Attack || command.action == Command::Dig)
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
	else if (command.action == Command::UseBomb)
	{
		TilePosition bombThrowPosition = object->position;
		bombThrowPosition.AddToTileRelY(-32);

		Vector2 direction;
		if (object->GetDirection() == Direction::Left)
		{
			direction.x = -900;
			direction.y = -600;
		}
		else if (object->GetDirection() == Direction::Right)
		{
			direction.x = 900;
			direction.y = -600;
		}
		direction.Normalize();
		direction *= 800.0f;
		EVENTMANAGER->QueueEvent(new ThrowBombEvent(bombThrowPosition, direction, object->_stickyBomb));
	}

	return newState;
}

void WalkState::OnExit(Player * object)
{
}
