#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "gameNode.h"

#include "D2DSprite.h"
#include "D2DAnimationSprite.h"

class GameObject : public gameNode
{
public :
	GameObject(ObjectId id);
	virtual ~GameObject();

	virtual HRESULT Init(ArcheType type);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id) = 0;
	virtual void HandleMessage(const IEvent *event) = 0;

	ObjectId GetId() { return _id; }

	TilePosition position;
	TilePosition desiredPosition;

protected :
	ObjectId _id;

	ArcheType _type;
};

#endif
