#include "stdafx.h"
#include "LookUpState.h"

#include "Player.h"

#include "LookRevertState.h"
#include "WalkState.h"

void LookUpState::OnEnter(Player * object)
{
	object->SetGraphics(L"lookUp");
	_cameraMoveTimer.Init(1.5f);
}

State<Player>* LookUpState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	if (object->_onObject)
	{
		object->_accel.y -= GRAVITY * 0.99;
	}


	if (_cameraMoveTimer.Tick(deltaTime) && !_didFired)
	{
		EVENTMANAGER->QueueEvent(new CameraMoveToEvent(object->position, Direction::Up, false));
		_didFired = true;
	}

	return nullptr;
}
State<Player>* LookUpState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	if (command.horizontal != Command::None)
	{
		if (command.horizontal == Command::MoveLeft)
		{
			object->SetDirection(Direction::Left);
			object->_accel.x = -object->_speed.x;
		}
		else if (command.horizontal == Command::MoveRight)
		{
			object->SetDirection(Direction::Right);
			object->_accel.x = object->_speed.x;
		}
		newState = new WalkState;
		return newState;
	}

	if (command.vertical == Command::None)
	{
		newState = new LookRevertState;
	}
	return newState;
}

void LookUpState::OnExit(Player * object)
{
	if (_didFired)
	{
		EVENTMANAGER->QueueEvent(new CameraMoveToEvent(object->position, Direction::Down, true));
	}
}
