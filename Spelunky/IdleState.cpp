#include "stdafx.h"
#include "IdleState.h"

#include "Player.h"

#include "WalkState.h"
#include "LookUpState.h"
#include "CrawlState.h"

void IdleState::OnEnter(Player * object)
{
	object->SetGraphics(L"idle");
}

State<Player>* IdleState::Update(Player * object, float deltaTime)
{

	return nullptr;
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

	//CrawlState로의 이전 처리
	if (command.vertical == Command::MoveDown)
	{
		newState = new CrawlState;
		return newState;
	}
	//LookUpState로의 이전 처리
	else if (command.vertical == Command::MoveUp)
	{
		newState = new LookUpState;
		return newState;
	}


	//if (command.horizontal == Command::MoveLeft)
	//{
	//	if (object->GetDirection() != Direction::Left)
	//	{
	//		object->SetDirection(Direction::Left);
	//		object->GetCurrentGraphics()->SyncFlip(Direction::Left);
	//	}
	//}
	//else if (command.horizontal == Command::MoveRight)
	//{
	//	if (object->GetDirection() != Direction::Right)
	//	{
	//		object->SetDirection(Direction::Right);
	//		object->GetCurrentGraphics()->SyncFlip(Direction::Right);
	//	}
	//}
	return nullptr;
}

void IdleState::OnExit(Player * object)
{
}
