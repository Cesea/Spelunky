#include "stdafx.h"
#include "StageManager.h"

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
}

HRESULT StageManager::Init()
{
	_currentStage = new Stage;
	RandomRoomGenerated randomTypes = MakeRandomRoomTypes();

	_currentStage->InitFromRoomTypes(randomTypes);
	_currentStage->CalculateAllMask(0, 0, STAGE_TOTAL_COUNTX, STAGE_TOTAL_COUNTY);
	return S_OK;
}

void StageManager::Release()
{
	_currentStage->Release();
	SAFE_DELETE(_currentStage);
}

void StageManager::Update(float deltaTime)
{
}

void StageManager::Render()
{
	_currentStage->Render(_pCamera->GetPosition());
}


Stage * StageManager::GetCurrentStage()
{
	return _currentStage;
}

RandomRoomGenerated StageManager::MakeRandomRoomTypes()
{
	RandomRoomGenerated result{};
	result.startRoomIndex = IntVector2(RND->GetFromIntTo(0, 3), 0);

	result.roomTypes[GetIndexFromXY(result.startRoomIndex.x, result.startRoomIndex.y, 4)] = (RoomType)RND->GetFromIntTo(2, 4);

	IntVector2 prevPath(-1, -1);
	IntVector2 currentPath = result.startRoomIndex;

	Direction majorDirection = (Direction)RND->GetFromIntTo(0, 1);

	bool ended{ false };
	while (!ended)
	{
		IntVector2 delta{};
		RoomType type{};

		if (currentPath.x == 0 && prevPath.x == 0)
		{
			delta.x = 1;
			majorDirection = Direction::Right;
		}
		else if (currentPath.x == 3 && prevPath.x == 3)
		{
			delta.x = -1;
			majorDirection = Direction::Left;
		}
		else if (currentPath.x == 0 || currentPath.x == 3)
		{
			delta.y += 1;
		}
		else
		{
			int randomDirection = RND->GetFromIntTo(1, 5);
			if (randomDirection != 5)
			{
				if (majorDirection == Direction::Left)
				{
					delta.x = -1;
				}
				else if (majorDirection == Direction::Right)
				{
					delta.x = 1;
				}
			}
			else if (randomDirection == 5)
			{
				delta.y += 1;
			}
		}
		prevPath = currentPath;
		currentPath += delta;

		if (currentPath.y == 4)
		{
			ended = true;
			result.endRoomIndex = prevPath;
			continue;
		}

		type = (RoomType)RND->GetFromIntTo(2, 4);
		result.roomTypes[GetIndexFromXY(currentPath.x, currentPath.y, 4)] = type;
	}

	for (int i = 0; i < 16; ++i)
	{
		if (result.roomTypes[i] == RoomType::ROOM_NONE)
		{
			result.roomTypes[i] = RoomType::ROOM_BLOCK;
		}
	}
	return result;
}
