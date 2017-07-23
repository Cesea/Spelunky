#include "stdafx.h"
#include "StandUpState.h"

#include "Player.h"

#include "IdleState.h"

void StandUpState::OnEnter(Player * object)
{
	object->SetGraphics(L"standUp");
}

State<Player>* StandUpState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_velocity += object->_accel * deltaTime;
	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	return nullptr;
}

State<Player>* StandUpState::HandleCommand(Player * object, const ControlCommand & command)
{
	return nullptr;
}

State<Player> * StandUpState::HandleFrameEndEvent(Player * actor)
{
	State<Player> *newState = new IdleState;
	return newState;
}

void StandUpState::OnExit(Player * object)
{
	object->_crawling = false;
}
