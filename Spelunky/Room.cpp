#include "stdafx.h"
#include "Room.h"

#include "StageManager.h"

#include "TileInfo.h"

IntVector2 CalculateBorderWorldIndex(int indexX, int indexY, int localX, int localY)
{
	IntVector2 result = {};
	if (indexX == 0 && indexY == 0)
	{
		result.x = localX;
		result.y = localY;
	}
	else if(indexX == 4 && indexY == 0)
	{
		result.x = STAGE_TOTAL_COUNTX - ROOM_TILE_COUNTX + localX;
		result.y = localY;
	}
	else if (indexX == 4 && indexY == 4)
	{
		result.x = STAGE_TOTAL_COUNTX - ROOM_TILE_COUNTX + localX;
		result.y = STAGE_TOTAL_COUNTY - ROOM_TILE_COUNTY + localY;
	}
	else if (indexX == 0 && indexY == 4)
	{
		result.x = localX;
		result.y = STAGE_TOTAL_COUNTY - ROOM_TILE_COUNTY + localY;
	}
	else
	{
		if (indexY == 0 || indexY == 4)
		{
			result.x = 8 + (indexX- 1) * ROOM_TILE_COUNTX + localX;
			if (indexY == 0)
			{
				result.y = localY;
			}
			else
			{
				result.y = STAGE_TOTAL_COUNTY - ROOM_TILE_COUNTY + localY;
			}
		}
		else if (indexX == 0 || indexX == 4)
		{
			result.y = 7 + ((indexY - 1 ) * ROOM_TILE_COUNTY) + localY;
			if (indexX == 0)
			{
				result.x = localX;
			}
			else
			{
				result.x = STAGE_TOTAL_COUNTX - ROOM_TILE_COUNTX + localX;
			}
		}
	}
	return result;
}
IntVector2 CalculateTileWorldIndex(int indexX, int indexY, int localX, int localY)
{
	IntVector2 result = {};

	int xTileStartIndex = indexX * ROOM_TILE_COUNTX + 3;
	int yTileStartIndex = indexY * ROOM_TILE_COUNTY + 3;

	result.x = xTileStartIndex + localX;
	result.y = yTileStartIndex + localY;
	return result;
}

Stage::Stage()
{
	//_gems.reserve(100);
	RegisterDelegates();
}

Stage::~Stage()
{
	UnRegisterDelegates();
	for (int y = 0; y < STAGE_TOTAL_COUNTY; ++y)
	{
		for (int x = 0; x < STAGE_TOTAL_COUNTX; ++x)
		{
			if (tileLayer0[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)])
			{
				OBJECTMANAGER->DestroyObject(tileLayer0[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)]->GetId());
			}
			if (tileLayer1[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)])
			{
				OBJECTMANAGER->DestroyObject(tileLayer1[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)]->GetId());
			}
			if (tileLayer2[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)])
			{
				OBJECTMANAGER->DestroyObject(tileLayer2[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)]->GetId());
			}
		}
	}
	_tunnels[0]->Release();
	_tunnels[1]->Release();
	OBJECTMANAGER->DestroyObject(_tunnels[0]->GetId());
	OBJECTMANAGER->DestroyObject(_tunnels[1]->GetId());

	for (auto &gem : _gems)
	{
		OBJECTMANAGER->DestroyObject(gem->GetId());
	}

	for (auto &sprite : _usingSprites)
	{
		SAFE_DELETE(sprite.second);
	}
	_usingSprites.clear();
	//SAFE_DELETE(_backgroundSprite);
}

HRESULT Stage::Init()
{
	return S_OK;
}

HRESULT Stage::InitFromRoomTypes(const std::wstring &firstKey, const RandomRoomGenerated &randomTypes)
{
	_backgroundSprite = new D2DSprite;
	_backgroundSprite->Init(IMAGEMANAGER->GetImage(L"minebg"), 0, 0, 256, 256, IntVector2(0, 0));

	BuildBorder();

	_stageBuildInfo = randomTypes;

	std::wstring buffer = L"";
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			buffer = firstKey;
			if (GetRandomFileNameFromRoomType(randomTypes.roomTypes[GetIndexFromXY(x, y, 4)], buffer))
			{
				_rooms[GetIndexFromXY(x, y, 4)].roomType = randomTypes.roomTypes[GetIndexFromXY(x, y, 4)];
				BuildRoomFromFile(buffer, &_rooms[GetIndexFromXY(x, y, 4)],x, y, false, _usingSprites);
			}
			else
			{
				Console::Log("Room %d, %d build failed\n", x, y);
			}
		}
	}
	buffer.clear();

	BuildEntrance();
	BuildGems();
	BuildThrows();

	return S_OK;
}

