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


#endif
