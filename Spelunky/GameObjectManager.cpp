#include "stdafx.h"
#include "GameObjectManager.h"

#include "IScene.h"

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}

HRESULT GameObjectManager::Init()
{
	return S_OK;
}

void GameObjectManager::Relase()
{
}

GameObject * GameObjectManager::CreateObject(IScene *scene, ArcheType type)
{
	GameObject *result = nullptr;
	switch (type)
	{
	case ArcheType::Player :
	{
	}break;
	case ArcheType::Tile : 
	{
	}break;
	case ArcheType::MineBG : 
	{
	}break;
	}
	if (result != nullptr)
	{
		scene->_objects.insert(std::make_pair(result->GetId(), result));
	}
	return result;
}

void GameObjectManager::DestroyObject(IScene *scene, GameObject * object)
{
}

void GameObjectManager::DestroyObject(IScene *scene, ObjectId id)
{
}

void GameObjectManager::DestroyObject(IScene *scene, ArcheType type)
{
}