HRESULT Stage::InitForMiddleStage(const std::wstring firstKey)
{
	_backgroundSprite = new D2DSprite;
	_backgroundSprite->Init(IMAGEMANAGER->GetImage(L"minebg"), 0, 0, 256, 256, IntVector2(0, 0));

	BuildBorder();

	_stageBuildInfo.startRoomIndex = IntVector2(1, 1);
	_stageBuildInfo.exitRoomIndex = IntVector2(2, 1);

	std::wstring buffer = L"";

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			buffer += firstKey;
			if (x == 1 && y == 1)
			{
				buffer += L"_middle_left.rt";
			}
			else if (x == 2 && y == 1)
			{
				buffer += L"_middle_right.rt";
			}
			else
			{
				buffer += L"_all.rt";
			}
			BuildRoomFromFile(buffer, &_rooms[GetIndexFromXY(x, y, 4)], x, y, false, _usingSprites);
			buffer.clear();
		}
	}
	buffer.clear();

	BuildEntrance();

	return S_OK;
}

void Stage::Release()
{
}

void Stage::ClearAllTheBits(int xStartIndex, int yStartIndex, int width, int height)
{
	for (int y = yStartIndex; y < yStartIndex + height; ++y)
	{
		for (int x = xStartIndex; x < xStartIndex + width; ++x)
		{
			int index = GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX);
			if (tileLayer0[index])
			{
				tileLayer0[index]->maskInfo = 0;
				for (int i = 0; i < 4; ++i)
				{
					tileLayer0[index]->spriteMaskInfo[i].hasMask= 0;
					tileLayer0[index]->spriteMaskInfo[i].maskSprite= nullptr;
				}
			}
		}
	}
	for (int y = yStartIndex; y < yStartIndex + height; ++y)
	{
		for (int x = xStartIndex; x < xStartIndex + width; ++x)
		{
			int index = GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX);
			if (tileLayer1[index])
			{
				tileLayer1[index]->maskInfo = 0;
				for (int i = 0; i < 4; ++i)
				{
					tileLayer1[index]->spriteMaskInfo[i].hasMask= 0;
					tileLayer1[index]->spriteMaskInfo[i].maskSprite= nullptr;
				}
			}
		}
	}
	for (int y = yStartIndex; y < yStartIndex + height; ++y)
	{
		for (int x = xStartIndex; x < xStartIndex + width; ++x)
		{
			int index = GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX);
			if (tileLayer2[index])
			{
				tileLayer2[index]->maskInfo = 0;
				for (int i = 0; i < 4; ++i)
				{
					tileLayer2[index]->spriteMaskInfo[i].hasMask= 0;
					tileLayer2[index]->spriteMaskInfo[i].maskSprite= nullptr;
				}
			}
		}
	}
}

void Stage::CalculateAllMask(int xStartIndex, int yStartIndex, int width, int height)
{
	ClearAllTheBits(xStartIndex, yStartIndex, width, height);
	CalculateMask(xStartIndex, yStartIndex, width, height, 1);
	CalculateMask(xStartIndex, yStartIndex, width, height, 0);
}

