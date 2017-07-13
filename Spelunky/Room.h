//#ifndef ROOM_H
//#define ROOM_H
//
//#include "stdafx.h"	
//#include "Tile.h"
//
//namespace PlayScene
//{
//	struct Room
//	{
//		Tile tiles[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};
//	};
//
//	Room BuildRoomFromFile(const WCHAR *filePath);
//
//	class Stage
//	{
//	public :
//		Stage();
//		~Stage();
//		HRESULT Init();
//		HRESULT InitFromRooms(std::vector<Room *> &rooms);
//		void Release();
//
//		void Render();
//
//		const Tile &GetTileConstRef(int x, int y);
//		Tile &GetTileRef(int x, int y);
//
//
//		std::vector<Tile &> &GetAdjacent4();
//		std::vector<Tile &> &GetAdjacent5();
//		std::vector<Tile &> &GetAdjacentTiles8();
//		std::vector<Tile &> &GetAdjacentTiles9();
//
//	private :
//		Tile tiles[STAGE_TILE_COUNTX * STAGE_TILE_COUNTY];
//	};
//}
//
//
//#endif