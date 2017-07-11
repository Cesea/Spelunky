#ifndef TILE_INFO_H
#define TILE_INFO_H

#include <string>
#include "IntVector2.h"

struct TileInfo
{
	TileInfo()
		:name(), sourceIndex()
	{}
	TileInfo(const std::wstring &name, const IntVector2 &pos)
		: name(name), sourceIndex(pos)
	{}

	std::wstring name;
	IntVector2 sourceIndex;
};

#endif