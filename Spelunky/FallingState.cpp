#include "stdafx.h"
#include "FallingState.h"

#include "Player.h"

#include "IdleState.h"
#include "WalkState.h"

void FallingState::OnEnter(Player * object)
{
	object->SetGraphics(L"falling");
}

State<Player>* FallingState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	if (object->_onGround)
	{
		newState = new WalkState;
	}

	object->_prevVelocity = object->_velocity;
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

	//if(object->position.tileY == 7 &&)
	return newState;
}

State<Player>* FallingState::HandleCommand(Player * object, const ControlCommand & command)
{
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
	return nullptr;
}

void FallingState::OnExit(Player * object)
{
	object->_velocity.y = 0;
}
