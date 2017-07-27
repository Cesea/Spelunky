#ifndef OBSTACLE_FACTORY_H
#define OBSTACLE_FACTORY_H

#include "Property.h"
#include <unordered_map>
#include "singletonBase.h"

class ObstacleBuilder
{
public:
	virtual ~ObstacleBuilder() {}
	virtual GameObject *Build(ObjectId id, BaseProperty *property) = 0;
};

template<typename T>
class TObstacleBuilder : public ObstacleBuilder
{
public:
	virtual ~TObstacleBuilder() {}
	T *Build(ObjectId id, BaseProperty *property) override
	{
		T *result = new T(id);
		result->Init(property);
		return result;
	}
};

class ObstacleFactory
{
public:
	typedef std::unordered_map<ObstacleType, ObstacleBuilder *> ObstacleBuilderMap;

	ObstacleFactory();
	virtual ~ObstacleFactory();

	HRESULT Init();
	void Release();

	GameObject *Build(const ObjectId id, ObstacleType type, BaseProperty *property);

private:
	void RegisterBuilders();
	void UnRegisterBuilders();

	void RegisterBuilder(ObstacleType type, ObstacleBuilder *builder);
	void UnRegisterBuilder(ObstacleType type);

	ObstacleBuilderMap _builders;
};

#endif