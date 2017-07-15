#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "singletonBase.h"
#include "GameObject.h"
#include <unordered_map>

class IScene;

#define UNVALID_OBJECT_ID 0

typedef std::unordered_map<ObjectId, GameObject *> ObjectMap;

class GameObjectManager : public singletonBase<GameObjectManager>
{
public :
	GameObjectManager();
	~GameObjectManager();

	HRESULT Init();
	void Relase();

	void SetCurrentScene(IScene *scene) { _currentScene = scene; }
	void DetachCurrentScene() { _currentScene = nullptr; }

	GameObject *CreateObject(IScene *scene, ArcheType type);

	void DestroyObject(IScene *scene, GameObject *object);
	void DestroyObject(IScene *scene, ObjectId id);
	void DestroyObject(IScene *scene, ArcheType type);

	ObjectMap &GetObjectMapRef() { return _objects; }

private :
	ObjectMap _objects;

	IScene *_currentScene{0};

	friend class GameObject;

};


#endif
