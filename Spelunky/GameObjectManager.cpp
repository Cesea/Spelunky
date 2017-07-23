#include "stdafx.h"
#include "GameObjectManager.h"

#include "IScene.h"

#include "Tile.h"

#include "Player.h"


GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}

HRESULT GameObjectManager::Init()
{
	RegisterDelegates();
	_objectFactory.Init();
	return S_OK;
}

void GameObjectManager::Relase()
{
	UnRegisterDelegates();
	_objectFactory.Release();
}

GameObject * GameObjectManager::CreateObject(const std::wstring & key, BaseProperty * property)
{
	GameObject *result = _objectFactory.Build(_currentScene->GetNextId(), key, property);
	_objects.insert(std::make_pair(result->GetId(), result));
	return result;
}

GameObject * GameObjectManager::CreateObject(const IEvent * event)
{
	CreateObjectEvent *convertedEvent = (CreateObjectEvent *)(event);
	GameObject *result = _objectFactory.Build(_currentScene->GetNextId(), convertedEvent->GetKey(), convertedEvent->GetProperty());
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
		delete found->second;
		found->second = nullptr;
		_objects.erase(found);
	}
}

void GameObjectManager::DestroyObject(const ObjectId id)
{
	auto &found = _objects.find(id);
	if (found != _objects.end())
	{
		delete found->second;
		_objects.erase(found);
	}
}

void GameObjectManager::DestroyAllObject()
{
	for (auto &object : _objects)
	{
		delete object.second;
	}
	_objects.clear();
}

void GameObjectManager::RegisterDelegates()
{
	EVENTMANAGER->RegisterDelegate(EVENT_CREATE_OBJECT, EventDelegate::FromFunction<GameObjectManager, &GameObjectManager::DestroyObject>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_DESTROY_OBJECT, EventDelegate::FromFunction<GameObjectManager, &GameObjectManager::DestroyObject>(this));
}

void GameObjectManager::UnRegisterDelegates()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_CREATE_OBJECT, EventDelegate::FromFunction<GameObjectManager, &GameObjectManager::DestroyObject>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_DESTROY_OBJECT, EventDelegate::FromFunction<GameObjectManager, &GameObjectManager::DestroyObject>(this));
}
