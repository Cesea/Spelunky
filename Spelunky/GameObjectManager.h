#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "singletonBase.h"
#include "GameObject.h"

#include "GameObjectFactory.h"
#include "Tile.h"

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

	GameObject *CreateObject(const std::wstring &key, BaseProperty *property);
	GameObject *CreateObject(const IEvent *event);
	GameObject *FindObjectId(ObjectId id);

	void DestroyObject(const IEvent *event);
	void DestroyObject(const ObjectId id);
	void DestroyAllObject();

	ObjectMap &GetObjectMapRef() { return _objects; }

private :
	ObjectMap _objects;

	GameObjectFactory _objectFactory;

	IScene *_currentScene{nullptr};

	friend class GameObject;

	void RegisterDelegates();
	void UnRegisterDelegates();

};


#endif
