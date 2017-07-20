#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"	
#include "Tile.h"

struct RandomRoomGenerated;

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
};


class Stage
{
public:
	Stage();
	~Stage();
	HRESULT Init();
	HRESULT InitFromRoomTypes(const RandomRoomGenerated &randomTypes);
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

private:
	Tile *tileLayer0[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};
	Tile *tileLayer1[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};
	Tile *tileLayer2[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};

	std::map<std::wstring, D2DSprite *> _usingSprites;

	IntVector2 _entranceRoomIndex{};
	IntVector2 _exitRoomIndex{};

	IntVector2 _entranceTileIndex{};
	IntVector2 _exitTileIndex{};
};

#endif