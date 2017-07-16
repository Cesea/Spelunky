#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"	
#include "Tile.h"

namespace PlayScene
{

	struct ReturnTile
	{
		Tile *tiles[9];
		int tileNum{};
	};

	struct Room
	{
		RoomType roomType;
		D2DSprite *roomBackgroud;
		Tile layer0[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};
		Tile layer1[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};
	};
	

	class Stage
	{
	public :
		Stage();
		~Stage();
		HRESULT Init();
		HRESULT InitFromRoomTypes(RoomType *randomRoomType);
		void Release();

		void RenderTileLayer(const TilePosition &camPos);
		void RenderMaskLayer(const TilePosition &camPos);

		const Tile &GetTileConstRef(int x, int y);
		Tile &GetTileRef(int x, int y);

		ReturnTile GetAdjacent4(const IntVector2 &p);
		ReturnTile GetAdjacent5(const IntVector2 &p);
		ReturnTile GetAdjacentTiles8(const IntVector2 &p);
		ReturnTile GetAdjacentTiles9(const IntVector2 &p);

		void CalculateMask(int xStartIndex, int yStartIndex, int width, int height);
		void ClearAllTheBits(int xStartIndex, int yStartIndex, int width, int height);

	private :
		void BuildBorder();
		void CopyBorderTiles(Room *rooms);

		void CopyTilesFromRooms(Room *rooms);
		bool GetRandomFileNameFromRoomType(RoomType types, WCHAR *buffer);

		void BuildRoomFromFile(const std::wstring &fileName, Room *room, std::map<std::wstring, D2DSprite *> &usingSprites);
		void BuildTileLayerFromFile(FileUtils::File & file, Tile * tileLayer, std::map<std::wstring, D2DSprite *> &usingSprites);

		void TileInfoBitmaskCopy(D2DSprite *sourSprite, Tile &sourTile, Tile &maskTile, uint32 offset);

		void CheckUsingSpriteExistence(const std::wstring &key);
	private :

		Tile tileLayer0[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY];
		Tile tileLayer1[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY];

		std::map<std::wstring, D2DSprite *> _usingSprites;
	};
}


#endif