void Stage::CalculateMask(int xStartIndex, int yStartIndex, int width, int height, int layer)
{
	Tile **tileLayer = nullptr;
	if (layer == 0)
	{
		tileLayer = tileLayer0;
	}
	else if (layer == 1)
	{
		tileLayer = tileLayer1;
	}
	for (int y = yStartIndex; y < yStartIndex + height; ++y)
	{
		for (int x = xStartIndex; x < xStartIndex + width; ++x)
		{
			int index = x + STAGE_TOTAL_COUNTX * y;
			if (tileLayer[index] &&
				tileLayer[index]->thisMaskInfo)
			{
				D2DSprite *thisTileSprite = tileLayer[index]->sprite;
				//위에 타일이 있는지 검사
				int upperY = y - 1;
				if (upperY >= 0)
				{
					//위에 타일이 없고 현재 타일이 위쪽에 마스크를 그리는 걸 허용하면..
					if ((tileLayer[x + STAGE_TOTAL_COUNTX * upperY] == nullptr))
					{
						if ((tileLayer[index]->thisMaskInfo >> 0) & 1)
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y], tileLayer2[x + STAGE_TOTAL_COUNTX * y], 0);
						}
					}
					//위에 타일이 있다. 
					else
					{
						if (((tileLayer[x + STAGE_TOTAL_COUNTX * upperY]->nearMaskInfo >> 3) & 1) &&
							(tileLayer[index]->thisMaskInfo >> 0) & 1)
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y], tileLayer2[x + STAGE_TOTAL_COUNTX * y], 0);
						}
					}
				}
				//왼쪽 타일이 있는지 검사
				int leftX = x - 1;
				if (leftX >= 0)
				{
					//왼쪽 타일이 없고 현재 타일이 왼쪽에 마스크를 그리는 걸 허용하면..
					if ((tileLayer[leftX + STAGE_TOTAL_COUNTX * y] == nullptr))
					{
						if ((tileLayer[index]->thisMaskInfo >> 1) & 1)
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y], tileLayer2[x + STAGE_TOTAL_COUNTX * y], 1);
						}
					}
					//왼쪽 타일이 있다
					else
					{
						if (((tileLayer[leftX + STAGE_TOTAL_COUNTX * y]->nearMaskInfo >> 2) & 1) &&
							((tileLayer[index]->thisMaskInfo >> 1) & 1))
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y], tileLayer2[x + STAGE_TOTAL_COUNTX * y], 1);
						}
					}
				}
				//오른 타일이 있는지 검사
				int rightX = x + 1;
				if (rightX < STAGE_TOTAL_COUNTX)
				{
					//오른쪽 타일이 없고 현재 타일이 오른쪽에 마스크를 그리는 걸 허용하면..
					if ((tileLayer[rightX + STAGE_TOTAL_COUNTX * y] == nullptr) )
					{
						if (((tileLayer[index]->thisMaskInfo >> 2) & 1))
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y], tileLayer2[x + STAGE_TOTAL_COUNTX * y], 2);
						}
					}
					//오른쪽 타일이 있다
					else
					{
						if (((tileLayer[rightX + STAGE_TOTAL_COUNTX * y]->nearMaskInfo >> 1) & 1) &&
							((tileLayer[index]->thisMaskInfo >> 2) & 1))
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y], tileLayer2[x + STAGE_TOTAL_COUNTX * y], 2);
						}
					}
				}
				//아래 타일이 있는지 검사
				int lowerY = y + 1;
				if (lowerY < STAGE_TOTAL_COUNTY)
				{
					//아래쪽 타일이 없고 현재 타일이 아래쪽에 그리는 걸 허용하면..
					if ((tileLayer[x + STAGE_TOTAL_COUNTX * lowerY] == nullptr))
					{
						if (((tileLayer[index]->thisMaskInfo >> 3) & 1))
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y], tileLayer2[x + STAGE_TOTAL_COUNTX * y], 3);
						}
					}
					//아래쪽 타일이 있다
					else
					{
						if (((tileLayer[x + STAGE_TOTAL_COUNTX * lowerY]->nearMaskInfo >> 0) & 1) &&
							(tileLayer[index]->thisMaskInfo >> 3) & 1)
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y], tileLayer2[x + STAGE_TOTAL_COUNTX * y], 3);
						}
					}
				}
			}
		}
	}
}

void Stage::Update(float deltaTime)
{
	for (auto &gem : _gems)
	{
		if (gem)
		{
			gem->Update(deltaTime);
		}
	}
	//Console::Log("x : %d, y : %d\n", _throws.front()->position.tileX, _throws.front()->position.tileY);
	for (auto &throws : _throws)
	{
		if (throws)
		{
			throws->Update(deltaTime);
		}
	}
}

