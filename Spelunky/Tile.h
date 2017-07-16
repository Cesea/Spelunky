#ifndef TILE_H
#define TILE_H

#include "stdafx.h"

#include "D2DSprite.h"

namespace PlayScene
{
	struct MaskInfo
	{
		D2DSprite *maskSprite{};
		bool32 hasMask{};
	};

	struct Tile
	{
		Tile &operator= (Tile &other)
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
			return *this;
		}

		D2DSprite *sprite{};
		MaskInfo spriteMaskInfo[4]{};
		IntVector2 sourceIndex{ -1, -1 };
		IntVector2 destroyedIndex{};

		uint32 thisMaskInfo{};
		uint32 nearMaskInfo{};
		TileCollisionType collisionType{};
		uint32 maskInfo{};

		bool32 canBeDestroyedByBomb{ false };
		
		//이 변수가 여기에 있어야 할까??? 아니면 룸에서 가지고 있어야 할까??
		int layer{};

		IntVector2 position;
	};

}


#endif