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
		D2DSprite *_sprite{};
		//std::wstring imageKey{};
		MaskInfo spriteMaskInfo[4]{};
		IntVector2 sourceIndex{ -1, -1 };
		IntVector2 destroyedIndex{};

		uint32 thisMaskInfo{};
		uint32 nearMaskInfo{};
		TileCollisionType collisionType{};
		uint32 maskInfo{};

		bool32 canBeDestroyedByBomb{ false };
		
		//�� ������ ���⿡ �־�� �ұ�??? �ƴϸ� �뿡�� ������ �־�� �ұ�??
		int layer{};
	};

}


#endif