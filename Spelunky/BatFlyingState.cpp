#include "stdafx.h"
#include "BatFlyingState.h"

#include "Bat.h"

void BatFlyingState::OnEnter(Bat * object)
{
	object->SetGraphics(L"fly");
}

State<Bat>* BatFlyingState::Update(Bat * object, float deltaTime)
{
	State<Bat> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_velocity += object->_accel * deltaTime;
	ClampFloat(&object->_velocity.x, -object->_maxVelocity.x, object->_maxVelocity.x);
	ClampFloat(&object->_velocity.y, -object->_maxVelocity.y, object->_maxVelocity.y);

	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	return nullptr;
}

State<Bat>* BatFlyingState::HandleCommand(Bat * object, const ControlCommand & command)
{
	return nullptr;
}

State<Bat>* BatFlyingState::HandleFrameEndEvent(Bat * actor)
{
	return nullptr;
}

void BatFlyingState::OnExit(Bat * object)
{
}
