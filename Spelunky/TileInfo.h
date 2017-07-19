#ifndef TILE_INFO_H
#define TILE_INFO_H

#include <string>
#include "IntVector2.h"

namespace MapTool
{
	struct MaskKeyInfo
	{
		std::wstring maskImageKey{};
		bool32 hasMask{};
	};

	struct BaseInfo
	{
		bool32 valid{true};
	};

	struct TileInfo : public BaseInfo
	{
		TileInfo()
		{}
		TileInfo &operator= (const TileInfo	&other)
		{
			imageKey = other.imageKey;
			sourceIndex = other.sourceIndex;
			destroyedIndex = other.destroyedIndex;
			thisMaskInfo = other.thisMaskInfo;
			nearMaskInfo = other.nearMaskInfo;
			collisionType = other.collisionType;
			maskInfo = other.maskInfo;
			canBeDestroyedByBomb = other.canBeDestroyedByBomb;
			layer = other.layer;
			return *this;
		}
		std::wstring imageKey{};
		MaskKeyInfo imageMaskInfo[4]{};
		IntVector2 sourceIndex{ -1, -1 };
		IntVector2 destroyedIndex{};

		uint32 thisMaskInfo{};
		uint32 nearMaskInfo{};
		TileCollisionType collisionType{};
		uint32 maskInfo{};

		bool32 canBeDestroyedByBomb{ false };
		int layer{};
	};

	struct TileImageInfo
	{
		TileImageInfo() {}
		~TileImageInfo() {}

		TileInfo tileInfos[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};
		int applied[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{ false, };
	};

	struct RoomInfo
	{
		RoomType roomType;
		IntVector2 doorPosition;

		TileInfo layer0[ROOM_TILE_COUNTX * ROOM_TILE_COUNTX];
		TileInfo layer1[ROOM_TILE_COUNTX * ROOM_TILE_COUNTX];

	};
}
#endif
