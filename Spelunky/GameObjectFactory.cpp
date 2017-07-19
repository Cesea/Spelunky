#include "stdafx.h"
#include "GameObjectFactory.h"

#include "Tile.h"

GameObjectFactory::GameObjectFactory()
{
}

GameObjectFactory::~GameObjectFactory()
{
}

HRESULT GameObjectFactory::Init()
{
	RegisterBuilders();
	return S_OK;
}

void GameObjectFactory::Release()
{
	UnRegisterBuilders();
}

GameObject * GameObjectFactory::Build(const ObjectId id, const std::wstring &key, BaseProperty *property)
{
	GameObject *result = nullptr;
	GameObjectBuilderIter &iter = _builders.find(key);
	if (iter != _builders.end())
	{
		result = iter->second->Build(id, property);
	}
	return result;
}

void GameObjectFactory::RegisterBuilders()
{
	RegisterBuilder(L"tile", new TGameObjectBuilder<Tile>());
}

void GameObjectFactory::UnRegisterBuilders()
{
	UnRegisterBuilder(L"tile");
}

void GameObjectFactory::RegisterBuilder(const std::wstring & key, GameObjectBuilder * builder)
{
GameObjectBuilderIter &iter = _builders.find(key);
	if (iter == _builders.end())
	{
		_builders.insert(std::make_pair(key, builder));
	}
	else
	{
		Console::Log("Builder %s already exist", key);
	}
}

void GameObjectFactory::UnRegisterBuilder(const std::wstring & key)
{
	GameObjectBuilderIter &iter = _builders.find(key);
	if (iter != _builders.end())
	{
		_builders.erase(key);
	}
	else
	{
		Console::Log("Builder %s doesn'y exist", key);
	}
}
