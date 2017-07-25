#include "stdafx.h"
#include "Property.h"
//
//
//int ConvertStringToTileProperty(const std::wstring & str)
//{
//	int result{0};
//	if (str == L"exit" || str == L"door")
//	{
//		result = TILE_PROPERTY_EXIT;
//	}
//	else if (str == L"randomgem" || str == L"random gem" || str == L"randomGem")
//	{
//		result = TILE_PROPERTY_RANDOM_GEM_SPAWN;
//	}
//	else if (str == L"randomtile" || str == L"random tile" || str == L"randomTile")
//	{
//		result = TILE_PROPERTY_RANDOM_TILE_SPAWN;
//	}
//	else if (str == L"randomitem" || str == L"random item" || str == L"randomItem")
//	{
//		result = TILE_PROPERTY_RANDOM_ITEM_SPAWN;
//	}
//	else if (str == L"randomenemy" || str == L"random enemy" || str == L"randomEnemy")
//	{
//		result = TILE_PROPERTY_RANDOM_ENEMY_SPAWN;
//	}
//	else
//	{
//		result = TILE_PROPERTY_NONE;
//	}
//	return result;
//}
//
//int ConvertStringToTypeByProperty(TileProperty property, const std::wstring &str)
//{
//	switch (property)
//	{
//	case TILE_PROPERTY_NONE:
//	{
//
//	}break;
//	case TILE_PROPERTY_EXIT: 
//	{
//		if (str == L"stage1" || str == L"stageone" || str == L"one")
//		{
//			return (int)EXIT_STAGE_ONE;
//		}
//		else if (str == L"stage2" || str == L"stagetwo" || str == L"two")
//		{
//			return (int)EXIT_STAGE_TWO;
//		}
//		else if (str == L"stage3" || str == L"stagethree" || str == L"three")
//		{
//			return (int)EXIT_STAGE_THREE;
//		}
//		else
//		{
//			return (int)EXIT_NONE;
//		}
//	}break;
//	case TILE_PROPERTY_RANDOM_GEM_SPAWN:
//	{
//		if (str == L"goldsmall" || str == L"small")
//		{
//			return (int)GEM_GOLD_SMALL;
//		}
//		else if (str == L"goldbig" || str == L"big")
//		{
//			return (int)GEM_GOLD_BIG;
//		}
//		else if (str == L"emerald" || str == L"Emerald")
//		{
//			return (int)GEM_EMERALD;
//		}
//		else if (str == L"saphire" || str == L"Saphire")
//		{
//			return (int)GEM_SAPHIRE;
//		}
//		else if (str == L"ruby" || str == L"Ruby")
//		{
//			return (int)GEM_RUBY;
//		}
//		else
//		{
//			return (int)GEM_NONE;
//		}
//	}break;
//	case TILE_PROPERTY_RANDOM_TILE_SPAWN: 
//	{
//	}break;
//	case TILE_PROPERTY_RANDOM_ITEM_SPAWN: 
//	{
//	}break;
//	case TILE_PROPERTY_RANDOM_ENEMY_SPAWN: 
//	{
//	}break;
//	case TILE_PROPERTY_ENEMY_SPAWN: 
//	{
//	}break;
//	}
//
//	return 0;
//}
//
//std::wstring ConvertTilePropertyToString(const TileProperty property)
//{
//	std::wstring result = L"";
//	switch (property)
//	{
//	case TILE_PROPERTY_NONE:
//	{
//	}break;
//	case TILE_PROPERTY_EXIT: 
//	{
//		result = L"exit";
//	}break;
//	case TILE_PROPERTY_RANDOM_GEM_SPAWN:
//	{
//		result = L"randomgem";
//	}break;
//	case TILE_PROPERTY_RANDOM_TILE_SPAWN: 
//	{
//		result = L"randomtile";
//	}break;
//	case TILE_PROPERTY_RANDOM_ITEM_SPAWN: 
//	{
//		result = L"randomitem";
//	}break;
//	case TILE_PROPERTY_RANDOM_ENEMY_SPAWN: 
//	{
//		result = L"randomemeny";
//	}break;
//	case TILE_PROPERTY_ENEMY_SPAWN: 
//	{
//		result = L"randomspawm";
//	}break;
//	}
//	return std::move(result);
//}
//
//std::wstring ConverTileTypeToStringByProperty(const int type, const TileProperty property)
//{
//	std::wstring result = L"";
//	switch (property)
//	{
//	case TILE_PROPERTY_NONE:
//	{
//	}break;
//	case TILE_PROPERTY_EXIT: 
//	{
//		if (type == EXIT_STAGE_ONE)
//		{
//			result = L"stageone";
//		}
//		else if (type == EXIT_STAGE_TWO)
//		{
//			result = L"stagetwo";
//		}
//		else if (type == EXIT_STAGE_THREE)
//		{
//			result = L"stagethree";
//		}
//	}break;
//	case TILE_PROPERTY_RANDOM_GEM_SPAWN:
//	{
//		if (type == GEM_GOLD_SMALL)
//		{
//			result = L"goldsmall";
//		}
//		else if (type == GEM_GOLD_BIG)
//		{
//			result = L"goldbig";
//		}
//		else if (type == GEM_EMERALD)
//		{
//			result = L"emerald";
//		}
//		else if (type == GEM_SAPHIRE)
//		{
//			result = L"saphire";
//		}
//		else if (type == GEM_RUBY)
//		{
//			result = L"ruby";
//		}
//	}break;
//	case TILE_PROPERTY_RANDOM_TILE_SPAWN: 
//	{
//	}break;
//	case TILE_PROPERTY_RANDOM_ITEM_SPAWN: 
//	{
//	}break;
//	case TILE_PROPERTY_RANDOM_ENEMY_SPAWN: 
//	{
//	}break;
//	case TILE_PROPERTY_ENEMY_SPAWN: 
//	{
//	}break;
//	}
//
//	return std::move(result);
//}

