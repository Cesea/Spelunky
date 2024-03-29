#include "stdafx.h"
#include "SnakeWalkState.h"

#include "Snake.h"

void SnakeWalkState::OnEnter(Snake * object)
{
	object->SetGraphics(L"walk");
}

State<Snake>* SnakeWalkState::Update(Snake * object, float deltaTime)
{
	if (object->GetDirection() == Direction::Left)
	{
		_wasControlled = true;
		object->_accel.x = -object->_speed.x;
	}
	else if (object->GetDirection() == Direction::Right)
	{
		_wasControlled = true;
		object->_accel.x = object->_speed.x;
	}

	State<Snake> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	object->_velocity += object->_accel * deltaTime;
	ClampFloat(&object->_velocity.x, -object->_maxVelocity.x, object->_maxVelocity.x);
	ClampFloat(&object->_velocity.y, -object->_maxVelocity.y, object->_maxVelocity.y);

	object->desiredPosition.AddToTileRel(object->_velocity * deltaTime);

	if (_wasControlled)
	{
		_wasControlled = false;
	}

	TileCollisionType lowerLeftColType = (object->_nearTiles.tiles[6]) ? 
		object->_nearTiles.tiles[6]->collisionType : TileCollisionType::TILE_COLLISION_NONE;
	TileCollisionType lowerRightColType = (object->_nearTiles.tiles[4]) ? 
		object->_nearTiles.tiles[4]->collisionType : TileCollisionType::TILE_COLLISION_NONE;

	TileCollisionType leftColType = (object->_nearTiles.tiles[2]) ? 
		object->_nearTiles.tiles[2]->collisionType : TileCollisionType::TILE_COLLISION_NONE;
	TileCollisionType rightColType = (object->_nearTiles.tiles[3]) ? 
		object->_nearTiles.tiles[3]->collisionType : TileCollisionType::TILE_COLLISION_NONE;

	if (object->GetDirection() == Direction::Left)
	{
		if ((lowerLeftColType != TILE_COLLISION_BLOCK && lowerRightColType != TILE_COLLISION_BLOCK) ||
			(leftColType == TILE_COLLISION_BLOCK && rightColType == TILE_COLLISION_BLOCK))
		{
			object->_velocity.x = 0;
		}
		else if (((leftColType == TILE_COLLISION_BLOCK) && (object->position.tileRel.x > 24 && object->position.tileRel.x < 40)) ||
			(lowerLeftColType != TILE_COLLISION_BLOCK) && (object->position.tileRel.x > 24 && object->position.tileRel.x < 40))
		{
			object->SetDirection(Direction::Right);
			object->GetCurrentGraphics()->SyncFlip(Direction::Right);
			object->_velocity.x = 20.0f;
		}
	}
	else if(object->GetDirection() == Direction::Right)
	{
		if ((lowerLeftColType != TILE_COLLISION_BLOCK && lowerRightColType != TILE_COLLISION_BLOCK) ||
			(leftColType == TILE_COLLISION_BLOCK && rightColType == TILE_COLLISION_BLOCK) )
		{
			object->_velocity.x = 0;
		}
		else if (((rightColType == TILE_COLLISION_BLOCK) && (object->position.tileRel.x > 24 && object->position.tileRel.x < 40))  ||
			((lowerRightColType != TILE_COLLISION_BLOCK) && (object->position.tileRel.x > 24 && object->position.tileRel.x < 40)))
		{
			object->SetDirection(Direction::Left);
			object->GetCurrentGraphics()->SyncFlip(Direction::Left);
			object->_velocity.x = -20.0f;
		}
	}
	return newState;
}



State<Snake>* SnakeWalkState::HandleCommand(Snake * object, const ControlCommand & command)
{
	return nullptr;
}

State<Snake>* SnakeWalkState::HandleFrameEndEvent(Snake * actor)
{
	return nullptr;
}

void SnakeWalkState::OnExit(Snake * object)
{
}
