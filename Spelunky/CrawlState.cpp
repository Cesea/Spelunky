#include "stdafx.h"
#include "CrawlState.h"

#include "Player.h"

#include "CrawlMoveState.h"
#include "StandUpState.h"
#include "LookUpState.h"

void CrawlState::OnEnter(Player * object)
{
	object->SetGraphics(L"crawl");
}

State<Player>* CrawlState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_velocity += object->_accel * deltaTime;
	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	return nullptr;
}

State<Player>* CrawlState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	return newState;
}

State<Player>* CrawlState::HandleFrameEndEvent(Player * actor)
{
	State<Player> *newState = nullptr;
	newState = new CrawlIdleState;
	return newState;
}

void CrawlState::OnExit(Player * object)
{
}

void CrawlIdleState::OnEnter(Player * object)
{
	object->SetGraphics(L"crawlIdle");
}

State<Player>* CrawlIdleState::Update(Player * object, float deltaTime)
{
	object->_velocity += object->_accel * deltaTime;
	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);
	return nullptr;
}

State<Player>* CrawlIdleState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	if (command.horizontal != Command::None)
	{
		if (command.horizontal == Command::MoveLeft)
		{
			object->SetDirection(Direction::Left);
			object->_accel.x = -object->_speed.x * 0.5f;
		}
		else if (command.horizontal == Command::MoveRight)
		{
			object->SetDirection(Direction::Right);
			object->_accel.x = object->_speed.x * 0.5f;
		}
		newState = new CrawlMoveState;
		return newState;
	}

	if (command.vertical == Command::MoveUp)
	{
		newState = new StandUpState;
	}
	else if (command.vertical == Command::None)
	{
		newState = new StandUpState;
	}
	return newState;
}

void CrawlIdleState::OnExit(Player * object)
{
}
