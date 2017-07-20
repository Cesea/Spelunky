#include "stdafx.h"
#include "Room.h"

#include "StageManager.h"

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
	int index = GetIndexFromXY(indexX, indexY, 4);

	int xTileStartIndex = indexX * ROOM_TILE_COUNTX + 3;
	int yTileStartIndex = indexY * ROOM_TILE_COUNTY + 3;

	result.x = xTileStartIndex + localX;
	result.y = yTileStartIndex + localY;
	return result;
}


Stage::Stage()
{
}

Stage::~Stage()
{
}

HRESULT Stage::Init()
{
	return S_OK;
}

HRESULT Stage::InitFromRoomTypes(const RandomRoomGenerated &randomTypes)
{
	BuildBorder();

	std::wstring buffer = L"";
	Room rooms[16];
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			if (GetRandomFileNameFromRoomType(randomTypes.roomTypes[GetIndexFromXY(x, y, 4)], buffer))
			{
				BuildRoomFromFile(buffer, &rooms[GetIndexFromXY(x, y, 4)],x, y, false, _usingSprites);
			}
			else
			{
				Console::Log("Room %d, %d build failed\n", x, y);
			}
		}
	}
	buffer.clear();
	return S_OK;
}

void Stage::Release()
{
}


void Stage::ClearAllTheBits(int xStartIndex, int yStartIndex, int width, int height)
{
	for (int i = 0; i < STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY; ++i)
	{
		if (tileLayer0[i])
		{
			tileLayer0[i]->maskInfo = 0;
			for (int j = 0; j < 4; ++j)
			{
				tileLayer0[i]->spriteMaskInfo[j].hasMask = 0;
				tileLayer0[i]->spriteMaskInfo[j].maskSprite = nullptr;
			}
		}
	}

	for (int i = 0; i < STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY; ++i)
	{
		if (tileLayer1[i])
		{
			tileLayer1[i]->maskInfo = 0;
			for (int j = 0; j < 4; ++j)
			{
				tileLayer1[i]->spriteMaskInfo[j].hasMask = 0;
				tileLayer1[i]->spriteMaskInfo[j].maskSprite = nullptr;
			}
		}
	}

	for (int i = 0; i < STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY; ++i)
	{
		if (tileLayer2[i])
		{
			tileLayer2[i]->maskInfo = 0;
			for (int j = 0; j < 4; ++j)
			{
				tileLayer2[i]->spriteMaskInfo[j].hasMask = 0;
				tileLayer2[i]->spriteMaskInfo[j].maskSprite = nullptr;
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

	for (int y = minY; y < maxY; ++y)
	{
		for (int x = minX; x < maxX; ++x)
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

	for (int y = minY; y < maxY; ++y)
	{
		for (int x = minX; x < maxX; ++x)
		{
			if (tileLayer1[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)])
			{
				tileLayer1[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)]->Render(gRenderTarget, untileldCamPos);
			}
		}
	}
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
void Stage::CopyBorderTiles(Room * rooms)
{

#pragma region Corner borders
	for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	{
		for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
		{
			int worldIndex = x + STAGE_TOTAL_COUNTX * y;
			int localIndex = x + ROOM_TILE_COUNTX * y;
			tileLayer0[worldIndex] = rooms[0].layer0[localIndex];
			tileLayer2[worldIndex] = rooms[0].layer1[localIndex];
			CopyTilesPosition(worldIndex, x, y);
		}
	}

	for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	{
		for (int x = STAGE_TOTAL_COUNTX - ROOM_TILE_COUNTX; x < STAGE_TOTAL_COUNTX; ++x)
		{
			int worldIndex = x + STAGE_TOTAL_COUNTX * y;

			int localX = x - (STAGE_TOTAL_COUNTX - ROOM_TILE_COUNTX);
			int localY = y;

			tileLayer0[worldIndex] = rooms[1].layer0[localX + localY * ROOM_TILE_COUNTX];
			tileLayer2[worldIndex] = rooms[1].layer1[localX + localY * ROOM_TILE_COUNTX];
			CopyTilesPosition(worldIndex, x, y);

		}
	}
	for (int y = STAGE_TOTAL_COUNTY - ROOM_TILE_COUNTY; y < STAGE_TOTAL_COUNTY; ++y)
	{
		for (int x = STAGE_TOTAL_COUNTX - ROOM_TILE_COUNTX; x < STAGE_TOTAL_COUNTX; ++x)
		{
			int worldIndex = x + STAGE_TOTAL_COUNTX * y;

			int localX = x - (STAGE_TOTAL_COUNTX - ROOM_TILE_COUNTX);
			int localY = y - (STAGE_TOTAL_COUNTY - ROOM_TILE_COUNTY);;

			tileLayer0[worldIndex] = rooms[2].layer0[localX + ROOM_TILE_COUNTX * localY];
			tileLayer2[worldIndex] = rooms[2].layer1[localX + ROOM_TILE_COUNTX * localY];
			CopyTilesPosition(worldIndex, x, y);

		}
	}

	for (int y = STAGE_TOTAL_COUNTY - ROOM_TILE_COUNTY; y < STAGE_TOTAL_COUNTY; ++y)
	{
		for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
		{
			int worldIndex = x + STAGE_TOTAL_COUNTX * y;

			int localX = x;
			int localY = y - (STAGE_TOTAL_COUNTY - ROOM_TILE_COUNTY);;

			tileLayer0[worldIndex] = rooms[3].layer0[localX + ROOM_TILE_COUNTX * localY];
			tileLayer2[worldIndex] = rooms[3].layer1[localX + ROOM_TILE_COUNTX * localY];
			
			CopyTilesPosition(worldIndex, x, y);
		}
	}

#pragma endregion

#pragma region Side Borders
	//여기서 상, 하 보더타일 카피
	for (int i = 0; i < 3; ++i)
	{
		//위 보더
		for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
		{
			for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
			{
				int worldX = 8 + (i)* ROOM_TILE_COUNTX + x;
				int worldY = y;

				int worldIndex = worldX + STAGE_TOTAL_COUNTX * worldY;

				int localIndex = x + ROOM_TILE_COUNTX * y;

				tileLayer0[worldIndex] = rooms[4 + i].layer0[localIndex];
				tileLayer2[worldIndex] = rooms[4 + i].layer1[localIndex];
				CopyTilesPosition(worldIndex, worldX, worldY);
			}
		}
		//아래 보더
		for (int y = STAGE_TOTAL_COUNTY - ROOM_TILE_COUNTY; y < STAGE_TOTAL_COUNTY; ++y)
		{
			for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
			{
				int worldX = 8 + (i)* ROOM_TILE_COUNTX + x;
				int worldY = y;

				int worldIndex = worldX + STAGE_TOTAL_COUNTX * worldY;

				int localIndex = x + ROOM_TILE_COUNTX * (y - (STAGE_TOTAL_COUNTY - ROOM_TILE_COUNTY));
				tileLayer0[worldIndex] = rooms[7 + i].layer0[localIndex];
				tileLayer2[worldIndex] = rooms[7 + i].layer1[localIndex];
				CopyTilesPosition(worldIndex, worldX, worldY);
			}
		}
	}

	//여기서 좌, 우 보더타일 카피
	for (int i = 0; i < 3; ++i)
	{
		//좌 보더
		for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
		{
			for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
			{
				int worldX = x;
				int worldY = 7 + (i)* ROOM_TILE_COUNTY + y;

				int worldIndex = worldX + STAGE_TOTAL_COUNTX * worldY;

				int localIndex = x + ROOM_TILE_COUNTX * y;
				tileLayer0[worldIndex] = rooms[10 + i].layer0[localIndex];
				tileLayer2[worldIndex] = rooms[10 + i].layer1[localIndex];
				CopyTilesPosition(worldIndex, worldX, worldY);

			}
		}

		//우 보더
		for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
		{
			for (int x = STAGE_TOTAL_COUNTX - ROOM_TILE_COUNTX; x < STAGE_TOTAL_COUNTX; ++x)
			{
				int worldX = x;
				int worldY = 7 + (i)* ROOM_TILE_COUNTY + y;

				int worldIndex = worldX + STAGE_TOTAL_COUNTX * worldY;

				int localIndex = (x - (STAGE_TOTAL_COUNTX - ROOM_TILE_COUNTX)) + ROOM_TILE_COUNTX * y;

				tileLayer0[worldIndex] = rooms[13 + i].layer0[localIndex];
				tileLayer2[worldIndex] = rooms[13 + i].layer1[localIndex];
				CopyTilesPosition(worldIndex, worldX, worldY);
			}
		}
#pragma endregion
	}
}


bool Stage::GetRandomFileNameFromRoomType(RoomType types, std::wstring &str)
{
	switch (types)
	{
	case RoomType::ROOM_BLOCK:
	{
		str = L"block_00.rt";
	}break;
	case RoomType::ROOM_AISLE :
	{
		str = L"aisle_00.rt";
	}break;
	case RoomType::ROOM_TOP_OPEN :
	{
		str = L"topopen_00.rt";
	}break;
	case RoomType::ROOM_BOTTOM_OPEN :
	{
		str = L"bottomopen_00.rt";
	}break;
	}
	 //str = L"00.rt";
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
				for (int i = 0; i < 7; ++i)
				{
					file.GetLine();
				}
				//EVENTMANAGER->QueueEvent(new CreateObjectEvent(L"tile", property));
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
	//maskTile->maskInfo |= (1 << offset);

	//maskTile->sprite = sourSprite;

	sourTile->spriteMaskInfo[offset].hasMask = true;
	sourTile->spriteMaskInfo[offset].maskSprite = sourSprite;

	//maskTile->spriteMaskInfo[offset].hasMask = true;
	//maskTile->spriteMaskInfo[offset].maskSprite = sourSprite;
}


