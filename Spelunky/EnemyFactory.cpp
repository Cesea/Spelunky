#include "stdafx.h"
#include "EnemyFactory.h"

#include "Snake.h"

EnemyFactory::EnemyFactory()
{
}

EnemyFactory::~EnemyFactory()
{
}

HRESULT EnemyFactory::Init()
{
	RegisterBuilders();
	return S_OK;
}

void EnemyFactory::Release()
{
	UnRegisterBuilders();
}

GameObject * EnemyFactory::Build(const ObjectId id, EnemyType type, BaseProperty *property)
{
	GameObject *result = nullptr;
	EnemyBuilderIter &iter = _builders.find(type);
	if (iter != _builders.end())
	{
		result = iter->second->Build(id, property);
	}
	return result;
}

void EnemyFactory::RegisterBuilders()
{
	RegisterBuilder(EnemyType::ENEMY_Snake, new TEnemyBuilder<Snake>());
}

void EnemyFactory::UnRegisterBuilders()
{
	UnRegisterBuilder(EnemyType::ENEMY_Snake);
}

void EnemyFactory::RegisterBuilder(EnemyType type, EnemyBuilder *builder)
{
	EnemyBuilderIter &iter = _builders.find(type);
	if (iter == _builders.end())
	{
		_builders.insert(std::make_pair(type, builder));
	}
	else
	{
		Console::Log("Enemy Builder %d already exist", type);
	}
}

void EnemyFactory::UnRegisterBuilder(EnemyType type)
{
	EnemyBuilderIter &iter = _builders.find(type);
	if (iter != _builders.end())
	{
		_builders.erase(type);
	}
	else
	{
		Console::Log("Enemy Builder %d doesn'y exist", type);
	}
}
