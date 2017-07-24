#ifndef ENEMY_FACTORY_H
#define ENEMY_FACTORY_H

#include "Property.h"
#include <unordered_map>
#include "singletonBase.h"

class EnemyBuilder
{
public:
	virtual ~EnemyBuilder() {}
	virtual GameObject *Build(ObjectId id, BaseProperty *property) = 0;
};

template<typename T>
class TEnemyBuilder : public EnemyBuilder
{
public:
	virtual ~TEnemyBuilder() {}
	T *Build(ObjectId id, BaseProperty *property) override
	{
		T *result = new T(id);
		result->Init(property);
		return result;
	}
};

class EnemyFactory
{
public:
	typedef std::unordered_map<EnemyType, EnemyBuilder *> EnemyBuilders;
	typedef std::unordered_map<EnemyType, EnemyBuilder *>::iterator EnemyBuilderIter;

	EnemyFactory();
	virtual ~EnemyFactory();

	HRESULT Init();
	void Release();

	GameObject *Build(const ObjectId id, EnemyType type, BaseProperty *property);

private:
	void RegisterBuilders();
	void UnRegisterBuilders();

	void RegisterBuilder(EnemyType type, EnemyBuilder *builder);
	void UnRegisterBuilder(EnemyType type);

	EnemyBuilders _builders;
};

#endif