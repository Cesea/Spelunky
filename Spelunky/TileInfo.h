#ifndef TILE_INFO_H
#define TILE_INFO_H

#include <string>
#include "IntVector2.h"

enum TileCollisionType
{
	TILE_COLLISION_NONE,
	TILE_COLLISION_BLOCK,
	TILE_COLLISION_UPPER,
};

enum ObjectLayer
{
	LAYER_TILE,
	LAYER_OBJECT,
	LAYER_MASK,
};


struct TileInfo
{
	TileInfo()
	{}

	TileInfo &operator= (const TileInfo	&other)
	{
		imageKey = other.imageKey;
		sourceIndex = other.sourceIndex;
		destroyedIndex = other.destroyedIndex;
		canMask = other.canMask;
		nearMaskInfo = other.nearMaskInfo;
		collisionType = other.collisionType;
		maskInfo = other.maskInfo;
		canBeDestroyedByBomb = other.canBeDestroyedByBomb;
		layer = other.layer;
		return *this;
	}
	//TileInfo(const std::wstring &imageKey, const IntVector2 &pos)
	//	: imageKey(imageKey), sourceIndex(pos), maskInfo() , nearMaskInfo(), destroyedIndex()
	//{}
	std::wstring imageKey{};
	IntVector2 sourceIndex{-1, -1};
	IntVector2 destroyedIndex{};

	uint32 canMask{};
	uint32 nearMaskInfo{};
	TileCollisionType collisionType{};
	uint32 maskInfo{};

	bool32 canBeDestroyedByBomb{false};
	int layer{};
};

struct TileImageInfo
{
	TileImageInfo() {}
	~TileImageInfo() {}

	TileInfo tileInfos[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};
	int applied[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{false,};
};


//복도 방,
//다 막힌 방
//위만 뚤린 방
//아래만 뚤린 방
enum RoomType
{
	//ROOM_,

};

struct RoomInfo
{
	//union 
	//{
	//	struct
	//	{
	//		TileInfo layer0[];
	//		TileInfo layer1[];
	//		TileInfo layer2[];
	//	};
	//	TileInfo
	//};
	TileInfo layer0[ROOM_TILE_COUNTX * ROOM_TILE_COUNTX];
	TileInfo layer1[ROOM_TILE_COUNTX * ROOM_TILE_COUNTX];
	TileInfo layer2[ROOM_TILE_COUNTX * ROOM_TILE_COUNTX];
};


#endif