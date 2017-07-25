#include "stdafx.h"
#include "FaintState.h"

#include "Player.h"

#include "StandUpState.h"

void FaintState::OnEnter(Player * object)
{
	object->SetGraphics(L"faint");
	_onGroundTimer.Init(1.0f);
	_recoverTimer.Init(2.0f);

	object->_collisionRepulse = true;
	object->_isFaint = true;
	object->_rectOffset.y = -55;
}

State<Player>* FaintState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_velocity += object->_accel * deltaTime;
	ClampFloat(&object->_velocity.x, -object->_maxVelocity.x, object->_maxVelocity.x);
	ClampFloat(&object->_velocity.y, -object->_maxVelocity.y, object->_maxVelocity.y);

	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (object->_onGround)
	{
		if (_onGroundTimer.Tick(deltaTime))
		{
			_recoverTimerOn = true;
		}
	}

	if (_recoverTimerOn)
	{
		if (_recoverTimer.Tick(deltaTime))
		{
			newState = new StandUpState;
		}
	}


	return newState;
}

State<Player>* FaintState::HandleCommand(Player * object, const ControlCommand & command)
{
	return nullptr;
}

void FaintState::OnExit(Player * object)
{
	object->_collisionRepulse = false;
	object->_isFaint = false;
	object->_rectOffset.y = -44;
}
