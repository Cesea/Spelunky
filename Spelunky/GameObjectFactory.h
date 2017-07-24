#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "Property.h"

#include "singletonBase.h"
#include "EnemyFactory.h"

class GameObjectBuilder
{
public :
	virtual ~GameObjectBuilder() {}
	virtual GameObject *Build(ObjectId id, BaseProperty *property) = 0;
};

template<typename T>
class TGameObjectBuilder : public GameObjectBuilder
{
public :
	virtual ~TGameObjectBuilder() {}
	T *Build(ObjectId id, BaseProperty *property) override
	{
		T *result = new T(id);
		result->Init(property);
		return result;
	}
};

class GameObjectFactory 
{
public:
	typedef std::unordered_map<std::wstring, GameObjectBuilder *> GameObjecetBuilder;
	typedef std::unordered_map<std::wstring, GameObjectBuilder *>::iterator GameObjectBuilderIter;

	GameObjectFactory();
	virtual ~GameObjectFactory();

	HRESULT Init();
	void Release();

	GameObject *Build(const ObjectId id, const std::wstring &key, BaseProperty *property);

private:
	void RegisterBuilders();
	void UnRegisterBuilders();

	void RegisterBuilder(const std::wstring &key, GameObjectBuilder *builder);
	void UnRegisterBuilder(const std::wstring &key);

	GameObjecetBuilder _builders;

	EnemyFactory _enemyFactory;
};

#endif