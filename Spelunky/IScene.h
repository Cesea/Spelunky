#ifndef SCENE_H
#define SCENE_H

#include "D2DAnimationSprite.h"

#include "Camera.h"
#include "Command.h"

#include "GameObjectManager.h"

class IScene 
{
	friend class GameObjectManager;
public:
	virtual ~IScene() {}

	virtual HRESULT Init(void) = 0;
	virtual void Release(void) = 0;
	virtual void Update(void) = 0;
	virtual void Render() = 0;

	virtual HRESULT LoadContent() = 0;

protected:
	ObjectMap _objects;

	InputMapper _inputMapper;

	ObjectId _playerId;

	ObjectId _lastId{1};
	ObjectId GetNextId() { return _lastId++; }
};


#endif