void Stage::Render(const TilePosition &camPos)
{
	int minX = camPos.tileX;
	//여기서 마스크 레이어의 재대로 된 렌더를 위하여 1을 더 해줌
	int maxX = camPos.tileX + 21;
	int minY = camPos.tileY;
	int maxY = camPos.tileY + 12;
	if (minX < 0) { minX = 0; }
	if (minY < 0) { minY = 0; }
	if (maxX > STAGE_TOTAL_COUNTX - 1) { maxX = STAGE_TOTAL_COUNTX - 1; }
	if (maxY > STAGE_TOTAL_COUNTY - 1) { maxY = STAGE_TOTAL_COUNTY - 1; }

	Vector2 untileldCamPos = camPos.UnTilelize();

	int backMinX = minX / 4;
	int backMaxX = maxX / 4;
	int backMinY = minY / 4;
	int backMaxY = maxY / 4;

	for (int y = backMinY; y <= backMaxY; ++y)
	{
		for (int x = backMinX; x <= backMaxX; ++x)
		{
			_backgroundSprite->Render(gRenderTarget, x * BACKGROUND_SIZE - untileldCamPos.x, 
				y * BACKGROUND_SIZE - untileldCamPos.y);
		}
	}

	_tunnels[0]->Render(gRenderTarget, untileldCamPos);
	_tunnels[1]->Render(gRenderTarget, untileldCamPos);

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			if (tileLayer0[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)])
			{
				tileLayer0[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)]->Render(gRenderTarget, untileldCamPos);
			}
			if (tileLayer2[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)])
			{
				tileLayer2[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)]->Render(gRenderTarget, untileldCamPos);
			}
		}
	}

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			if (tileLayer1[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)])
			{
				tileLayer1[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)]->Render(gRenderTarget, untileldCamPos);
			}
		}
	}

	for (auto &throws : _throws)
	{
		throws->Render(gRenderTarget, untileldCamPos);
	}

	for (auto &gem : _gems)
	{
		gem->Render(gRenderTarget, untileldCamPos);
	}
}

void Stage::DestroyTile(const IntVector2 & tilePos)
{
	bool destroyed = false;

	Tile *pLayer0Tile = tileLayer0[GetIndexFromXY(tilePos.x, tilePos.y, STAGE_TOTAL_COUNTX)];
	Tile *pLayer1Tile = tileLayer1[GetIndexFromXY(tilePos.x, tilePos.y, STAGE_TOTAL_COUNTX)];
	Tile *pLayer2Tile = tileLayer2[GetIndexFromXY(tilePos.x, tilePos.y, STAGE_TOTAL_COUNTX)];


	if (pLayer0Tile != nullptr &&
		pLayer0Tile->canBeDestroyedByBomb)
	{
		pLayer0Tile->canBeDestroyedByBomb = false;
		pLayer0Tile->sourceIndex = pLayer0Tile->destroyedIndex;
		pLayer0Tile->collisionType = TileCollisionType::TILE_COLLISION_NONE;
		pLayer0Tile->thisMaskInfo = 0;
		pLayer0Tile->nearMaskInfo = 15;
		destroyed = true;

		pLayer2Tile = pLayer0Tile;
		pLayer0Tile = nullptr;
	}
	else if(pLayer1Tile != nullptr &&
		pLayer1Tile->canBeDestroyedByBomb)
	{
		pLayer1Tile->canBeDestroyedByBomb = false;
		pLayer1Tile->sourceIndex = pLayer1Tile->destroyedIndex;
		pLayer1Tile->collisionType = TileCollisionType::TILE_COLLISION_NONE;
		pLayer1Tile->thisMaskInfo = 0;
		pLayer1Tile->nearMaskInfo = 15;
		destroyed = true;

		pLayer2Tile = pLayer1Tile;
		pLayer1Tile = nullptr;
	}

	if (pLayer2Tile != nullptr &&
		pLayer2Tile->canBeDestroyedByBomb)
	{
		pLayer2Tile->canBeDestroyedByBomb = false;
		pLayer2Tile->sourceIndex = pLayer2Tile->destroyedIndex;
		pLayer2Tile->collisionType = TileCollisionType::TILE_COLLISION_NONE;
		pLayer2Tile->thisMaskInfo = 0;
		pLayer2Tile->nearMaskInfo = 15;
		destroyed = true;
	}
	if (destroyed)
	{
		for (auto &gem : _gems)
		{
			if (gem->position.tileX == tilePos.x && gem->position.tileY == tilePos.y)
			{
				gem->Digged();
			}
		}

		ClearAllTheBits(tilePos.x - 1, tilePos.y - 1, 3, 3);
		CalculateMask(tilePos.x - 1, tilePos.y - 1, 3, 3, 0);
		CalculateMask(tilePos.x - 1, tilePos.y - 1, 3, 3, 1);
	}
}

void Stage::RegisterDelegates()
{
	EVENTMANAGER->RegisterDelegate(EVENT_COLLECT_MONEY, EventDelegate::FromFunction<Stage, &Stage::HandleCollectMoneyEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_ITEM_BREAK, EventDelegate::FromFunction < Stage, &Stage::HandleItemBreakEvent>(this));
}

