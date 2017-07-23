#ifndef TILE_H
#define TILE_H

#include "CollidingObject.h"	


class Tile : public CollidingObject
{
public :
	Tile(ObjectId id);
	virtual ~Tile();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

public :

	virtual GameObject *Copy(ObjectId id);

	Tile &operator= (TileProperty *other);
	Tile &operator= (const Tile &other);

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
};
struct ReturnTile
{
	Tile *tiles[9];
	int tileNum{};
};

#endif