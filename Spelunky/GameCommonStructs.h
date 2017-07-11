#ifndef GAME_COMMON_STRUCTS_H
#define GAME_COMMON_STRUCTS_H

class TilePosition
{
public:
	TilePosition()
		:tileX(), tileY(), tileRel()
	{}
	explicit TilePosition(const Vector2 &v)
	{
		tileX = v.x / TILE_SIZE;
		tileY = v.y / TILE_SIZE;
		tileRel.x = v.x - (tileX * TILE_SIZE);
		tileRel.y = v.y - (tileY * TILE_SIZE);
	}
	explicit TilePosition(float x, float y)
	{
		tileX = x / TILE_SIZE;
		tileY = y / TILE_SIZE;
		tileRel.x = x - (tileX * TILE_SIZE);
		tileRel.y = y - (tileY * TILE_SIZE);
	}
	explicit TilePosition(int x, int y)
		:tileX(x), tileY(y), tileRel()
	{}
	explicit TilePosition(int x, int y, float fX, float fY)
		:tileX(x), tileY(y), tileRel(fX, fY)
	{}

	TilePosition &operator= (const TilePosition &other);

	void AddToTile(int x, int y) { tileX += x; tileY += y; }
	void AddToTileX(int i) { tileX += i; }
	void AddToTileY(int i) { tileY += i; }

	void AddToTileRel(float x, float y);
	void AddToTileRelX(float f);
	void AddToTileRelY(float f);


	//���� true�� ��ȯ�ߴٸ� ReTileize �� ���̰� false�� ��ȯ�� �̷������ ����
	bool ReTilelize();
	bool ReTilelizeX();
	bool ReTilelizeY();
	inline Vector2 UnTilelize() const
	{
		return Vector2((TILE_SIZE * tileX) + tileRel.x, (TILE_SIZE * tileY) + tileRel.y);
	}

	int tileX;
	int tileY;
	Vector2 tileRel;
};

template<typename T>
struct TileSet
{
	TileSet(uint32 countX , uint32 countY , const T &clearValue);
	~TileSet();

	void Clear(T value);

	T GetValue(uint32 x, uint32 y);
	void SetValue(uint32 x, uint32 y, const T &value);

	T &At(uint32 x, uint32 y);
	T &At(uint32 i);

	uint32 countX{};
	uint32 countY{};
	T *tiles{};
	//uint32 tiles[TILECOUNTY][TILECOUNTX] = {};
};

typedef uint32 ObjectId;

enum class ArcheType
{
	Player,
	Tile,
	MineBG,
	GUI,
};

enum Direction
{
	Left, Right, Up, Down
};

enum class Command
{
	None,
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,
	DashOn,
	DashOff,
	UseBomb,
	UseRope,
	Attack,
	Pickup,
	Jump,
	JumpOff
};

struct ControlCommand
{
	bool fire;
	Command horizontal{};
	Command vertical{};
	Command jump{};
	Command action{};
	Command dash{};
};


enum TileProperty
{
	TILE_None,
	TILE_Block,
	TILE_Pushable,
	TILE_Climbable,
	TILE_StandableClimb,
	TILE_VerticalDeath,
};

enum class EntityState
{
	None,
	Idle, 
	Walking, 
	CrawlDown,
	CrawlUp,
	CrawlMove,
	CrawlIdle,
	LookUp,
	LookDown,
	Jumping, 
	Falling,
	Climbing,
	Grabbing,
	OnLedge,
	Attacking, 
	Throwing,
	Hurt,
	Faint,
	Dead,
};


template<typename T>
inline TileSet<T>::TileSet(uint32 countX, uint32 countY, const T & clearValue)
{
	this->countX = countX;
	this->countY = countY;
	tiles = new T[countX * countY];
	Clear(clearValue);
}

template<typename T>
inline TileSet<T>::~TileSet()
{
	delete[] tiles;
}

template<typename T>
inline void TileSet<T>::Clear(T value)
{
	for (int i = 0; i < countX * countY; ++i)
	{
		tiles[i] = value;
	}
}

template<typename T>
inline T TileSet<T>::GetValue(uint32 x, uint32 y)
{
	return tiles[x + countX * y];
}

template<typename T>
inline void TileSet<T>::SetValue(uint32 x, uint32 y, const T & value)
{
	tiles[x + countX * y] = value;
}

template<typename T>
inline T & TileSet<T>::At(uint32 x, uint32 y)
{
	return tiles[x + countX	* y];
}

template<typename T>
inline T & TileSet<T>::At(uint32 i)
{
	Assert(i >= 0 && i < countX* countY);
	return tiles[i];
}


#endif

