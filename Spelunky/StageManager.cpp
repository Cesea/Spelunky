#include "stdafx.h"
#include "StageManager.h"

namespace PlayScene
{
	PlayScene::StageManager::StageManager()
	{
	}

	PlayScene::StageManager::~StageManager()
	{
	}

	HRESULT PlayScene::StageManager::Init()
	{
		_currentStage = new Stage;
		RoomType roomTypes[16]{};
		_currentStage->InitFromRoomTypes(roomTypes);
		//_stage.CalculateMask(0, 0, STAGE_TILE_COUNTX, STAGE_TILE_COUNTY);
		return S_OK;
	}

	void PlayScene::StageManager::Release()
	{
		_currentStage->Release();
		SAFE_DELETE(_currentStage);
	}

	void StageManager::Update(float deltaTime)
	{
	}

	void StageManager::RenderTileLayer()
	{
		_currentStage->RenderTileLayer(TilePosition());
	}

	void StageManager::RenderMaskLayer()
	{
		_currentStage->RenderMaskLayer(TilePosition());
	}

	Stage * PlayScene::StageManager::GetCurrentStage()
	{
		return _currentStage;
	}
}
