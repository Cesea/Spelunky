#include "stdafx.h"
#include "AttackState.h"

#include "Player.h"

#include "WalkState.h"


void AttackState::OnEnter(Player * object)
{
	object->SetGraphics(L"attack");
}

State<Player>* AttackState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_velocity += object->_accel * deltaTime;
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

	return nullptr;
}

State<Player>* AttackState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	if (command.horizontal == Command::MoveLeft)
	{
		_wasControlled = true;
		if (object->GetDirection() != Direction::Left)
		{
			object->SetDirection(Direction::Left);
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
			object->_velocity.x = 40.0f;
		}
		else
		{
			object->_accel.x = object->_speed.x;
		}
	}
	if (command.jump == Command::Jump)
	{
		object->_velocity.y = -540;
	}
	return newState;
}

State<Player>* AttackState::HandleFrameEndEvent(Player * object)
{
	return object->_stateManager.GetPrevState();
}

void AttackState::OnExit(Player * object)
{
}

void ThrowState::OnEnter(Player * object)
{
}

State<Player>* ThrowState::Update(Player * object, float deltaTime)
{
	return nullptr;
}

State<Player>* ThrowState::HandleCommand(Player * object, const ControlCommand & command)
{
	return nullptr;
}

State<Player>* ThrowState::HandleFrameEndEvent(Player * actor)
{
	return nullptr;
}

void ThrowState::OnExit(Player * object)
{
}
