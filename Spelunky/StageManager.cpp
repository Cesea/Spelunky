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
	if (_currentStage)
	{
		_currentStage->Update(deltaTime);

		if (!_inMiddleStage)
		{
			_currentStageElapsedTime += deltaTime;
			_totalStageElapsedTime += deltaTime;
		}
	}
}

void StageManager::Render(const Vector2 &offset)
{
	if (_currentStage)
	{
		const TilePosition &camPos = _pCamera->GetPosition();
		Vector2 &camPosUntiled = camPos.UnTilelize();
		camPosUntiled += offset;

		int minX = camPos.tileX;
		int maxX = camPos.tileX + 21;
		int minY = camPos.tileY;
		int maxY = camPos.tileY + 12;
		if (minX < 0) { minX = 0; }
		if (minY < 0) { minY = 0; }
		if (maxX > STAGE_TOTAL_COUNTX - 1) { maxX = STAGE_TOTAL_COUNTX - 1; }
		if (maxY > STAGE_TOTAL_COUNTY - 1) { maxY = STAGE_TOTAL_COUNTY - 1; }

		_currentStage->RenderDeeperLayer(minX, maxX, minY, maxY, camPosUntiled);

		_currentStage->RenderObjects(camPosUntiled);
		_pPlayer->Render(gRenderTarget, camPosUntiled);
		_currentStage->RenderBlockingLayer(minX, maxX, minY, maxY, camPosUntiled);

		_currentStage->Render(_pCamera->GetPosition());
	}
}

void StageManager::DestroyTile(const IntVector2 & tilePos)
{
	if (_currentStage)
	{
		_currentStage->DestroyTile(tilePos);
	}
}

void StageManager::DestroyTile(const int xStartIndex, const int yStartIndex, const int width, const int height)
{
	if (_currentStage)
	{
		_currentStage->DestroyTile(xStartIndex, yStartIndex, width, height);
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

void StageManager::Reset()
{
	_currentStage->Release();
	SAFE_DELETE(_currentStage);

	_timeElapsedInStage= 0;
	_currentStageCount= 0;

	_inMiddleStage = true;

	_currentStageElapsedTime = 0 ;
	_totalStageElapsedTime = 0 ;

	_normalStageMoneyCollected= 0 ;
}

RandomRoomGenerated StageManager::MakeRandomRoomTypes()
{
	RandomRoomGenerated result{};
	result.startRoomIndex = IntVector2(RND->GetFromIntTo(0, 3), 0);

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
			result.roomTypes[GetIndexFromXY(prevPath.x, prevPath.y, 4)] = RoomType::ROOM_BOTTOM_OPEN;
			type = RoomType::ROOM_TOP_OPEN;
		}
		else if (currentPath.x == 3 && prevPath.x == 3)
		{
			delta.x = -1;
			majorDirection = Direction::Left;
			result.roomTypes[GetIndexFromXY(prevPath.x, prevPath.y, 4)] = RoomType::ROOM_BOTTOM_OPEN;
			type = RoomType::ROOM_TOP_OPEN;
		}
		else if (currentPath.x == 0 || currentPath.x == 3)
		{
			delta.y += 1;
			majorDirection = Direction::Right;
			type = RoomType::ROOM_BOTTOM_OPEN;
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
				type = RoomType::ROOM_AISLE;
			}
			else if (randomDirection == 5)
			{
				delta.y += 1;
				type = RoomType::ROOM_BOTTOM_OPEN;
			}
		}
		prevPath = currentPath;

		if (currentPath.y == 3)
		{
			int a = 0; 
		}

		result.roomTypes[GetIndexFromXY(currentPath.x, currentPath.y, 4)] = type;
		currentPath += delta;

		Console::Log("%d, %d\n", currentPath.x, currentPath.y);

		if (currentPath.y == 4)
		{
			ended = true;
			result.exitRoomIndex = prevPath;
			currentPath.y = 3;
			break;
		}
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
	_normalStageMoneyCollected = _currentStage->GetMoneyCollected();
	SAFE_DELETE(_currentStage);

	_inMiddleStage = true;
	

	Stage *newStage = new Stage;
	std::wstring firstKey{};
	firstKey = L"mine";
	newStage->InitForMiddleStage(firstKey);
	newStage->CalculateAllMask(0, 0, STAGE_TOTAL_COUNTX, STAGE_TOTAL_COUNTY);

	_currentStage = newStage;

	_pPlayer->position.tileX = _currentStage->GetStartPosition().x;
	_pPlayer->position.tileY = _currentStage->GetStartPosition().y + 1 ;
	_pPlayer->desiredPosition = _pPlayer->position;

	TilePosition exitPositionToPlayer = _currentStage->GetCurrentExitPosition();
	exitPositionToPlayer.AddToTileRelY(64);
	_pPlayer->SetExitPosition(exitPositionToPlayer);

	EVENTMANAGER->FireEvent(new PlayerGoExitEvent(true));
	EVENTMANAGER->FireEvent(new OnMiddleStageEvent);
	_pCamera->ResetForMiddleStage();

	Console::Log("%d\n", OBJECTMANAGER->GetObjectMapRef().size());
}

void StageManager::BuildNextStage()
{
	SAFE_DELETE(_currentStage);
	EVENTMANAGER->FireEvent(new ExitMiddleStageEvent());
	_currentStageElapsedTime = 0;

	_inMiddleStage = false;
	Stage *newStage = new Stage;
	RandomRoomGenerated randomTypes = MakeRandomRoomTypes();

	std::wstring firstKey{};
		firstKey = L"mine";

	newStage->InitFromRoomTypes(firstKey, randomTypes);
	newStage->CalculateAllMask(0, 0, STAGE_TOTAL_COUNTX, STAGE_TOTAL_COUNTY);

	_currentStage = newStage;

	_currentStageCount++;

	_pPlayer->position.tileX = _currentStage->GetStartPosition().x;
	_pPlayer->position.tileY = _currentStage->GetStartPosition().y + 1;
	_pPlayer->position.AddToTileRelX(20);
	_pPlayer->position.AddToTileRelY(-10);

	_pPlayer->desiredPosition = _pPlayer->position;


	TilePosition exitPositionToPlayer = _currentStage->GetCurrentExitPosition();
	exitPositionToPlayer.AddToTileRelY(64);
	_pPlayer->SetExitPosition(exitPositionToPlayer);
	_pCamera->ResetForNormalStage();
	_currentStage->PostInit();

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
	EVENTMANAGER->FireEvent(new LayerOnEvent(true, false, false, _pPlayer->position));
}
