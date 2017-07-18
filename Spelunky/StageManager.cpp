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
		RandomRoomGenerated randomTypes = MakeRandomRoomTypes();

		_currentStage->InitFromRoomTypes(randomTypes);
		_currentStage->CalculateAllMask(0, 0, STAGE_TOTAL_COUNTX, STAGE_TOTAL_COUNTY);
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

	void StageManager::RenderBorderLayer()
	{
		_currentStage->RenderBorderLayer(_pCamera->GetPosition());
	}

	void StageManager::RenderTileLayer()
	{
		_currentStage->RenderTileLayer(_pCamera->GetPosition());
	}

	void StageManager::RenderMaskLayer()
	{
		_currentStage->RenderMaskLayer(_pCamera->GetPosition());
	}

	Stage * PlayScene::StageManager::GetCurrentStage()
	{
		return _currentStage;
	}

	RandomRoomGenerated StageManager::MakeRandomRoomTypes()
	{
		RandomRoomGenerated result{};
		result.startRoomIndex = IntVector2(RND->GetFromIntTo(0, 3), 0);

		result.roomTypes[GetIndexFromXY(result.startRoomIndex.x, result.startRoomIndex.y, 4)] = (RoomType)RND->GetFromIntTo(1, 3);
		
		IntVector2 prevPath(-1, -1);
		IntVector2 currentPath = result.startRoomIndex;
		bool ended{ false };
		while (!ended)
		{
			IntVector2 delta{};
			RoomType type{};

			if (currentPath.x == 0 && prevPath.x == 0)
			{
				delta.x = 1;
			}
			else if (currentPath.x == 3 && prevPath.x == 3)
			{
				delta.x = -1;
			}
			else if(currentPath.x == 0 || currentPath.x == 3)
			{
				delta.y += 1;
			}
			else
			{
				int randomDirection = RND->GetFromIntTo(1, 5);
				if (randomDirection == 1 || randomDirection == 2)
				{
					delta.x -= 1;
				}
				else if (randomDirection == 3 || randomDirection == 4)
				{
					delta.x += 1;
				}
				else if (randomDirection == 5)
				{
					delta.y += 1;
				}
			}
			currentPath += delta;

			if (currentPath.y == 4)
			{
				ended = true;
				continue;
			}

			type = (RoomType)RND->GetFromIntTo(1, 3);
			result.roomTypes[GetIndexFromXY(currentPath.x, currentPath.y, 4)] = type;

			prevPath = currentPath;
		}
	return result;
	}
}
