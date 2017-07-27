#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"	
#include "Tile.h"
#include "Tunnel.h"
#include "Gem.h"
#include "Throws.h"

#include "Bomb.h"
#include "Enemy.h"

#include "Crate.h"
#include "Eatables.h"

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

	void PostInit();
	void Release();

	void Update(float deltaTime);
	void Render(const TilePosition &camPos);

	void RenderDeeperLayer(int minX, int maxX, int minY, int maxY, const Vector2 &camPos);
	void RenderBlockingLayer(int minX, int maxX, int minY, int maxY, const Vector2 &camPos);
	void RenderObjects(const Vector2 &camPos);

	void ClearAllTheBits(int xStartIndex, int yStartIndex, int width, int height);
	void CalculateAllMask(int xStartIndex, int yStartIndex, int width, int height);
	void CalculateMask(int xStartIndex, int yStartIndex, int width, int height, int layer);

	ReturnTile GetAdjacent9(const IntVector2 &pos);

	IntVector2 GetStartPosition() { return IntVector2(_tunnels[0]->position.tileX, _tunnels[0]->position.tileY); }

	void DestroyTile(const IntVector2 &tilePos);
	void DestroyTile(int xStartIndex, int yStartIndex, int width, int height);

	const TilePosition &GetCurrentExitPosition() { return _tunnels[1]->position; }

	const int GetMoneyCollected() { return _moneyCollected; }

	Tile *GetValidTileAt(int x, int y);

private:
	void RegisterDelegates();
	void UnRegisterDelegates();

	void BuildBorder();
	bool GetRandomFileNameFromRoomType(RoomType types, std::wstring &str);

	void BuildRoomFromFile(const std::wstring &fileName, Room *room, int roomX, int roomY, bool border, std::map<std::wstring, D2DSprite *> &usingSprites);
	void BuildTileLayerFromFile(FileUtils::File & file, int roomX, int roomY, bool border, std::map<std::wstring, D2DSprite *> &usingSprites);
	void BuildOrnamentsFromFile(FileUtils::File & file, int roomX, int roomY, bool border, std::map<std::wstring, D2DSprite *> &usingSprites);

	void CheckUsingSpriteExistence(const std::wstring &key);

	void TileInfoBitmaskCopy(D2DSprite *sourSprite, Tile *sourTile, uint32 offset);

	void BuildEntrance();
	void BuildGems();
	void BuildThrows();
	void BuildEnemies();
	void BuildCrates();
	void CollectRoomPropertyFromFile(FileUtils::File &file, Room *room);


	void HandleCollectMoneyEvent(const IEvent *event);
	void HandleItemBreakEvent(const IEvent *event);
	void HandleEnemyDeadEvent(const IEvent *event);
	void HandleThrowBombEvent(const IEvent *event);
	void HandleDestroyATileEvent(const IEvent *event);

	void HandleSpawnObjectEvent(const IEvent *event);
	void HandleCollectEatableEvent(const IEvent *event);

private :
	void BuildRandomGem(const IntVector2 &genPos);
	void BuildRandomEatable(const IntVector2 &genPos);

private:
	Room _rooms[16]{};

	Tile *tileLayer0[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};
	Tile *tileLayer1[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};
	Tile *tileLayer2[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};
	Tile *tileLayer3[STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY]{};

	Tunnel *_tunnels[2]{};

	std::list<Bomb *> _bombs{};
	std::list<Gem *> _gems{};
	std::list<Throws *> _throws{};
	std::list<Enemy *> _enemies{};
	std::list<Crate *> _crates{};
	std::list<Eatables *> _eatables{};


	std::map<std::wstring, D2DSprite *> _usingSprites;

	D2DSprite *_backgroundSprite{};

	RandomRoomGenerated _stageBuildInfo{};

	int _moneyCollected{0};
};

#endif