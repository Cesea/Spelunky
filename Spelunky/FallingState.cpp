#include "stdafx.h"
#include "FallingState.h"

#include "Player.h"

#include "IdleState.h"
#include "WalkState.h"
#include "LadderClimbState.h"

#include "GrabState.h"

#include "UpperDeathState.h"
#include "AttackState.h"

void FallingState::OnEnter(Player * object)
{
	object->SetGraphics(L"falling");
	_canGrabAgainTimer.Init(0.15f);
	object->_isFalling = true;
}

State<Player>* FallingState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	if (_canGrabAgainTimer.Tick(deltaTime))
	{
		_canGrabAgain = true;
	}

	if (object->_onGround)
	{
		SOUNDMANAGER->Play(L"land");
		newState = new WalkState;
	}
	if (object->_canGrab)
	{
		newState = new GrabState;
	}
	if (object->_upperDeath && !object->_onGround)
	{
		newState = new UpperDeathState;
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
	return newState;
}

State<Player>* FallingState::HandleCommand(Player * object, const ControlCommand & command)
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
		object->_accel.x = -object->_speed.x;
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
		object->_accel.x = object->_speed.x;
	}

	if (object->_canClimb && 
		command.vertical == Command::MoveUp)
	{
		SOUNDMANAGER->Play(L"grab");
		newState = new LadderClimbState;
		return newState;
	}

//Attack이나 Throw 상태로의 이전 처리
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
		direction *= 700.0f;
		EVENTMANAGER->QueueEvent(new ThrowBombEvent(bombThrowPosition, direction, object->_stickyBomb));
	}

	return newState;
}

void FallingState::OnExit(Player * object)
{
	object->_velocity.y = 0;
	object->_isFalling = false;
}
