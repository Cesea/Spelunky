#include "stdafx.h"
#include "CollisionComponent.h"

#include "MovingObject.h"

void CollisionComponent::Init(const Rect & rect, const Vector2 & offset)
{
	_rect = rect;
	_rectOffset = offset;
}

bool CollisionComponent::Update(MovingObject * object, float deltaTime, const ReturnTile * nearTiles)
{
	bool result = false;
	if (_repulse)
	{
		result = CollideRepulse(object, deltaTime, nearTiles);
	}
	else
	{
		result = CollideStop(object, deltaTime, nearTiles);
	}
	return result;
}

bool CollisionComponent::CollideRepulse(MovingObject * object, float deltaTime, const ReturnTile * nearTiles)
{
	bool result = false;

	for (int i = 0; i < 8; ++i)
	{
		if (nearTiles->tiles[i] == nullptr ||
			nearTiles->tiles[i]->sourceIndex.x == -1)
			continue;

		Tile *currentTile = nearTiles->tiles[i];

		Rect absRect = object->desiredPosition.UnTilelize() + _rect + _rectOffset;
		Rect tileRect = RectMake(currentTile->position.tileX * TILE_SIZE, currentTile->position.tileY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

		Rect overlapRect;
		//아래 타일의 경우 체크를 해 주어야 할 경우가 있다. 사다리, 위에서 떨어지면 죽는 타일 등....
		if (i == 0)
		{
			if (IsRectangleOverlap(absRect, tileRect, overlapRect))
			{
				if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
				{
					object->desiredPosition.AddToTileRel(0, -overlapRect.height);
					object->_velocity.y *= -0.3f;
					object->_velocity.x *= 0.6;
					object->_onGround = true;
					result = true;
				}
			}
		}
		else
		{
			if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
			{
				if (IsRectangleOverlap(absRect, tileRect, overlapRect))
				{
					//위 타일
					if (i == 1)
					{
						object->desiredPosition.AddToTileRel(0, overlapRect.height);
						object->_velocity.y = 0.0f;
						result = true;
					}
					//왼 타일
					else if (i == 2)
					{
						object->desiredPosition.AddToTileRel(overlapRect.width, 0);
						object->_velocity.x *= -0.6f;
						result = true;
					}
					//오른 타일
					else if (i == 3)
					{
						object->desiredPosition.AddToTileRel(-overlapRect.width, 0);
						object->_velocity.x *= -0.6f;
						result = true;
					}
					//대각선
					else
					{
						//수직으로 충돌이 일어남
						if (overlapRect.width > overlapRect.height)
						{
							object->_velocity.y = 0.0f;
							float pushingHeight;
							if (i == 4 || i == 6)
							{
								pushingHeight = -overlapRect.height;
								object->_onGround = true;
							}
							else
							{
								pushingHeight = overlapRect.height;
							}
							object->desiredPosition.AddToTileRel(0, pushingHeight);
							result = true;
						}
						//수평으로 충돌이 일어남
						else
						{
							object->_velocity.x = 0.0f;
							float pushingWidth;
							if (i == 5 || i == 6)
							{
								pushingWidth = overlapRect.width;
							}
							else
							{
								pushingWidth = -overlapRect.width;
							}
							object->desiredPosition.AddToTileRel(pushingWidth, 0);
							result = true;
						}
					}
				}
			}
		}
	}
	object->position = object->desiredPosition;
	return result;
}

bool CollisionComponent::CollideStop(MovingObject * object, float deltaTime, const ReturnTile * nearTiles)
{
	bool result = false;
	for (int i = 0; i < 8; ++i)
	{
		if (nearTiles->tiles[i] == nullptr ||
			nearTiles->tiles[i]->sourceIndex.x == -1)
			continue;

		Tile *currentTile = nearTiles->tiles[i];

		Rect absRect = object->desiredPosition.UnTilelize() + _rect + _rectOffset;
		Rect tileRect = RectMake(currentTile->position.tileX * TILE_SIZE, currentTile->position.tileY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

		Rect overlapRect;
		//아래 타일의 경우 체크를 해 주어야 할 경우가 있다. 사다리, 위에서 떨어지면 죽는 타일 등....
		if (i == 0)
		{
			if (IsRectangleOverlap(absRect, tileRect, overlapRect))
			{
				if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
				{
					object->desiredPosition.AddToTileRel(0, -overlapRect.height);
					object->_velocity.y = 0.0f;
					object->_onGround = true;
					result = true;
				}
			}
		}
		else
		{
			if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
			{
				if (IsRectangleOverlap(absRect, tileRect, overlapRect))
				{
					//위 타일
					if (i == 1)
					{
						object->desiredPosition.AddToTileRel(0, overlapRect.height);
						object->_velocity.y = 0.0f;
						result = true;
					}
					//왼 타일
					else if (i == 2)
					{
						object->desiredPosition.AddToTileRel(overlapRect.width, 0);
						object->_velocity.x = 0.0f;
						result = true;
					}
					//오른 타일
					else if (i == 3)
					{
						object->desiredPosition.AddToTileRel(-overlapRect.width, 0);
						object->_velocity.x = 0.0f;
						result = true;
					}
					//대각선
					else
					{
						//수직으로 충돌이 일어남
						if (overlapRect.width > overlapRect.height)
						{
							object->_velocity.y = 0.0f;
							float pushingHeight;
							if (i == 4 || i == 6)
							{
								pushingHeight = -overlapRect.height;
								object->_onGround = true;
							}
							else
							{
								pushingHeight = overlapRect.height;
							}
							result = true;
							object->desiredPosition.AddToTileRel(0, pushingHeight);
						}
						//수평으로 충돌이 일어남
						else
						{
							object->_velocity.x = 0.0f;
							float pushingWidth;
							if (i == 5 || i == 6)
							{
								pushingWidth = overlapRect.width;
							}
							else
							{
								pushingWidth = -overlapRect.width;
							}
							object->desiredPosition.AddToTileRel(pushingWidth, 0);
							result = true;
						}
					}
				}
			}
		}
	}
	object->position = object->desiredPosition;
	return result;
}
