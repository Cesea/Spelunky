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
	TileInfo(const std::wstring &imageKey, const IntVector2 &pos)
		: imageKey(imageKey), sourceIndex(pos), maskInfo() , nearMaskInfo(), destroyedIndex()
	{}
	std::wstring imageKey{};
	IntVector2 sourceIndex{};
	IntVector2 destroyedIndex{};

	bool32 canMask{false};
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

#endif