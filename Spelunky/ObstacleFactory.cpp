#include "stdafx.h"
#include "ObstacleFactory.h"

#include "BombCrate.h"
#include "PushingRock.h"
#include "ArrowRock.h"

ObstacleFactory::ObstacleFactory()
{
}

ObstacleFactory::~ObstacleFactory()
{
}

HRESULT ObstacleFactory::Init()
{
	RegisterBuilders();
	return S_OK;
}

void ObstacleFactory::Release()
{
	UnRegisterBuilders();
}

GameObject * ObstacleFactory::Build(const ObjectId id, ObstacleType type, BaseProperty * property)
{
	GameObject *result = nullptr;
	auto&iter = _builders.find(type);
	if (iter != _builders.end())
	{
		result = iter->second->Build(id, property);
	}
	return result;

}

void ObstacleFactory::RegisterBuilders()
{
	RegisterBuilder(ObstacleType::OBSTACLE_ArrowRock, new TObstacleBuilder<ArrowRock>());
	RegisterBuilder(ObstacleType::OBSTACLE_PushingRock, new TObstacleBuilder<PushingRock>());
	RegisterBuilder(ObstacleType::OBSTACLE_BombCrate, new TObstacleBuilder<BombCrate>());
}

void ObstacleFactory::UnRegisterBuilders()
{
	UnRegisterBuilder(ObstacleType::OBSTACLE_BombCrate);
	UnRegisterBuilder(ObstacleType::OBSTACLE_PushingRock);
	UnRegisterBuilder(ObstacleType::OBSTACLE_ArrowRock);
}

void ObstacleFactory::RegisterBuilder(ObstacleType type, ObstacleBuilder * builder)
{
	auto &iter = _builders.find(type);
	if (iter == _builders.end())
	{
		_builders.insert(std::make_pair(type, builder));
	}
	else
	{
		Console::Log("Obstacle Builder %d already exist", type);
	}
}

void ObstacleFactory::UnRegisterBuilder(ObstacleType type)
{
	auto &iter = _builders.find(type);
	if (iter != _builders.end())
	{
		_builders.erase(type);
	}
	else
	{
		Console::Log("Obstacle Builder %d doesn'y exist", type);
	}
}
