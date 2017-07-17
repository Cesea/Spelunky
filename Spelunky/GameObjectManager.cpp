#include "stdafx.h"
#include "GameObjectManager.h"

#include "IScene.h"

#include "Player.h"

#include "Gem.h"
#include "Rock.h"

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}

HRESULT GameObjectManager::Init()
{
	RegisterDelegates();
	return S_OK;
}

void GameObjectManager::Relase()
{
	UnRegisterDelegates();
}

GameObject * GameObjectManager::CreateObject(IScene *scene, ArcheType type)
{
	GameObject *result = nullptr;
	switch (type)
	{
	case ArcheType::Player:
	{
		result = new Player(scene->GetNextId());
		result->Init(ArcheType::Player);
	}break;
	case ArcheType::Tile:
	{
	}break;
	case ArcheType::MineBG:
	{
	}break;
	case ArcheType::Gem:
	{
		Gem *result = new Gem(scene->GetNextId());
		result->Init(ArcheType::Gem, 10);
		_objects.insert(std::make_pair(result->GetId(), result));
		return result;
	}
	case ArcheType::Rock :
	{
		Rock *result = new Rock	 (scene->GetNextId());
		result->Init(ArcheType::Rock);
		_objects.insert(std::make_pair(result->GetId(), result));
		return result;
	}break;
	}
	_objects.insert(std::make_pair(result->GetId(), result));
	return result;
}

GameObject * GameObjectManager::FindObjectId(ObjectId id)
{
	GameObject *result = nullptr;
	auto &found = _objects.find(id);
	if (found != _objects.end())
	{
		result = found->second;
	}
	return result;
}

void GameObjectManager::DestroyObject(const IEvent * event)
{
	DestroyObjectEvent *convertedEvent = (DestroyObjectEvent *)(event);
	auto &found = _objects.find(convertedEvent->GetId());
	if (found != _objects.end())
	{
		_objects.erase(found);
	}
}

void GameObjectManager::RegisterDelegates()
{
	EVENTMANAGER->RegisterDelegate(EVENT_DESTROY_OBJECT, EventDelegate::FromFunction<GameObjectManager, &GameObjectManager::DestroyObject>(this));
}

void GameObjectManager::UnRegisterDelegates()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_DESTROY_OBJECT, EventDelegate::FromFunction<GameObjectManager, &GameObjectManager::DestroyObject>(this));
}
