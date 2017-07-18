#include "stdafx.h"
#include "IdleState.h"

#include "Player.h"

#include "FallingState.h"
#include "WalkState.h"
#include "LookUpState.h"
#include "CrawlState.h"

#include "JumpState.h"

#include "LadderClimbState.h"
#include "OnLedgeState.h"

#include "AttackState.h"

void IdleState::OnEnter(Player * object)
{
	object->SetGraphics(L"idle");
	object->_velocity = Vector2();
}

State<Player>* IdleState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;

	object->_velocity += object->_accel * deltaTime;
	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (!object->_onGround)
	{
		newState = new FallingState;
	}

	if(object->_onLedge)
	{
		newState = new OnLedgeState;
		return newState;
	}
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
		//LadderClimbState로의 이전 처리
		if (object->_canClimb)
		{
			newState = new LadderClimbState;
			return newState;
		}
		else
		{
			newState = new CrawlState;
			return newState;
		}
	}
	else if (command.vertical == Command::MoveUp)
	{
		//LadderClimbState로의 이전 처리
		if (object->_canClimb)
		{
			newState = new LadderClimbState;
			return newState;
		}
		else
		{
			//LookUpState로의 이전 처리
			newState = new LookUpState;
			return newState;
		}
	}
	//Attac이나 Throw 상태로의 이전 처리
	if (command.action == Command::Attack)
	{
		if (object->_holding)
		{
			newState = new ThrowState;
			return newState;
		}
		else
		{
			newState = new AttackState;
			return newState;
		}
	}


	return nullptr;
}

void IdleState::OnExit(Player * object)
{
}
