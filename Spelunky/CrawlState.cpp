#include "stdafx.h"
#include "CrawlState.h"

#include "Player.h"

#include "CrawlMoveState.h"
#include "StandUpState.h"
#include "LookUpState.h"
#include "FallingState.h"

void CrawlState::OnEnter(Player * object)
{
	object->SetGraphics(L"crawl");
	object->_crawling = true;
}

State<Player>* CrawlState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);


	if (object->_onObject)
	{
		object->_accel.y -= GRAVITY * 0.99;
	}

	object->_velocity += object->_accel * deltaTime;
	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (!object->_onGround)
	{
		newState = new FallingState;
	}

	return newState;
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

	_cameraMoveTimer.Init(1.5f);
}

State<Player>* CrawlIdleState::Update(Player * object, float deltaTime)
{
	if (object->_onObject)
	{
		object->_accel.y -= GRAVITY * 0.99;
	}

	object->_velocity += object->_accel * deltaTime;
	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (_cameraMoveTimer.Tick(deltaTime) && !_didFired)
	{
		EVENTMANAGER->QueueEvent(new CameraMoveToEvent(object->position, Direction::Down, false));
		_didFired = true;
	}
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
	if (command.action == Command::Attack)
	{
		if (object->_holding)
		{
			ObjectId putDownId = UNVALID_OBJECT_ID;
			if (object->_holdingObject[0])
			{
				putDownId = object->_holdingObject[0]->GetId();
				object->_holdingObject[0] = nullptr;
			}
			EVENTMANAGER->QueueEvent(new PutDownEvent(putDownId, object->_seeingDirection));
			SOUNDMANAGER->Play(L"item_drop");
			object->_holding = false;
		}
		else
		{
			EVENTMANAGER->QueueEvent(new PickupEvent(object->_id));
		}
	}
	else if (command.action == Command::UseBomb)
	{
		if (object->_bomb > 0)
		{

			TilePosition bombThrowPosition = object->position;
			bombThrowPosition.AddToTileRelY(-16);

			Vector2 direction;
			if (object->GetDirection() == Direction::Left)
			{
				direction.x = -200;
				direction.y = -30;
			}
			else if (object->GetDirection() == Direction::Right)
			{
				direction.x = 200;
				direction.y = -30;
			}
			direction.Normalize();
			direction *= 150.0f;
			EVENTMANAGER->QueueEvent(new ThrowBombEvent(bombThrowPosition, direction, object->_stickyBomb));
			object->_bomb--;
		}
	}
	return newState;
}

void CrawlIdleState::OnExit(Player * object)
{
	if (_didFired)
	{
		EVENTMANAGER->QueueEvent(new CameraMoveToEvent(object->position, Direction::Up, true));
	}
}
