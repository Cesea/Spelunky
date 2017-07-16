#include "stdafx.h"
#include "OnLedgeState.h"

#include "Player.h"

#include "WalkState.h"
#include "FallingState.h"
#include "JumpState.h"

void OnLedgeState::OnEnter(Player * object)
{
	object->SetGraphics(L"onLedge");
	_enteredPosition = object->position;
}

State<Player>* OnLedgeState::Update(Player * object, float deltaTime)
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
		return newState;
	}

	if ((object->position.tileRel.x >= 8 && object->position.tileRel.x <= 56) &&
		(object->position.tileX == _enteredPosition.tileX))
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
		}
	}
	return newState;
}

State<Player>* OnLedgeState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	if (command.horizontal == Command::MoveLeft)
	{
		_wasControlled = true;
		if (object->GetDirection() != Direction::Left)
		{
			object->SetDirection(Direction::Left);
			object->GetCurrentGraphics()->SyncFlip(Direction::Left);
			object->_velocity.x = -70.0f;
			newState = new WalkState;
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
			object->_velocity.x = 70.0f;
			newState = new WalkState;
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

	return newState;
}

void OnLedgeState::OnExit(Player * object)
{
}

void LedgeGrabState::OnEnter(Player * object)
{
	object->_interpolating = true;
	object->SetGraphics(L"ledgeGrab");
	_startPosition = object->position;
	if (object->GetDirection() == Direction::Left)
	{
		_endPosition = _startPosition;
		_endPosition.AddToTileRel(-28, 52);
	}
	else if(object->GetDirection() == Direction::Right)
	{
		_endPosition = _startPosition;
		_endPosition.AddToTileRel(+28, 52);
	}

	_timer.Init(0.4f);
}

State<Player>* LedgeGrabState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	if (_timer.Tick(deltaTime))
	{
		_stop = true;
	}

	if (!_stop)
	{
		float t = _timer.GetCurrentSecond() / _timer.GetTargetSecond();
		object->position = InterpolateTilePosition(_startPosition, _endPosition, t);
	}
	return nullptr;
}

State<Player>* LedgeGrabState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	if (command.jump == Command::Jump)
	{
		newState = new JumpState;
		return newState;
	}
	return newState;
}

void LedgeGrabState::OnExit(Player * object)
{
	object->desiredPosition = object->position;
	object->_interpolating = false;
}
