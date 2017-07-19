#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "singletonBase.h"
#include "GameObject.h"
#include <unordered_map>
#include "Tile.h"

class IScene;

#define UNVALID_OBJECT_ID 0

class GameObjectBuilder
{
public :
	virtual ~GameObjectBuilder() {}
	virtual GameObject *Build(ObjectId id, TileProperty property) = 0;
};

template<typename T>
class TGameObjectBuilder
{
public :
	virtual ~TGameObjectBuilder() {}
	T *Build(ObjectId id, TileProperty property) override
	{
		T *result = new T(id);
		result->Init(property);
		return result;
	}
};

typedef std::unordered_map<ObjectId, GameObject *> ObjectMap;
typedef std::unordered_map<TileProperty, GameObjectBuilder *> ObjectBuilderMap;

namespace PlayScene
{
	struct Property;
}

class GameObjectManager : public singletonBase<GameObjectManager>
{
public :
	GameObjectManager();
	~GameObjectManager();

	HRESULT Init();
	void Relase();

	void SetCurrentScene(IScene *scene) { _currentScene = scene; }
	void DetachCurrentScene() { _currentScene = nullptr; }

	GameObject *CreateObject(ArcheType type);
	GameObject *CreateObjectFromProperty(PlayScene::Property *property);
	GameObject *FindObjectId(ObjectId id);

	void DestroyObject(const IEvent *event);

	ObjectMap &GetObjectMapRef() { return _objects; }

private :
	ObjectMap _objects;
	ObjectBuilderMap _builders;

	IScene *_currentScene{nullptr};

	friend class GameObject;

	void RegisterDelegates();
	void UnRegisterDelegates();

};


#endif
