#ifndef PROPERTY_H
#define PROPERTY_H

class D2DSprite;

namespace MapTool
{
	struct PropertyInfo;
}

enum Direction
{
	Left, Right, Up, Down, None,
};

typedef uint32 ObjectId;

struct MaskInfo
{
	D2DSprite *maskSprite{};
	bool32 hasMask{};
};

enum EatableType
{
	EATABLE_Bomb,
	EATABLE_Rope,
};

enum CrateType
{
	CRATE_GemSpawn,
	CRATE_ItemSpawn
};

enum EnemyType
{
	ENEMY_NONE,
	ENEMY_Snake,
	ENEMY_Spider,
	ENEMY_Primitive,
	ENEMY_Bat,
	ENEMY_StrongSnake,
};

enum GemType
{
	GEM_Emerald,
	GEM_Saphire,
	GEM_Ruby,
	GEM_Diamond,
	GEM_Ingot,
	GEM_ThreeIngot,
	GEM_Nugget,
	GEM_Stone,
	GEM_ThreeStone,
};

enum ObstacleType
{
	OBSTACLE_PushingRock,
	OBSTACLE_BombCrate,
	OBSTACLE_ArrowRock,
};

//�� ���� ��
//���� ��,
//���� �Ը� ��
//�Ʒ��� �Ը� ��
enum RoomType
{
	ROOM_NONE,
	ROOM_BLOCK,
	ROOM_AISLE,
	ROOM_TOP_OPEN,
	ROOM_BOTTOM_OPEN,
};

enum TileCollisionType
{
	TILE_COLLISION_NONE,
	TILE_COLLISION_BLOCK,
	TILE_COLLISION_UPPER_DEAD,
	TILE_COLLISION_LADDER,
	TILE_COLLISION_EOF_LADDER,
	TILE_COLLISION_EXIT,
};

struct BaseProperty
{
	IntVector2 position;
	bool32 valid{true};
};

struct PlayerProperty : public BaseProperty
{
	virtual void Init(const MapTool::PropertyInfo &propertyInfo) {}
};

struct TileProperty : public BaseProperty
{
	D2DSprite *sprite{};
	MaskInfo spriteMaskInfo[4]{};
	IntVector2 sourceIndex{ -1, -1 };
	IntVector2 destroyedIndex{};

	uint32 thisMaskInfo{};
	uint32 nearMaskInfo{};
	TileCollisionType collisionType{};
	uint32 maskInfo{};

	bool32 canBeDestroyedByBomb{ false };

	int layer{};

	virtual void Init(const MapTool::PropertyInfo &propertyInfo) {}
};

struct GemProperty : public BaseProperty
{
	IntVector2 sourceIndex{-1, -1};
	int value;
	GemType type;
	virtual void Init(const MapTool::PropertyInfo &propertyInfo);
};

struct TunnelProperty : public BaseProperty
{
	IntVector2 sourceIndex{-1, -1};
	bool32 exit{false};
	virtual void Init(const MapTool::PropertyInfo &propertyInfo);
};

struct BombProperty : public BaseProperty
{
	IntVector2 sourceIndex{ -1, -1 };
	bool32 sticky{ false };
	Vector2 initialVelocity{};
	virtual void Init(const MapTool::PropertyInfo &propertyInfo);
};

struct ThrowProperty : public BaseProperty
{
	IntVector2 sourceIndex{ -1, -1 };
	bool32 breakable;
	virtual void Init(const MapTool::PropertyInfo &propertyInfo);
};

struct EnemyProperty : public BaseProperty
{
	EnemyType type;
	virtual void Init(const MapTool::PropertyInfo &propertyInfo);
};

struct CrateProperty : public BaseProperty
{
	IntVector2 sourceIndex{ -1, -1 };
	CrateType type;
	virtual void Init(const MapTool::PropertyInfo &propertyInfo);
};

struct EatableProperty : public BaseProperty
{
	IntVector2 sourceIndex{ -1, -1 };
	EatableType type;
	virtual void Init(const MapTool::PropertyInfo &propertyInfo);
};

struct ObstacleProperty : public BaseProperty
{
	IntVector2 sourceIndex{ -1, -1 };
	ObstacleType type;
	Direction facingDirection;
	virtual void Init(const MapTool::PropertyInfo &propertyInfo);
};

int ConvertStringToTileProperty(const std::wstring &str);
int ConvertStringToTypeByProperty(TileProperty property, const std::wstring &str);

std::wstring ConvertTilePropertyToString(const TileProperty property);
std::wstring ConverTileTypeToStringByProperty(const int type, const TileProperty property);

#endif