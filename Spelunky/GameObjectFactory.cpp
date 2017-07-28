#include "stdafx.h"
#include "GameObjectFactory.h"

#include "Tile.h"
#include "Tunnel.h"
#include "Player.h"
#include "Gem.h"
#include "Bomb.h"
#include "Throws.h"
#include "Enemy.h"
#include "Crate.h"
#include "Eatables.h"
#include "Arrow.h"

GameObjectFactory::GameObjectFactory()
{
}

GameObjectFactory::~GameObjectFactory()
{
}

HRESULT GameObjectFactory::Init()
{
	RegisterBuilders();
	_enemyFactory.Init();
	_obstacleFactory.Init();
	return S_OK;
}

void GameObjectFactory::Release()
{
	UnRegisterBuilders();
	_enemyFactory.Release();
	_obstacleFactory.Release();
}

GameObject * GameObjectFactory::Build(const ObjectId id, const std::wstring &key, BaseProperty *property)
{
	GameObject *result = nullptr;
	GameObjectBuilderIter &iter = _builders.find(key);
	if (iter != _builders.end())
	{
		if (iter->first.compare(L"enemy") == 0)
		{
			EnemyProperty *convertedProperty = (EnemyProperty *)(property);
			result = _enemyFactory.Build(id, convertedProperty->type, convertedProperty);
		}
		else if (iter->first.compare(L"obstacle") == 0)
		{
			ObstacleProperty *convertedProperty = (ObstacleProperty *)(property);
			result = _obstacleFactory.Build(id, convertedProperty->type, convertedProperty);
		}
		else
		{
			result = iter->second->Build(id, property);
		}
	}
	return result;
}

void GameObjectFactory::RegisterBuilders()
{
	RegisterBuilder(L"tile", new TGameObjectBuilder<Tile>());
	RegisterBuilder(L"tunnel", new TGameObjectBuilder<Tunnel>());
	RegisterBuilder(L"player", new TGameObjectBuilder<Player>());
	RegisterBuilder(L"gem", new TGameObjectBuilder<Gem>());
	RegisterBuilder(L"bomb", new TGameObjectBuilder<Bomb>());
	RegisterBuilder(L"throws", new TGameObjectBuilder<Throws>());
	RegisterBuilder(L"enemy", new TGameObjectBuilder<Enemy>());
	RegisterBuilder(L"crate", new TGameObjectBuilder<Crate>());
	RegisterBuilder(L"eatables", new TGameObjectBuilder<Eatables>());
	RegisterBuilder(L"obstacle", new TGameObjectBuilder<Obstacle>());
	RegisterBuilder(L"arrow", new TGameObjectBuilder<Arrow>());
}

void GameObjectFactory::UnRegisterBuilders()
{
	UnRegisterBuilder(L"tile");
	UnRegisterBuilder(L"tunnel");
	UnRegisterBuilder(L"player");
	UnRegisterBuilder(L"gem");
	UnRegisterBuilder(L"bomb");
	UnRegisterBuilder(L"throws");
	UnRegisterBuilder(L"enemy");
	UnRegisterBuilder(L"crate");
	UnRegisterBuilder(L"eatables");
	UnRegisterBuilder(L"obstacle");
	UnRegisterBuilder(L"arrow");
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
		//Console::Log("Builder %s already exist", key);
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
		//Console::Log("Builder %s doesn'y exist", key);
	}
}
