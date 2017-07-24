#include "stdafx.h"
#include "SpiderOnGroundState.h"

#include "Spider.h"

#include "SpiderJumpState.h"

void SpiderOnGroundState::OnEnter(Spider * object)
{
	object->SetGraphics(L"ground");
	_jumpTimer.Init(2.0f);
}

State<Spider>* SpiderOnGroundState::Update(Spider * object, float deltaTime)
{
	State<Spider> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_velocity += object->_accel * deltaTime;
	ClampFloat(&object->_velocity.x, -object->_maxVelocity.x, object->_maxVelocity.x);
	ClampFloat(&object->_velocity.y, -object->_maxVelocity.y, object->_maxVelocity.y);

	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (_jumpTimer.Tick(deltaTime))
	{
		newState = new SpiderJumpState;
	}

	return newState;
}

State<Spider>* SpiderOnGroundState::HandleCommand(Spider * object, const ControlCommand & command)
{
	return nullptr;
}

State<Spider>* SpiderOnGroundState::HandleFrameEndEvent(Spider * actor)
{
	return nullptr;
}

void SpiderOnGroundState::OnExit(Spider * object)
{
}
