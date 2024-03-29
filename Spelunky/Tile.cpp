#include "stdafx.h"

Tile::Tile(ObjectId id)
	:CollidingObject(id)
{
}

Tile::~Tile()
{
}

HRESULT Tile::Init(BaseProperty * property)
{
	TileProperty *convertedProperty = (TileProperty *)(property);
	*this = convertedProperty;
	return S_OK;
}

void Tile::Release(void)
{
}

void Tile::Update(float deltaTime)
{
}

void Tile::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	const IntVector2 &tileIndex = sourceIndex;
	if (tileIndex.x != -1)
	{
		//Console::Log("%d, %d\n", position.tileX, position.tileY);
		sprite->FrameRender(gRenderTarget, position.tileX * TILE_SIZE - camPos.x,
			position.tileY * TILE_SIZE - camPos.y, tileIndex.x, tileIndex.y);

		for (int i = 0; i < 4; ++i)
		{
			if (spriteMaskInfo[i].hasMask)
			{
				if (i == 0)
				{
					spriteMaskInfo[i].maskSprite->FrameRender(gRenderTarget, position.tileX * TILE_SIZE - camPos.x,
						position.tileY * TILE_SIZE - TILE_SIZE_HALF - camPos.y, 5, 0);
				}
				else if (i == 1)
				{
					spriteMaskInfo[i].maskSprite->FrameRender(gRenderTarget, position.tileX * TILE_SIZE - TILE_SIZE_HALF- camPos.x,
						position.tileY * TILE_SIZE - camPos.y, 7, 2);
				}
				else if (i == 2)
				{
					spriteMaskInfo[i].maskSprite->FrameRender(gRenderTarget, position.tileX * TILE_SIZE + TILE_SIZE_HALF - camPos.x,
						position.tileY * TILE_SIZE - camPos.y, 7, 1);
				}
				else if (i == 3)
				{
					spriteMaskInfo[i].maskSprite->FrameRender(gRenderTarget, position.tileX * TILE_SIZE - camPos.x,
						position.tileY * TILE_SIZE + TILE_SIZE_HALF - camPos.y, 5, 1);
				}
			}
		}

	}
}

GameObject * Tile::Copy(ObjectId id)
{
	Tile *result = new Tile(id);
	result = this;
	return result;
}

Tile & Tile::operator=(TileProperty * other)
{
	sprite = other->sprite;
	memcpy(spriteMaskInfo, other->spriteMaskInfo, sizeof(MaskInfo) * 4);
	sourceIndex = other->sourceIndex;
	destroyedIndex = other->destroyedIndex;
	thisMaskInfo = other->thisMaskInfo;
	nearMaskInfo = other->nearMaskInfo;
	collisionType = other->collisionType;
	maskInfo = other->maskInfo;
	canBeDestroyedByBomb = other->canBeDestroyedByBomb;
	layer = other->layer;

	position.tileX = other->position.x;
	position.tileY = other->position.y;
	return *this;
}

Tile & Tile::operator=(const Tile & other)
{
	sprite = other.sprite;
	memcpy(spriteMaskInfo, other.spriteMaskInfo, sizeof(MaskInfo) * 4);
	sourceIndex = other.sourceIndex;
	destroyedIndex = other.destroyedIndex;
	thisMaskInfo = other.thisMaskInfo;
	nearMaskInfo = other.nearMaskInfo;
	collisionType = other.collisionType;
	maskInfo = other.maskInfo;
	canBeDestroyedByBomb = other.canBeDestroyedByBomb;
	layer = other.layer;

	position = other.position;
	return *this;
}
