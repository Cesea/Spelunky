#include "stdafx.h"
#include "GameCommonStructs.h"

TilePosition & TilePosition::operator=(const TilePosition & other)
{
	tileX = other.tileX;
	tileY = other.tileY;
	tileRel = other.tileRel;
	return *this;
}

void TilePosition::AddToTileRel(float x, float y)
{
	tileRel.x += x;
	tileRel.y += y;
	ReTilelize();
}

void TilePosition::AddToTileRelX(float f)
{
	tileRel.x += f;
	ReTilelizeX();
}

void TilePosition::AddToTileRelY(float f)
{
	tileRel.y += f;
	ReTilelizeY();
}

//움직임 클리핑을 어떻게 처리 할 것인가...
bool TilePosition::ReTilelize()
{
	bool xResult = ReTilelizeX();
	bool yResult = ReTilelizeY();
	return (xResult && yResult);
}

bool TilePosition::ReTilelizeX()
{
	bool result = false;
	if (tileRel.x < 0)
	{
		tileX -= 1;
		tileRel.x += TILE_SIZE;
		result = true;
	}
	else if (tileRel.x >= TILE_SIZE)
	{
		tileX += 1;
		tileRel.x -= TILE_SIZE;
		result = true;
	}
	return result;
}

bool TilePosition::ReTilelizeY()
{
	bool result = false;
	if (tileRel.y < 0)
	{
		tileY -= 1;
		tileRel.y += TILE_SIZE;
		result = true;
	}
	else if (tileRel.y >= TILE_SIZE)
	{
		tileY += 1;
		tileRel.y -= TILE_SIZE;
		result = true;
	}
	return result;
}

TileSet::TileSet(uint32 countX, uint32 countY, int32 clearValue)
{
	this->countX = countX;
	this->countY = countY;
	tiles = new uint32[countX * countY];
}

TileSet::~TileSet()
{
	delete[] tiles;
}

void TileSet::Clear(int32 value)
{
	for (int i = 0; i < countX * countY; ++i)
	{
		tiles[i] = value;
	}
}

int32 TileSet::GetValue(uint32 x, uint32 y)
{
	int32 result{};
	result = tiles[y * countX + x];
	return result;
}

void TileSet::SetValue(uint32 x, uint32 y, int32 value)
{
	tiles[y * countX + x] = value;
}

uint32 & TileSet::At(uint32 x, uint32 y)
{
	return tiles[y * countX + x];
}

uint32 & TileSet::At(uint32 i)
{
	return tiles[i];
}
