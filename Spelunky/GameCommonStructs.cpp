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

void TilePosition::AddToTileRel(const Vector2 & v)
{
	tileRel.x += v.x;
	tileRel.y += v.y;
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

//enum TileProperty
//{
//	TILE_PROPERTY_NONE,
//	TILE_PROPERTY_EXIT,
//	TILE_PROPERTY_RANDOM_GEM_SPAWN,
//	TILE_PROPERTY_RANDOM_TILE_SPAWN,
//	TILE_PROPERTY_RANDOM_ITEM_SPAWN,
//	TILE_PROPERTY_RANDOM_ENEMY_SPAWN,
//	TILE_PROPERTY_ENEMY_SPAWN,
//};
//enum ExitType
//{
//	EXIT_STAGE_ONE,
//	EXIT_STAGE_TWO,
//	EXIT_STAGE_TREE,
//};
//enum GemType
//{
//	GEM_GOLD_SMALL,
//	GEM_GOLD_BIG,
//	GEM_EMERALD,
//	GEM_SAPHIRE,
//	GEM_RUBY
//};
//enum ItemType
//{
//	ITEM_GEM_CRATE,
//	ITEM_ITEM_CRATE,
//	ITEM_ROCK,
//	ITEM_JAR,
//};

int ConvertStringToTileProperty(const std::wstring & str)
{
	int result{0};
	if (str == L"exit" || str == L"door")
	{
		result = TILE_PROPERTY_EXIT;
	}
	else if (str == L"randomgem" || str == L"random gem" || str == L"randomGem")
	{
		result = TILE_PROPERTY_RANDOM_GEM_SPAWN;
	}
	else if (str == L"randomtile" || str == L"random tile" || str == L"randomTile")
	{
		result = TILE_PROPERTY_RANDOM_TILE_SPAWN;
	}
	else if (str == L"randomitem" || str == L"random item" || str == L"randomItem")
	{
		result = TILE_PROPERTY_RANDOM_ITEM_SPAWN;
	}
	else if (str == L"randomenemy" || str == L"random enemy" || str == L"randomEnemy")
	{
		result = TILE_PROPERTY_RANDOM_ENEMY_SPAWN;
	}
	else
	{
		result = TILE_PROPERTY_NONE;
	}
	return result;
}

int ConvertStringToTypeByProperty(TileProperty property, const std::wstring &str)
{
	switch (property)
	{
	case TILE_PROPERTY_NONE:
	{

	}break;
	case TILE_PROPERTY_EXIT: 
	{
		if (str == L"stage1" || str == L"stageone" || str == L"one")
		{
			return (int)EXIT_STAGE_ONE;
		}
		else if (str == L"stage2" || str == L"stagetwo" || str == L"two")
		{
			return (int)EXIT_STAGE_TWO;
		}
		else if (str == L"stage3" || str == L"stagethree" || str == L"three")
		{
			return (int)EXIT_STAGE_THREE;
		}
		else
		{
			return (int)EXIT_NONE;
		}
	}break;
	case TILE_PROPERTY_RANDOM_GEM_SPAWN:
	{
		if (str == L"goldsmall" || str == L"small")
		{
			return (int)GEM_GOLD_SMALL;
		}
		else if (str == L"goldbig" || str == L"big")
		{
			return (int)GEM_GOLD_BIG;
		}
		else if (str == L"emerald" || str == L"Emerald")
		{
			return (int)GEM_EMERALD;
		}
		else if (str == L"saphire" || str == L"Saphire")
		{
			return (int)GEM_SAPHIRE;
		}
		else if (str == L"ruby" || str == L"Ruby")
		{
			return (int)GEM_RUBY;
		}
		else
		{
			return (int)GEM_NONE;
		}
	}break;
	case TILE_PROPERTY_RANDOM_TILE_SPAWN: 
	{
	}break;
	case TILE_PROPERTY_RANDOM_ITEM_SPAWN: 
	{
	}break;
	case TILE_PROPERTY_RANDOM_ENEMY_SPAWN: 
	{
	}break;
	case TILE_PROPERTY_ENEMY_SPAWN: 
	{
	}break;
	}

	return 0;
}

std::wstring ConvertTilePropertyToString(const TileProperty property)
{
	std::wstring result = L"";
	switch (property)
	{
	case TILE_PROPERTY_NONE:
	{
	}break;
	case TILE_PROPERTY_EXIT: 
	{
		result = L"exit";
	}break;
	case TILE_PROPERTY_RANDOM_GEM_SPAWN:
	{
		result = L"randomgem";
	}break;
	case TILE_PROPERTY_RANDOM_TILE_SPAWN: 
	{
		result = L"randomtile";
	}break;
	case TILE_PROPERTY_RANDOM_ITEM_SPAWN: 
	{
		result = L"randomitem";
	}break;
	case TILE_PROPERTY_RANDOM_ENEMY_SPAWN: 
	{
		result = L"randomemeny";
	}break;
	case TILE_PROPERTY_ENEMY_SPAWN: 
	{
		result = L"randomspawm";
	}break;
	}
	return std::move(result);
}

std::wstring ConverTileTypeToStringByProperty(const int type, const TileProperty property)
{
	std::wstring result = L"";
	switch (property)
	{
	case TILE_PROPERTY_NONE:
	{
	}break;
	case TILE_PROPERTY_EXIT: 
	{
		if (type == EXIT_STAGE_ONE)
		{
			result = L"stageone";
		}
		else if (type == EXIT_STAGE_TWO)
		{
			result = L"stagetwo";
		}
		else if (type == EXIT_STAGE_THREE)
		{
			result = L"stagethree";
		}
	}break;
	case TILE_PROPERTY_RANDOM_GEM_SPAWN:
	{
		if (type == GEM_GOLD_SMALL)
		{
			result = L"goldsmall";
		}
		else if (type == GEM_GOLD_BIG)
		{
			result = L"goldbig";
		}
		else if (type == GEM_EMERALD)
		{
			result = L"emerald";
		}
		else if (type == GEM_SAPHIRE)
		{
			result = L"saphire";
		}
		else if (type == GEM_RUBY)
		{
			result = L"ruby";
		}
	}break;
	case TILE_PROPERTY_RANDOM_TILE_SPAWN: 
	{
	}break;
	case TILE_PROPERTY_RANDOM_ITEM_SPAWN: 
	{
	}break;
	case TILE_PROPERTY_RANDOM_ENEMY_SPAWN: 
	{
	}break;
	case TILE_PROPERTY_ENEMY_SPAWN: 
	{
	}break;
	}

	return std::move(result);
}
