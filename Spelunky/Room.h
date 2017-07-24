#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"	
#include "Tile.h"
#include "Tunnel.h"
#include "Gem.h"
#include "Throws.h"

#include "Bomb.h"
#include "Enemy.h"

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



typedef std::list<BaseProperty *> BasePropertyList;

struct Room
{
	~Room()
	{
		//for (auto &prop : properties)
		//{
		//	SAFE_DELETE(prop.second);
		//}
		properties.clear();
	}

	RoomType roomType;
	D2DSprite *roomBackgroud;
	Tile *layer0[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};
	Tile *layer1[ROOM_TILE_COUNTX * ROOM_TILE_COUNTY]{};
	std::unordered_map<std::wstring, BasePropertyList> properties;
};

class Stage
{
public:
	Stage();
	~Stage();
	HRESULT Init();
	HRESULT InitFromRoomTypes(const std::wstring &firstKey, const RandomRoomGenerated &randomTypes);
	HRESULT InitForMiddleStage(const std::wstring firstKey);
	void Release();

	void Update(float deltaTime);
	void Render(const TilePosition &camPos);

	void ClearAllTheBits(int xStartIndex, int yStartIndex, int width, int height);
	void CalculateAllMask(int xStartIndex, int yStartIndex, int width, int height);
	void CalculateMask(int xStartIndex, int yStartIndex, int width, int height, int layer);

	ReturnTile GetAdjacent9(const IntVector2 &pos);

	IntVector2 GetStartPosition() { return IntVector2(_tunnels[0]->position.tileX, _tunnels[0]->position.tileY); }

	void DestroyTile(const IntVector2 &tilePos);
	void DestroyTile(int xStartIndex, int yStartIndex, int width, int height);

	const TilePosition &GetCurrentExitPosition() { return _tunnels[1]->position; }

private:
	void RegisterDelegates();
	void UnRegisterDelegates();

	void BuildBorder();
	bool GetRandomFileNameFromRoomType(RoomType types, std::wstring &str);

	void BuildRoomFromFile(const std::wstring &fileName, Room *room, int roomX, int roomY, bool border, std::map<std::wstring, D2DSprite *> &usingSprites);
	void BuildTileLayerFromFile(FileUtils::File & file, int roomX, int roomY, bool border, std::map<std::wstring, D2DSprite *> &usingSprites);
	void BuildOrnamentsFromFile(FileUtils::File & file, int roomX, int roomY, bool border, std::map<std::wstring, D2DSprite *> &usingSprites);

	void CheckUsingSpriteExistence(const std::wstring &key);

	void TileInfoBitmaskCopy(D2DSprite *sourSprite, Tile *sourTile, Tile *maskTile, uint32 offset);

	void BuildEntrance();
	void BuildGems();
	void BuildThrows();
	void BuildEnemies();
	void CollectRoomPropertyFromFile(FileUtils::File &file, Room *room);


	void HandleCollectMoneyEvent(const IEvent *event);
	void HandleItemBreakEvent(const IEvent *event);
	void HandleEnemyDeadEvent(const IEvent *event);
	void HandleThrowBombEvent(const IEvent *event);
private:
	Room _rooms[16]{};

	Tile *tileLayer0[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};
	Tile *tileLayer1[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};
	Tile *tileLayer2[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};

	Tunnel *_tunnels[2]{};

	std::list<Bomb *> _bombs{};
	std::list<Gem *> _gems{};
	std::list<Throws *> _throws{};
	std::list<Enemy *> _enemies{};


	std::map<std::wstring, D2DSprite *> _usingSprites;

	D2DSprite *_backgroundSprite{};

	RandomRoomGenerated _stageBuildInfo{};
};

#endif