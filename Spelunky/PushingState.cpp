#include "stdafx.h"
#include "PushingState.h"

#include "Player.h"

#include "FallingState.h"
#include "WalkState.h"
#include "IdleState.h"

void PushingState::OnEnter(Player * object)
{
	object->SetGraphics(L"pushing");
	object->_pushingObject = true;
	_initialDirection = object->GetDirection();
	SOUNDMANAGER->Play(L"push_block");
}

State<Player>* PushingState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	if (object->_onObject)
	{
		object->_accel.y -= GRAVITY * 0.99;
	}

	object->_velocity += object->_accel * deltaTime;
	ClampFloat(&object->_velocity.x, -object->_maxVelocity.x, object->_maxVelocity.x);
	ClampFloat(&object->_velocity.y, -object->_maxVelocity.y, object->_maxVelocity.y);

	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (!object->_onGround)
	{
		newState = new FallingState;
	}
	if (object->GetDirection() != _initialDirection)
	{
		newState = new WalkState;
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
		}
	}

	return newState;
}

State<Player>* PushingState::HandleCommand(Player * object, const ControlCommand & command)
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
	return newState;
}

void PushingState::OnExit(Player * object)
{
	object->_pushingObject = false;
	SOUNDMANAGER->Stop(L"push_block");
}
