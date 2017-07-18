#include "stdafx.h"
#include "AttackState.h"

#include "Player.h"

#include "WalkState.h"
#include "LadderClimbState.h"
#include "FallingState.h"


void AttackState::OnEnter(Player * object)
{
	object->SetGraphics(L"attack");
}

State<Player>* AttackState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	if (object->_stateClimbing)
	{
		object->_accel.y -= GRAVITY;
	}

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
	if (object->_stateClimbing)
	{
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
		}
		else if (command.vertical == Command::MoveDown)
		{
			if (object->GetDirection() == Direction::Up)
			{
				object->_velocity.y = 20.0f;
			}
			_wasControlled = true;
			object->_accel.y = object->_speed.y;
		}
		if (command.jump == Command::Jump)
		{
			object->_velocity.y = -540;
		}
	}
	else
	{
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
	}
	return newState;
}

State<Player>* AttackState::HandleFrameEndEvent(Player * object)
{
	State<Player> *newState = nullptr;
	if (object->_onGround)
	{
		newState = new WalkState;
		return newState;
	}
	else if (object->_stateClimbing)
	{
		newState = new LadderClimbState;
		return newState;
	}
	else if (!object->_onGround)
	{
		newState = new FallingState;
		return newState;
	}
	return newState;
}

void AttackState::OnExit(Player * object)
{
}

void ThrowState::OnEnter(Player * object)
{
	object->SetGraphics(L"throw");
	object->_holding = false;
}

State<Player>* ThrowState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	if (object->_stateClimbing)
	{
		object->_accel.y -= GRAVITY;
	}

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

State<Player>* ThrowState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;

	if (object->_stateClimbing)
	{
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
		}
		else if (command.vertical == Command::MoveDown)
		{
			if (object->GetDirection() == Direction::Up)
			{
				object->_velocity.y = 20.0f;
			}
			_wasControlled = true;
			object->_accel.y = object->_speed.y;
		}
		if (command.jump == Command::Jump)
		{
			object->_velocity.y = -540;
		}
	}
	else
	{
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
	}
	return newState;
}

State<Player>* ThrowState::HandleFrameEndEvent(Player * object)
{
	State<Player> *newState = nullptr;
	if (object->_onGround)
	{
		newState = new WalkState;
		return newState;
	}
	else if (object->_stateClimbing)
	{
		newState = new LadderClimbState;
		return newState;
	}
	else if (!object->_onGround)
	{
		newState = new FallingState;
		return newState;
	}
	return newState;
}

void ThrowState::OnExit(Player * object)
{
}
