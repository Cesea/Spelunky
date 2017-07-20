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

//다 막힌 방
//복도 방,
//위만 뚤린 방
//아래만 뚤린 방
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
	virtual void Init(const MapTool::PropertyInfo &propertyInfo);
};

struct TunnelProperty : public BaseProperty
{
	IntVector2 sourceIndex{-1, -1};
	bool32 exit{false};
	virtual void Init(const MapTool::PropertyInfo &propertyInfo);
};

int ConvertStringToTileProperty(const std::wstring &str);
int ConvertStringToTypeByProperty(TileProperty property, const std::wstring &str);

std::wstring ConvertTilePropertyToString(const TileProperty property);
std::wstring ConverTileTypeToStringByProperty(const int type, const TileProperty property);

#endif