#ifndef GAME_COMMON_STRUCTS_H
#define GAME_COMMON_STRUCTS_H

#include "Property.h"

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
	void AddToTileRel(const Vector2 &v);
	void AddToTileRelX(float f);
	void AddToTileRelY(float f);


	//만약 true를 반환했다면 ReTileize 한 것이고 false면 변환이 이루어지지 않음
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

inline TilePosition InterpolateTilePosition(const TilePosition &start, const TilePosition & end, float t)
{
	Vector2 startUntiled = start.UnTilelize();
	Vector2 endUntiled = end.UnTilelize();

	Vector2 pos = InterpolateVector(startUntiled, endUntiled, t);
	return TilePosition(pos.x, pos.y);
}

template<typename T>
struct TileSet
{
	TileSet(uint32 countX , uint32 countY , const T &clearValue);
	~TileSet();

	void Clear(T value);


	int32 GetValue(uint32 x, uint32 y);
	void SetValue(uint32 x, uint32 y, int32 value);

	void SetInfo(uint32 x, uint32 y, const T &value);

	T &AtInfo(uint32 x, uint32 y);
	T &AtInfo(uint32 i);

	uint32 countX{};
	uint32 countY{};

	//tiles 변수는 해당 타일에 무언가가 있는지의 유무를 나타내고 
	//실제 정보들은 infos 변수에 담아져 있다.
	int32 *tiles{};
	T *infos{};
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
	JumpOff,
	GoExit,
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

enum ObjectLayer
{
	LAYER_TILE,
	LAYER_OBJECT,
	LAYER_MASK,
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
	tiles = new int32[countX * countY];
	infos = new T[countX * countY];
	Clear(clearValue);
}

template<typename T>
inline TileSet<T>::~TileSet()
{
	delete[] tiles;
	delete[] infos;
}

template<typename T>
inline void TileSet<T>::Clear(T value)
{
	for (int i = 0; i < countX * countY; ++i)
	{
		tiles[i] = 0;
		infos[i] = value;
	}
}

template<typename T>
inline int32 TileSet<T>::GetValue(uint32 x, uint32 y)
{
	if ((x < 0) || (x > countX - 1) || (y < 0) || (y > countY - 1))
	{
		Console::Log("Aceessing out of border\n");
	}
	return tiles[x + countX * y];
}

template<typename T>
inline void TileSet<T>::SetValue(uint32 x, uint32 y, int32 value)
{
	if ((x < 0) || (x > countX - 1) || (y < 0) || (y > countY - 1))
	{
		Console::Log("Aceessing out of border\n");
	}
	tiles[x + countX * y] = value;
}

template<typename T>
inline void TileSet<T>::SetInfo(uint32 x, uint32 y, const T & value)
{
	if ((x < 0) || (x > countX - 1) || (y < 0) || (y > countY - 1))
	{
		Console::Log("Aceessing out of border\n");
	}
	infos[x + countX * y] = value;
}

template<typename T>
inline T & TileSet<T>::AtInfo(uint32 x, uint32 y)
{
	if ((x < 0) || (x > countX - 1) || (y < 0) || (y > countY - 1))
	{
		Console::Log("Aceessing out of border\n");
	}
	return infos[x + countX * y];
}

template<typename T>
inline T & TileSet<T>::AtInfo(uint32 i)
{
	if ((i < 0) || i > countX * countY - 1)
	{
		Console::Log("Aceessing out of border\n");
	}
	return infos[i];
}


#endif