void Stage::UnRegisterDelegates()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_COLLECT_MONEY, EventDelegate::FromFunction<Stage, &Stage::HandleCollectMoneyEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_ITEM_BREAK, EventDelegate::FromFunction < Stage, &Stage::HandleItemBreakEvent>(this));
}

void Stage::BuildBorder()
{
	std::wstring path = LdataPath;
	Room borderRooms[16];
	BuildRoomFromFile( L"border_top_left.rt", &borderRooms[0], 0, 0, true, _usingSprites);
	BuildRoomFromFile(L"border_top_right.rt", &borderRooms[1], 4, 0, true, _usingSprites);
	BuildRoomFromFile(L"border_bottom_right.rt", &borderRooms[2], 4, 4, true, _usingSprites);
	BuildRoomFromFile(L"border_bottom_left.rt", &borderRooms[3], 0, 4, true, _usingSprites);
	for (int i = 0; i < 3; ++i)
	{
		BuildRoomFromFile(L"border_top.rt", &borderRooms[4 + i], 1 + i, 0, true,_usingSprites);
	}
	for (int i = 0; i < 3; ++i)
	{
		BuildRoomFromFile(L"border_bottom.rt", &borderRooms[7 + i], 1 + i, 4, true, _usingSprites);
	}
	for (int i = 0; i < 3; ++i)
	{
		BuildRoomFromFile(L"border_left.rt", &borderRooms[10 + i], 0, 1 + i, true, _usingSprites);
	}
	for (int i = 0; i < 3; ++i)
	{
		BuildRoomFromFile(L"border_right.rt", &borderRooms[13 + i], 4, 1 + i, true, _usingSprites);
	}
}

bool Stage::GetRandomFileNameFromRoomType(RoomType types, std::wstring &str)
{
	//switch (types)
	//{
	//case RoomType::ROOM_BLOCK:
	//{
	//	str += L"_block_00.rt";
	//}break;
	//case RoomType::ROOM_AISLE :
	//{
	//	str += L"_aisle_00.rt";
	//}break;
	//case RoomType::ROOM_TOP_OPEN :
	//{
	//	str += L"_topopen_00.rt";
	//}break;
	//case RoomType::ROOM_BOTTOM_OPEN :
	//{
	//	str += L"_bottomopen_00.rt";
	//}break;
	//}

	str = L"00.rt";
	return true;
}

void Stage::BuildRoomFromFile(const std::wstring &fileName, 
	Room *room, int roomX, int roomY, bool border, std::map<std::wstring, D2DSprite *> &usingSprites)
{

	FileUtils::File loadFile;
	std::wstring filePath;
	filePath += LdataPath;
	filePath += fileName;

	if (loadFile.Open(filePath, FileUtils::FileAccess::Read))
	{
		loadFile.GetLine();
		loadFile.GetLine();

		BuildTileLayerFromFile(loadFile, room->layer0, roomX, roomY, border, 0, usingSprites);
		BuildTileLayerFromFile(loadFile, room->layer1, roomX, roomY, border, 1, usingSprites);

		CollectRoomPropertyFromFile(loadFile, room);

		loadFile.Close();
	}
}