#include "TileInfo.h"

void GemProperty::Init(const MapTool::PropertyInfo & propertyInfo)
{
	sourceIndex = propertyInfo.sourceIndex;
	value = _wtoi(propertyInfo.value0);
	if (value == 800)
	{
		type = GemType::GEM_Emerald;
	}
	else if (value == 1200)
	{
		type = GemType::GEM_Saphire;
	}
	else if (value == 1600)
	{
		type = GemType::GEM_Ruby;
	}
	else if (value == 2000)
	{
		type = GemType::GEM_Diamond;
	}
	else if (value == 500)
	{
		type = GemType::GEM_Ingot;
	}
	else if (value == 1500)
	{
		type = GemType::GEM_ThreeIngot;
	}
	else if (value == 100)
	{
		type = GemType::GEM_Nugget;
	}
}

void TunnelProperty::Init(const MapTool::PropertyInfo & propertyInfo)
{
	sourceIndex = propertyInfo.sourceIndex;
}

void BombProperty::Init(const MapTool::PropertyInfo & porpertyInfo)
{
}

void ThrowProperty::Init(const MapTool::PropertyInfo & porpertyInfo)
{
	sourceIndex = porpertyInfo.sourceIndex;
	breakable = _wtoi(porpertyInfo.value0);
}

void EnemyProperty::Init(const MapTool::PropertyInfo & propertyInfo)
{
	if (wcscmp(propertyInfo.value0, L"snake") == 0)
	{
		type = EnemyType::ENEMY_Snake;
	}
	else if (wcscmp(propertyInfo.value0, L"spider") == 0)
	{
		type = EnemyType::ENEMY_Spider;
	}
	else if (wcscmp(propertyInfo.value0, L"primitive") == 0)
	{
		type = EnemyType::ENEMY_Primitive;
	}
	else if (wcscmp(propertyInfo.value0, L"bat") == 0)
	{
		type = EnemyType::ENEMY_Bat;
	}
	else if (wcscmp(propertyInfo.value0, L"strongSnake") == 0)
	{
		type = EnemyType::ENEMY_StrongSnake;
	}

}
