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
	EVENTMANAGER->RegisterDelegate(EVENT_STAGE_TRANSITIOIN,
		EventDelegate::FromFunction<StageManager, &StageManager::HandleStageTransitionEvent>(STAGEMANAGER));

	return S_OK;
}

void StageManager::Release()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_STAGE_TRANSITIOIN,
		EventDelegate::FromFunction<StageManager, &StageManager::HandleStageTransitionEvent>(STAGEMANAGER));

	_currentStage->Release();
	SAFE_DELETE(_currentStage);
}

void StageManager::Update(float deltaTime)
{
}

void StageManager::Render()
{
	if (_currentStage)
	{
		_currentStage->Render(_pCamera->GetPosition());
	}
}


Stage * StageManager::GetCurrentStage()
{
	if (_currentStage)
	{
		return _currentStage;
	}
	return nullptr;
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
			result.exitRoomIndex = prevPath;
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

void StageManager::BuildMiddleStage()
{
	SAFE_DELETE(_currentStage);

	_inMiddleStage = true;

	Stage *newStage = new Stage;
	std::wstring firstKey{};
	if ((_currentStageCount / 4) == 0)
	{
		firstKey = L"mine";
	}
	else if ((_currentStageCount / 4) == 1)
	{
		firstKey = L"jungle";
	}
	else if ((_currentStageCount / 4) == 2)
	{
		firstKey = L"temple";
	}
	newStage->InitForMiddleStage(firstKey);
	newStage->CalculateAllMask(0, 0, STAGE_TOTAL_COUNTX, STAGE_TOTAL_COUNTY);

	_currentStage = newStage;

	_pPlayer->position.tileX = _currentStage->GetStartPosition().x;
	_pPlayer->position.tileY = _currentStage->GetStartPosition().y ;
	_pPlayer->position.AddToTileRelY(-20);

	_pPlayer->desiredPosition = _pPlayer->position;
	Console::Log("%d\n", OBJECTMANAGER->GetObjectMapRef().size());
}

void StageManager::BuildNextStage()
{
	SAFE_DELETE(_currentStage);

	_inMiddleStage = false;
	Stage *newStage = new Stage;
	RandomRoomGenerated randomTypes = MakeRandomRoomTypes();

	std::wstring firstKey{};
	if ((_currentStageCount / 4) == 0)
	{
		firstKey = L"mine";
	}
	else if ((_currentStageCount / 4) == 1)
	{
		firstKey = L"jungle";
	}
	else if ((_currentStageCount / 4) == 2)
	{
		firstKey = L"temple";
	}
	newStage->InitFromRoomTypes(firstKey, randomTypes);
	newStage->CalculateAllMask(0, 0, STAGE_TOTAL_COUNTX, STAGE_TOTAL_COUNTY);

	_currentStage = newStage;

	_currentStageCount++;

	_pPlayer->position.tileX = _currentStage->GetStartPosition().x;
	_pPlayer->position.tileY = _currentStage->GetStartPosition().y ;
	_pPlayer->position.AddToTileRelY(-20);

	_pPlayer->desiredPosition = _pPlayer->position;

	Console::Log("%d\n", OBJECTMANAGER->GetObjectMapRef().size());
}

void StageManager::HandleStageTransitionEvent(const IEvent * event)
{
	if (_inMiddleStage)
	{
		BuildNextStage();
	}
	else
	{
		BuildMiddleStage();
	}
	TIMEMANAGER->Tick();
}
