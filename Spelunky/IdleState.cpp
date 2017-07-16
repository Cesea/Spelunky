#include "stdafx.h"
#include "IdleState.h"

#include "Player.h"

#include "FallingState.h"
#include "WalkState.h"
#include "LookUpState.h"
#include "CrawlState.h"

#include "JumpState.h"

#include "LadderClimbState.h"

void IdleState::OnEnter(Player * object)
{
	object->SetGraphics(L"idle");
	object->_velocity = Vector2();
}

State<Player>* IdleState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	if (!object->_onGround)
	{
		newState = new FallingState;
	}

	object->_velocity += object->_accel * deltaTime;
	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);
	return newState;
}

State<Player>* IdleState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	//WalkState로의 이전 처리
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

	if (command.jump == Command::Jump)
	{
		newState = new JumpState;
		return newState;
	}
	//CrawlState로의 이전 처리
	if (command.vertical == Command::MoveDown)
	{
		newState = new CrawlState;
		return newState;
	}
	//LookUpState로의 이전 처리
	else if (command.vertical == Command::MoveUp)
	{
		if (object->_canClimb)
		{
			newState = new LadderClimbState;
			return newState;
		}
		else
		{
			newState = new LookUpState;
			return newState;
		}
	}
	return nullptr;
}

void IdleState::OnExit(Player * object)
{
}