void Stage::BuildTileLayerFromFile(FileUtils::File & file, 
	Tile **pTileLayer, int roomX, int roomY, bool border, int layerNum, std::map<std::wstring, D2DSprite *> &usingSprites)
{
	WCHAR buffer[40]{};

	TileProperty *property = new TileProperty;

	for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	{
		for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
		{
			int index = x + ROOM_TILE_COUNTX * y;
			file.GetLine();

			file.Read(L"Image Key : %s\n", &buffer);

			if (wcscmp(buffer, L"e") != 0)
			{
				CheckUsingSpriteExistence(buffer);
				property->sprite = usingSprites.find(buffer)->second;

				property->position.x = x;
				property->position.y = y;

				file.Read(L"SourceIndex X : %d, Y : %d\n", &property->sourceIndex.x, &property->sourceIndex.y);
				file.Read(L"Destroyed Index X : %d, Y : %d\n",
					&property->destroyedIndex.x, &property->destroyedIndex.y);
				file.Read(L"This Mask Info : %u\n", &property->thisMaskInfo);
				file.Read(L"Near Mask Info : %u\n", &property->nearMaskInfo);
				file.Read(L"Collision Type : %d\n", &property->collisionType);
				file.Read(L"Mask Info : %u\n", &property->maskInfo);
				file.Read(L"Layer : %d\n", &property->layer);
				file.Read(L"Can Be Destroyed : %d\n", &property->canBeDestroyedByBomb);

				pTileLayer[index] = (Tile *)OBJECTMANAGER->CreateObject(L"tile", property);
				IntVector2 worldPositon{};
				if (border)
				{
					worldPositon = CalculateBorderWorldIndex(roomX, roomY, x, y);
					pTileLayer[index]->position.tileX = worldPositon.x;
					pTileLayer[index]->position.tileY = worldPositon.y;
					if (layerNum == 0)
					{
						tileLayer0[GetIndexFromXY(worldPositon.x, worldPositon.y, STAGE_TOTAL_COUNTX)] = pTileLayer[index];
					}
					else if (layerNum == 1)
					{
						tileLayer2[GetIndexFromXY(worldPositon.x, worldPositon.y, STAGE_TOTAL_COUNTX)] = pTileLayer[index];
					}
				}
				else
				{
					worldPositon = CalculateTileWorldIndex(roomX, roomY, x, y);
					pTileLayer[index]->position.tileX = worldPositon.x;
					pTileLayer[index]->position.tileY = worldPositon.y;
					if (layerNum == 0)
					{
						tileLayer1[GetIndexFromXY(worldPositon.x, worldPositon.y, STAGE_TOTAL_COUNTX)] = pTileLayer[index];
					}
					else if (layerNum == 1)
					{
						tileLayer2[GetIndexFromXY(worldPositon.x, worldPositon.y, STAGE_TOTAL_COUNTX)] = pTileLayer[index];
					}
				}
			}
			else
			{
				for (int i = 0; i < 8; ++i)
				{
					file.GetLine();
				}
				ZeroMemory(buffer, 40);
			}
		}
	}
}

void Stage::CheckUsingSpriteExistence(const std::wstring & key)
{
	auto &found = _usingSprites.find(key);
	if (found == _usingSprites.end())
	{
		D2DSprite *insertImage = new D2DFrameSprite;
		insertImage->Init(IMAGEMANAGER->GetImage(key), 64, 64, IntVector2());
		_usingSprites.insert(std::make_pair(key, insertImage));
	}
}

void Stage::CopyTilesPosition(int worldIndex, int x, int y)
{
	if (tileLayer0[worldIndex] != nullptr)
	{
		tileLayer0[worldIndex]->position.tileX = x;
		tileLayer0[worldIndex]->position.tileY = y;
	}
	if (tileLayer2[worldIndex] != nullptr)
	{
		tileLayer2[worldIndex]->position.tileX = x;
		tileLayer2[worldIndex]->position.tileY = y;
	}
}

void Stage::CopyTilesFromRooms(Room * rooms)
{
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			int index = GetIndexFromXY(x, y, 4);

			Room &currentRoom = rooms[index];

			int xTileStartIndex = x * ROOM_TILE_COUNTX + 3;
			int yTileStartIndex = y * ROOM_TILE_COUNTY + 3;

			for (int j = yTileStartIndex; j < yTileStartIndex + ROOM_TILE_COUNTY; ++j)
			{
				for (int i = xTileStartIndex; i < xTileStartIndex + ROOM_TILE_COUNTX; ++i)
				{
					tileLayer1[i + STAGE_TOTAL_COUNTX * j] =
						currentRoom.layer0[(i - xTileStartIndex) + ROOM_TILE_COUNTX * (j - yTileStartIndex)];

					tileLayer2[i + STAGE_TOTAL_COUNTX * j] =
						currentRoom.layer1[(i - xTileStartIndex) + ROOM_TILE_COUNTX * (j - yTileStartIndex)];

					if (tileLayer1[i + STAGE_TOTAL_COUNTX * j] != nullptr)
					{
						tileLayer1[i + STAGE_TOTAL_COUNTX * j]->position.tileX = i;
						tileLayer1[i + STAGE_TOTAL_COUNTX * j]->position.tileY = j;
					}
				}
			}
		}
	}
}

void Stage::TileInfoBitmaskCopy(D2DSprite * sourSprite, Tile * sourTile, Tile * maskTile, uint32 offset)
{
	sourTile->maskInfo |= (1 << offset);
	sourTile->spriteMaskInfo[offset].hasMask = true;
	sourTile->spriteMaskInfo[offset].maskSprite = sourSprite;
}

