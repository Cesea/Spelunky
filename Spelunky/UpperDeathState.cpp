#include "stdafx.h"
#include "UpperDeathState.h"

#include "Player.h"

void UpperDeathState::OnEnter(Player * object)
{
	object->SetGraphics(L"upperDeath");
	object->_dead = true;
	SOUNDMANAGER->Play(L"spike_hit");
	_deadTimer.Init(4.0f);
}

State<Player>* UpperDeathState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_accel.y -= GRAVITY;
	if (_deadTimer.Tick(deltaTime))
	{
		EVENTMANAGER->QueueEvent(new LayerOnEvent(false, false, object->_dead, object->position));
	}

	return nullptr;
}

State<Player>* UpperDeathState::HandleCommand(Player * object, const ControlCommand & command)
{
	return nullptr;
}

void UpperDeathState::OnExit(Player * object)
{
}
