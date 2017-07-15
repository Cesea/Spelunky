#include "stdafx.h"
#include "GameObjectManager.h"

#include "IScene.h"

#include "Player.h"

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
		result = new Player(scene->GetNextId());
		result->Init(ArcheType::Player);
	}break;
	case ArcheType::Tile : 
	{
	}break;
	case ArcheType::MineBG : 
	{
	}break;
	}
	_objects.insert(std::make_pair(result->GetId(), result));
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
