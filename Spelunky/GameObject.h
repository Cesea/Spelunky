#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "gameNode.h"

#include "D2DSprite.h"
#include "D2DAnimationSprite.h"

#include "StateManager.h"
#include "DataSet.h"

#include "File.h"

//#include "CollisionComponent.h"
//#include "MoveComponent.h"

class CollisionComponent;
class MoveComponent;

class GameObject : public gameNode
{
public :
	GameObject(ObjectId id);
	virtual ~GameObject();

	virtual HRESULT Init(BaseProperty *property) = 0;
	virtual void Release(void) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos) = 0;

	virtual void Serialize(FileUtils::File &file) {}
	virtual void DeSerialize(FileUtils::File &file) {}

	virtual GameObject *Copy(ObjectId id) = 0;

	ObjectId GetId() { return _id; }

	TilePosition position;
	TilePosition desiredPosition;

protected :
	ObjectId _id;

};

#endif
