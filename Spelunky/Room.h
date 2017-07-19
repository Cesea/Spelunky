#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"	
#include "Tile.h"

namespace PlayScene
{

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
		Tile layer0[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};
		Tile layer1[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};

		Property properties[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};
	};
	

	class Stage
	{
	public :
		Stage();
		~Stage();
		HRESULT Init();
		HRESULT InitFromRoomTypes(const RandomRoomGenerated &randomTypes);
		void Release();

		void RenderBorderLayer(const TilePosition &camPos);
		void RenderTileLayer(const TilePosition &camPos);
		void RenderMaskLayer(const TilePosition &camPos);

		const Tile &GetTileConstRef(int x, int y);
		Tile &GetTileRef(int x, int y);

		ReturnTile GetAdjacent4(const IntVector2 &p);
		ReturnTile GetAdjacent5(const IntVector2 &p);
		ReturnTile GetAdjacentTiles8(const IntVector2 &p);
		ReturnTile GetAdjacentTiles9(const IntVector2 &p);

		void ClearAllTheBits(int xStartIndex, int yStartIndex, int width, int height);

		void CalculateAllMask(int xStartIndex, int yStartIndex, int width, int height);

	private :
		void BuildBorder();
		void CopyBorderTiles(Room *rooms);

		void CopyTilesFromRooms(Room *rooms);
		bool GetRandomFileNameFromRoomType(RoomType types, std::wstring &str);

		void BuildRoomFromFile(const std::wstring &fileName, Room *room, std::map<std::wstring, D2DSprite *> &usingSprites);
		void BuildTileLayerFromFile(FileUtils::File & file, Tile * tileLayer, std::map<std::wstring, D2DSprite *> &usingSprites);

		void CopyTilePropertyFromFile(FileUtils::File & file, Property *propertyLayer, std::map<std::wstring, D2DSprite *> &usingSprites);

		void BuildProperties();


		void TileInfoBitmaskCopy(D2DSprite *sourSprite, Tile &sourTile, Tile &maskTile, uint32 offset);

		void CheckUsingSpriteExistence(const std::wstring &key);

		void CalculateMask(int xStartIndex, int yStartIndex, int width, int height, int layer);
	private :
		Tile tileLayer0[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY];
		Tile tileLayer1[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY];
		Tile tileLayer2[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY];

		Property propertyLayer[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY];

		std::map<std::wstring, D2DSprite *> _usingSprites;

		IntVector2 _entranceRoomIndex{};
		IntVector2 _exitRoomIndex{};

		IntVector2 _entranceTileIndex{};
		IntVector2 _exitTileIndex{};
		//Tile _entrance{};
		//Tile _exit{};
	};
}


#endif