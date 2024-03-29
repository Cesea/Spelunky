#include "stdafx.h"
#include "CrawlMoveState.h"

#include "Player.h"
#include "StandUpState.h"
#include "FallingState.h"

#include "CrawlState.h"

#include "OnLedgeState.h"

void CrawlMoveState::OnEnter(Player * object)
{
	object->SetGraphics(L"crawlMove");
}

State<Player>* CrawlMoveState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);


if (object->_onObject)
	{
		object->_accel.y -= GRAVITY * 0.99;
	}

	object->_velocity += object->_accel * deltaTime;
	ClampFloat(&object->_velocity.x, -object->_maxVelocity.x * 0.2f, object->_maxVelocity.x * 0.2f);
	ClampFloat(&object->_velocity.y, -object->_maxVelocity.y * 0.2f, object->_maxVelocity.y * 0.2f);

	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (!object->_onGround)
	{
		newState = new FallingState;
	}

	if (_wasControlled)
	{
		_wasControlled = false;
	}
	else
	{
		if (object->_velocity.x > EPSILON || object->_velocity.x < -EPSILON)
		{
			object->_velocity.x *= 0.2f;
		}
		else
		{
			object->_velocity.x = 0.0f;
			newState = new CrawlIdleState;
		}
	}
	return newState;
}

State<Player>* CrawlMoveState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	if (command.horizontal == Command::MoveLeft)
	{
		_wasControlled = true;
		if (object->GetDirection() != Direction::Left)
		{
			object->SetDirection(Direction::Left);
			object->GetCurrentGraphics()->SyncFlip(Direction::Left);
			object->_velocity.x = -20.0f;
		}
		else
		{
			object->_accel.x = -object->_speed.x * 0.5f;
			if (object->_onLedge)
			{
				newState = new LedgeGrabState;
				return newState;
			}
		}
	}
	else if (command.horizontal == Command::MoveRight)
	{
		_wasControlled = true;
		if (object->GetDirection() != Direction::Right)
		{
			object->SetDirection(Direction::Right);
			object->GetCurrentGraphics()->SyncFlip(Direction::Right);
			object->_velocity.x = 20.0f;
		}
		else
		{
			object->_accel.x = object->_speed.x * 0.5f;
			if (object->_onLedge)
			{
				newState = new LedgeGrabState;
				return newState;
			}
		}
	}	

	if (command.vertical == Command::None)
	{
		newState = new StandUpState;
	}
	return newState;
}

void CrawlMoveState::OnExit(Player * object)
{
}
