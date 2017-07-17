#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "Room.h"
class MovingObject;
//#include "MovingObject.h"

class CollisionComponent
{
public :
	CollisionComponent() {}
	virtual ~CollisionComponent() {}

	virtual void Init(const Rect &rect, const Vector2 &offset);
	virtual void Update(MovingObject *object, float deltaTime, const PlayScene::ReturnTile *nearTiles);

	const Rect &GetRect() { return _rect; }
	const Vector2 &GetOffset() { return _rectOffset; }

	void SetRepulse(bool b) { _repulse = b; }

private :
	void CollideRepulse(MovingObject *object, float deltaTime, const PlayScene::ReturnTile *nearTiles);
	void CollideStop(MovingObject *object, float deltaTime, const PlayScene::ReturnTile *nearTiles);

	Rect _rect;
	Vector2 _rectOffset;

	bool32 _repulse{ false };
};


#endif
