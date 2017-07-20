#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"	
#include "Tile.h"
#include "Tunnel.h"

struct RandomRoomGenerated
{
	IntVector2 startRoomIndex{};
	IntVector2 exitRoomIndex{};
	RoomType roomTypes[16]{ ROOM_NONE, };

	RandomRoomGenerated &operator= (const RandomRoomGenerated &other)
	{
		startRoomIndex = other.startRoomIndex;
		exitRoomIndex = other.exitRoomIndex;
		for (int i = 0; i < 16; ++i)
		{
			roomTypes[i] = other.roomTypes[i];
		}
		return *this;
	}
};


struct ReturnTile
{
	Tile *tiles[9];
	int tileNum{};
};

struct Room
{
	RoomType roomType;
	D2DSprite *roomBackgroud;
	Tile *layer0[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};
	Tile *layer1[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};
	std::unordered_map<std::wstring, BaseProperty *> properties;
};

class Stage
{
public:
	Stage();
	~Stage();
	HRESULT Init();
	HRESULT InitFromRoomTypes(const std::wstring &firstKey, const RandomRoomGenerated &randomTypes);
	void Release();

	void Render(const TilePosition &camPos);
	void RenderMask(const TilePosition &camPos);

	void ClearAllTheBits(int xStartIndex, int yStartIndex, int width, int height);
	void CalculateAllMask(int xStartIndex, int yStartIndex, int width, int height);
	void CalculateMask(int xStartIndex, int yStartIndex, int width, int height, int layer);

private:
	void BuildBorder();
	void CopyBorderTiles(Room *rooms);
	bool GetRandomFileNameFromRoomType(RoomType types, std::wstring &str);

	void BuildRoomFromFile(const std::wstring &fileName, Room *room, int roomX, int roomY, bool border, std::map<std::wstring, D2DSprite *> &usingSprites);
	void BuildTileLayerFromFile(FileUtils::File & file, Tile **pTileLayer, int roomX, int roomY, bool border, int layerNum, std::map<std::wstring, D2DSprite *> &usingSprites);

	void CheckUsingSpriteExistence(const std::wstring &key);

	void CopyTilesPosition(int worldIndex, int x, int y);
	void CopyTilesFromRooms(Room *rooms);

	void TileInfoBitmaskCopy(D2DSprite *sourSprite, Tile *sourTile, Tile *maskTile, uint32 offset);

	void BuildEntrance();
	void CollectRoomPropertyFromFile(FileUtils::File &file, Room *room);

	void BuildProperties();

private:
	Room _rooms[16]{};

	Tile *tileLayer0[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};
	Tile *tileLayer1[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};
	Tile *tileLayer2[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};

	Tunnel *_tunnels[2]{};

	std::map<std::wstring, D2DSprite *> _usingSprites;

	D2DSprite *_backgroundSprite{};

	RandomRoomGenerated _stageBuildInfo{};
};

#endif