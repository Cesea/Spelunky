#include "stdafx.h"
#include "ColliisonComponent.h"

#include "MovingObject.h"

void CollisionComponent::Init(const Rect & rect, const Vector2 & offset)
{
	_rect = rect;
	_rectOffset = offset;
}

void CollisionComponent::Update(MovingObject * object, float deltaTime, const ReturnTile * nearTiles)
{
	if (_repulse)
	{
		CollideRepulse(object, deltaTime, nearTiles);
	}
	else
	{
		CollideStop(object, deltaTime, nearTiles);
	}
	
}

void CollisionComponent::CollideRepulse(MovingObject * object, float deltaTime, const ReturnTile * nearTiles)
{
	for (int i = 0; i < 8; ++i)
	{
		if (nearTiles->tiles[i] == nullptr ||
			nearTiles->tiles[i]->sourceIndex.x == -1)
			continue;

		Tile *currentTile = nearTiles->tiles[i];

		Rect absRect = object->desiredPosition.UnTilelize() + _rect + _rectOffset;
		Rect tileRect = RectMake(currentTile->position.tileX * TILE_SIZE, currentTile->position.tileY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

		Rect overlapRect;
		//�Ʒ� Ÿ���� ��� üũ�� �� �־�� �� ��찡 �ִ�. ��ٸ�, ������ �������� �״� Ÿ�� ��....
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
				}
			}
		}
		else
		{
			if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
			{
				if (IsRectangleOverlap(absRect, tileRect, overlapRect))
				{
					//�� Ÿ��
					if (i == 1)
					{
						object->desiredPosition.AddToTileRel(0, overlapRect.height);
						object->_velocity.y = 0.0f;
					}
					//�� Ÿ��
					else if (i == 2)
					{
						object->desiredPosition.AddToTileRel(overlapRect.width, 0);
						object->_velocity.x *= -0.6f;
					}
					//���� Ÿ��
					else if (i == 3)
					{
						object->desiredPosition.AddToTileRel(-overlapRect.width, 0);
						object->_velocity.x *= -0.6f;
					}
					//�밢��
					else
					{
						//�������� �浹�� �Ͼ
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
						}
						//�������� �浹�� �Ͼ
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
						}
					}
				}
			}
		}
	}
	object->position = object->desiredPosition;
}

void CollisionComponent::CollideStop(MovingObject * object, float deltaTime, const ReturnTile * nearTiles)
{
	for (int i = 0; i < 8; ++i)
	{
		if (nearTiles->tiles[i] == nullptr ||
			nearTiles->tiles[i]->sourceIndex.x == -1)
			continue;

		Tile *currentTile = nearTiles->tiles[i];

		Rect absRect = object->desiredPosition.UnTilelize() + _rect + _rectOffset;
		Rect tileRect = RectMake(currentTile->position.tileX * TILE_SIZE, currentTile->position.tileY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

		Rect overlapRect;
		//�Ʒ� Ÿ���� ��� üũ�� �� �־�� �� ��찡 �ִ�. ��ٸ�, ������ �������� �״� Ÿ�� ��....
		if (i == 0)
		{
			if (IsRectangleOverlap(absRect, tileRect, overlapRect))
			{
				if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
				{
					object->desiredPosition.AddToTileRel(0, -overlapRect.height);
					object->_velocity.y = 0.0f;
					object->_onGround = true;
				}
			}
		}
		else
		{
			if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
			{
				if (IsRectangleOverlap(absRect, tileRect, overlapRect))
				{
					//�� Ÿ��
					if (i == 1)
					{
						object->desiredPosition.AddToTileRel(0, overlapRect.height);
						object->_velocity.y = 0.0f;
					}
					//�� Ÿ��
					else if (i == 2)
					{
						object->desiredPosition.AddToTileRel(overlapRect.width, 0);
						object->_velocity.x = 0.0f;
					}
					//���� Ÿ��
					else if (i == 3)
					{
						object->desiredPosition.AddToTileRel(-overlapRect.width, 0);
						object->_velocity.x = 0.0f;
					}
					//�밢��
					else
					{
						//�������� �浹�� �Ͼ
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
						}
						//�������� �浹�� �Ͼ
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
						}
					}
				}
			}
		}
	}
	object->position = object->desiredPosition;
}