void Stage::BuildEntrance()
{
	int startXIndex = _stageBuildInfo.startRoomIndex.x;
	int startYIndex = _stageBuildInfo.startRoomIndex.y;
	
	int endXIndex = _stageBuildInfo.exitRoomIndex.x;
	int endYIndex = _stageBuildInfo.exitRoomIndex.y;

	TunnelProperty *startProperty = (TunnelProperty *)_rooms[GetIndexFromXY(startXIndex, startYIndex, 4)].properties.find(L"tunnel")->second.front();
	TunnelProperty *endProperty = (TunnelProperty *)_rooms[GetIndexFromXY(endXIndex, endYIndex, 4)].properties.find(L"tunnel")->second.front();
	startProperty->sourceIndex.x += 1;

	IntVector2 startLocalIndex = startProperty->position;
	IntVector2 endLocalIndex = endProperty->position;

	startProperty->position = CalculateTileWorldIndex(startXIndex, startYIndex, startLocalIndex.x, startLocalIndex.y);
	endProperty->position = CalculateTileWorldIndex(endXIndex, endYIndex, endLocalIndex.x, endLocalIndex.y);

	_tunnels[0] = (Tunnel *)OBJECTMANAGER->CreateObject(L"tunnel", startProperty);
	_tunnels[1] = (Tunnel *)OBJECTMANAGER->CreateObject(L"tunnel", endProperty);

	_tunnels[1]->SetExit(true);
}

void Stage::BuildGems()
{
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			for (auto &prop : _rooms[GetIndexFromXY(x, y, 4)].properties.find(L"gem")->second)
			{
				prop->position = CalculateTileWorldIndex(x, y, prop->position.x, prop->position.y);
				Gem *newGem = (Gem *)OBJECTMANAGER->CreateObject(L"gem", prop);

				if (tileLayer0[GetIndexFromXY(newGem->position.tileX, newGem->position.tileY, STAGE_TOTAL_COUNTX)] != nullptr)
				{
					newGem->SetIsInTile(true);
				}
				else
				{
					if (tileLayer1[GetIndexFromXY(newGem->position.tileX, newGem->position.tileY, STAGE_TOTAL_COUNTX)] != nullptr)
					{
						newGem->SetIsInTile(true);
					}
				}

				if(newGem->position.tileX)
				_gems.push_back(newGem);
			}
		}
	}
}

void Stage::BuildThrows()
{
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			for (auto &prop : _rooms[GetIndexFromXY(x, y, 4)].properties.find(L"throws")->second)
			{
				ThrowProperty *convertedProperty = (ThrowProperty *)(prop);
				if (convertedProperty->sourceIndex.x == 2)
				{
					IntVector2 worldPosition = CalculateTileWorldIndex(x, y, prop->position.x, prop->position.y);
					Throws *newThrows = (Throws *)OBJECTMANAGER->CreateObject(L"throws", prop);

					newThrows->position.tileX = worldPosition.x;
					newThrows->position.tileY = worldPosition.y;
					newThrows->desiredPosition = newThrows->position;

					convertedProperty->sourceIndex.x += 1;
					Throws *newThrowsSecond = (Throws *)OBJECTMANAGER->CreateObject(L"throws", prop);

					newThrowsSecond->position.tileX = worldPosition.x;
					newThrowsSecond->position.tileY = worldPosition.y;
					newThrowsSecond->desiredPosition = newThrows->position;

					_throws.push_back(newThrows);
					_throws.push_back(newThrowsSecond);
				}
				else
				{
					IntVector2 worldPosition = CalculateTileWorldIndex(x, y, prop->position.x, prop->position.y);

					Throws *newThrows = (Throws *)OBJECTMANAGER->CreateObject(L"throws", prop);

					newThrows->position.tileX = worldPosition.x;
					newThrows->position.tileY = worldPosition.y;
					newThrows->desiredPosition = newThrows->position;

					if (newThrows->position.tileX)
						_throws.push_back(newThrows);
				}
			}
		}
	}
}

