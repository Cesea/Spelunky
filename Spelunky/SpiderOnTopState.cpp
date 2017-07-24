#include "stdafx.h"
#include "SpiderOnTopState.h"

#include "Spider.h"

#include "SpiderOnGroundState.h"

void SpiderOnTopState::OnEnter(Spider * object)
{
	object->SetGraphics(L"flip");
}

State<Spider>* SpiderOnTopState::Update(Spider * object, float deltaTime)
{
	State<Spider> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_velocity += object->_accel * deltaTime;
	ClampFloat(&object->_velocity.x, -object->_maxVelocity.x, object->_maxVelocity.x);
	ClampFloat(&object->_velocity.y, -object->_maxVelocity.y, object->_maxVelocity.y);

	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (object->_onGround)
	{
		newState = new SpiderOnGroundState;
	}
	return newState;
}

State<Spider>* SpiderOnTopState::HandleCommand(Spider * object, const ControlCommand & command)
{
	return nullptr;
}

State<Spider>* SpiderOnTopState::HandleFrameEndEvent(Spider * actor)
{
	return nullptr;
}

void SpiderOnTopState::OnExit(Spider * object)
{
}
