#include "stdafx.h"
#include "SpiderJumpState.h"

#include "Spider.h"

#include "SpiderOnGroundState.h"

void SpiderJumpState::OnEnter(Spider * object)
{
	object->SetGraphics(L"jump");
	object->_velocity.y -= object->_speed.y;
}

State<Spider>* SpiderJumpState::Update(Spider * object, float deltaTime)
{
	if (object->_seeingDirection == Direction::Left)
	{
		object->_accel.x -= object->_speed.x;
	}
	else if (object->_seeingDirection == Direction::Right)
	{
		object->_accel.x += object->_speed.x;
	}

	State<Spider> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_velocity += object->_accel * deltaTime;
	ClampFloat(&object->_velocity.x, -object->_maxVelocity.x, object->_maxVelocity.x);
	ClampFloat(&object->_velocity.y, -object->_maxVelocity.y, object->_maxVelocity.y);

	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (object->_onGround && object->_velocity.y > 0)
	{
		newState = new SpiderOnGroundState;
	}
	return newState;
}

State<Spider>* SpiderJumpState::HandleCommand(Spider * object, const ControlCommand & command)
{
	return nullptr;
}

State<Spider>* SpiderJumpState::HandleFrameEndEvent(Spider * actor)
{
	return nullptr;
}

void SpiderJumpState::OnExit(Spider * object)
{
	object->_velocity.x = 0;
}