void Stage::CollectRoomPropertyFromFile(FileUtils::File & file, Room * room)
{
	std::wstring str1 = file.GetLine();
	std::wstring propertyName{};

	for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	{
		for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
		{
			int index = x + ROOM_TILE_COUNTX * y;
			std::wstring str2 = file.GetLine();

			WCHAR buffer[40]{};
			file.Read(L"Property Name : %s\n", &buffer);
			if (wcscmp(buffer, L"none") != 0 && wcslen(buffer) != 0)
			{
				MapTool::PropertyInfo propertyInfo{};
				wcscpy(propertyInfo.name, buffer);

				file.Read(L"SourceIndex X : %d, Y : %d\n",
					&propertyInfo.sourceIndex.x, &propertyInfo.sourceIndex.y);
				file.Read(L"Value0 : %s\n", &propertyInfo.value0);
				if (wcscmp(propertyInfo.value0, L"none") == 0)
				{
					_stprintf(propertyInfo.value0, L"");
				}
				file.Read(L"Value1 : %s\n", &propertyInfo.value1);
				if (wcscmp(propertyInfo.value1, L"none") == 0)
				{
					_stprintf(propertyInfo.value1, L"");
				}
				file.GetLine();

				propertyName = buffer;
				BaseProperty *property = PROPERTYFACTORY->Build(propertyName, propertyInfo);
				if (property != nullptr)
				{
					property->position = IntVector2(x, y);
				
					auto &found = room->properties.find(propertyName);
					//없으면..
					if (found == room->properties.end())
					{
						BasePropertyList propList;
						propList.push_back(property);
						room->properties.insert(std::make_pair(propertyName, propList));
					}
					//있으면
					else
					{
						found->second.push_back(property);
					}
				}
			}
			else
			{
				file.GetLine();
				file.GetLine();
				file.GetLine();
				file.GetLine();
			}
		}
	}
}

void Stage::HandleCollectMoneyEvent(const IEvent * event)
{
	CollectMoneyEvent *convertedEvent = (CollectMoneyEvent *)event;

	GameObject *object = OBJECTMANAGER->FindObjectId(convertedEvent->GetId());
	for (auto &iter = _gems.begin(); iter != _gems.end();)
	{
		if ((*iter) == object)
		{
			_gems.erase(iter);
			break;
		}
		else
		{
			iter++;
		}
	}
}

void Stage::HandleItemBreakEvent(const IEvent * event)
{
	ItemBreakEvent *convertedEvent = (ItemBreakEvent *)event;
	GameObject *object = OBJECTMANAGER->FindObjectId(convertedEvent->GetId());
	if (convertedEvent->GetBreakType() == BreakType::Jar || 
		convertedEvent->GetBreakType() == BreakType::Bone  
		)
	{
		for (auto &iter = _throws.begin(); iter != _throws.end();)
		{
			if ((*iter) == object)
			{
				_throws.erase(iter);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
}

ReturnTile Stage::GetAdjacent9(const IntVector2 & pos)
{
	ReturnTile result{};
	result.tileNum = 9;

	int index = 0;

	for (int y = 1; y >= -1; --y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			if (x == 0 && y == 0) { continue; }

			IntVector2 absTilePos(pos.x + x, pos.y + y);
			if (absTilePos.x < 0 || absTilePos.x > STAGE_TOTAL_COUNTX - 1 ||
				absTilePos.y < 0 || absTilePos.y > STAGE_TOTAL_COUNTY - 1)
			{
				result.tiles[index] = nullptr;
				index++;
				continue;
			}
			result.tiles[index] = tileLayer1[absTilePos.x + absTilePos.y * STAGE_TOTAL_COUNTX];
			if (result.tiles[index] == nullptr)
			{
				result.tiles[index] = tileLayer0[absTilePos.x + absTilePos.y * STAGE_TOTAL_COUNTX];
			}
			index++;
		}
	}

	if (tileLayer1[GetIndexFromXY(pos.x, pos.y, STAGE_TOTAL_COUNTX)] == nullptr)
	{
		result.tiles[8] = tileLayer0[GetIndexFromXY(pos.x, pos.y, STAGE_TOTAL_COUNTX)];
	}
	else
	{
		result.tiles[8] = tileLayer1[GetIndexFromXY(pos.x, pos.y, STAGE_TOTAL_COUNTX)];
	}

	Tile *temp;
	temp = result.tiles[0];
	result.tiles[0] = result.tiles[1];
	result.tiles[1] = temp;

	temp = result.tiles[1];
	result.tiles[1] = result.tiles[6];
	result.tiles[6] = temp;

	temp = result.tiles[2];
	result.tiles[2] = result.tiles[3];
	result.tiles[3] = temp;

	temp = result.tiles[3];
	result.tiles[3] = result.tiles[4];
	result.tiles[4] = temp;

	return result;}



