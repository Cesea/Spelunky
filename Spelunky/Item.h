#ifndef ITEM_H
#define ITEM_H

#include "MovingObject.h"

//#include "CollisionComponent.h"
class CollisionComponent;

#include "Room.h"

class Item : public MovingObject
{
public :
	Item(ObjectId id);
	virtual ~Item();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void Apply(ObjectId id) = 0;

	void HandlePlayerPositionEvent(const IEvent *event);

protected  :
	D2DSprite *_sprite{};

	ReturnTile _nearTiles{};

	bool32 _actorOn{false};
	ObjectId _onActorId{ 0 };
	bool32 _valid{true};

	CollisionComponent *_collisionComp{};
};

#endif