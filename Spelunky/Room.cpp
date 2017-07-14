#include "stdafx.h"
#include "Room.h"

using namespace PlayScene;

PlayScene::Stage::Stage()
{
}

PlayScene::Stage::~Stage()
{
}

HRESULT PlayScene::Stage::Init()
{
	return S_OK;
}

HRESULT PlayScene::Stage::InitFromRoomTypes(RoomType *randomRoomType)
{
	WCHAR buffer[50]{};
	Room rooms[16];
	for (int i = 0; i < 16; ++i)
	{
		if (GetRandomFileNameFromRoomType(randomRoomType[i], buffer))
		{
			BuildRoomFromFile(buffer, &rooms[i], _usingSprites);
		}
		else
		{
			Console::Log("Room %d build failed\n", i);
		}
		ZeroMemory(buffer, 50);
	}
	CopyTilesFromRooms(rooms);
	return S_OK;
}

void PlayScene::Stage::Release()
{
}

void PlayScene::Stage::RenderTileLayer(const TilePosition &camPos)
{
	//나중에 카메라 위치에 기반해서 뿌릴거임..
	int minX = camPos.tileX;
	int maxX = camPos.tileX + 20;
	int minY = camPos.tileY;
	int maxY = camPos.tileY + 12;
	if (minX < 0) { minX = 0; }
	if (minY < 0) { minY = 0; }
	if (maxX > STAGE_TILE_COUNTX - 1) { maxX = STAGE_TILE_COUNTX - 1; }
	if (maxY > STAGE_TILE_COUNTY - 1) { maxY = STAGE_TILE_COUNTY - 1; }
	//ClampInt(&minX, 0, STAGE_TILE_COUNTX);
	//ClampInt(&minY, 0, STAGE_TILE_COUNTY);

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			int index = x + y * STAGE_TILE_COUNTX;
			if (tileLayer0[index].sourceIndex.x != -1)
			{
				const auto & tile = tileLayer0[index];
				const IntVector2 &tileIndex = tile.sourceIndex;
				tile.sprite->FrameRender(gRenderTarget, (x - minX) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.x), 
					(y - minY) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.y), tileIndex.x, tileIndex.y);
			}
		}
	}
}
void PlayScene::Stage::RenderMaskLayer(const TilePosition &camPos)
{
	int minX = camPos.tileX;
	//여기서 마스크 레이어의 재대로 된 렌더를 위하여 1을 더 해줌
	int maxX = camPos.tileX + 21;
	int minY = camPos.tileY;
	int maxY = camPos.tileY + 12;
	if (minX < 0) { minX = 0; }
	if (minY < 0) { minY = 0; }
	if (maxX > STAGE_TILE_COUNTX - 1) { maxX = STAGE_TILE_COUNTX - 1; }
	if (maxY > STAGE_TILE_COUNTY - 1) { maxY = STAGE_TILE_COUNTY - 1; }

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			int index = x + y * STAGE_TILE_COUNTX;
			if ((tileLayer1[index].sourceIndex.x != -1) ||
				(tileLayer1[index].maskInfo != 0))
			{
				const auto &tile = tileLayer1[index];
				const IntVector2 &tileIndex = tile.sourceIndex;
				//tile.sprite->FrameRender(gRenderTarget, (x - minX) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.x), 
					//(y - minY) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.y), tileIndex.x, tileIndex.y);
				for (int i = 0; i < 4; ++i)
				{
					if (tileLayer1[index].spriteMaskInfo[i].hasMask)
					{
						if (i == 0)
						{
							tile.spriteMaskInfo[i].maskSprite->FrameRender(gRenderTarget, (x - minX) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.x),
								(y - minY) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.y) + TILE_SIZE_HALF, 5, 0);
						}
						else if (i == 1)
						{
							tile.spriteMaskInfo[i].maskSprite->FrameRender(gRenderTarget, (x - minX) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.x) + TILE_SIZE_HALF,
								(y - minY) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.y), 7, 2);
						}
						else if (i == 2)
						{
							tile.spriteMaskInfo[i].maskSprite->FrameRender(gRenderTarget, (x - minX) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.x) - TILE_SIZE_HALF,
								(y - minY) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.y), 7, 1);
						}
						else if (i == 3)
						{
							tile.spriteMaskInfo[i].maskSprite->FrameRender(gRenderTarget, (x - minX) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.x),
								(y - minY) * TILE_SIZE - ConvertFloatToInt(camPos.tileRel.y) - TILE_SIZE_HALF, 5, 1);
						}
					}
				}
			}
		}
	}
}

const Tile & PlayScene::Stage::GetTileConstRef(int x, int y)
{
	return tileLayer0[x + (STAGE_TILE_COUNTX* y)];
}
Tile & PlayScene::Stage::GetTileRef(int x, int y)
{
	return tileLayer0[x + (STAGE_TILE_COUNTX* y)];
}

ReturnTile PlayScene::Stage::GetAdjacent4()
{
	return ReturnTile();
}

ReturnTile PlayScene::Stage::GetAdjacent5()
{
	return ReturnTile();
}

ReturnTile PlayScene::Stage::GetAdjacentTiles8()
{
	return ReturnTile();
}

ReturnTile PlayScene::Stage::GetAdjacentTiles9()
{
	return ReturnTile();
}

void PlayScene::Stage::CalculateMask(int xStartIndex, int yStartIndex, int width, int height)
{
	ClearAllTheBits(xStartIndex, yStartIndex, width, height);
	for (int y = yStartIndex; y < yStartIndex + height; ++y)
	{
		for (int x = xStartIndex; x < xStartIndex + width; ++x)
		{
			int index = x + STAGE_TILE_COUNTX * y;
			if (tileLayer0[index].thisMaskInfo)
			{
				D2DSprite *thisTileSprite = tileLayer0[index].sprite;
				//위에 타일이 있는지 검사
				int upperY = y - 1;
				if (upperY >= 0)
				{
					//위에 타일이 없고 현재 타일이 위쪽에 마스크를 그리는 걸 허용하면..
					if ((tileLayer0[x + STAGE_TILE_COUNTX * upperY].sourceIndex.x == -1) &&
						((tileLayer0[index].thisMaskInfo >> 0) & 1))
					{
						TileInfoBitmaskCopy(thisTileSprite, tileLayer0[x + STAGE_TILE_COUNTX * upperY], tileLayer1[x + STAGE_TILE_COUNTX * upperY], 0);
					}
					//위에 타일이 있다. 
					else
					{
						if (((tileLayer0[x + STAGE_TILE_COUNTX * upperY].nearMaskInfo >> 3) & 1) &&
							(tileLayer0[index].thisMaskInfo >> 0) & 1)
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer0[x + STAGE_TILE_COUNTX * upperY], tileLayer1[x + STAGE_TILE_COUNTX * upperY], 0);
						}
					}
				}
				//왼쪽 타일이 있는지 검사
				int leftX = x - 1;
				if (leftX >= 0)
				{
					//왼쪽 타일이 없고 현재 타일이 왼쪽에 마스크를 그리는 걸 허용하면..
					if ((tileLayer0[leftX + STAGE_TILE_COUNTX * y].sourceIndex.x == -1) &&
						((tileLayer0[index].thisMaskInfo >> 1) & 1))
					{
						TileInfoBitmaskCopy(thisTileSprite, tileLayer0[leftX + STAGE_TILE_COUNTX * y], tileLayer1[leftX + STAGE_TILE_COUNTX * y], 1);
					}
					//왼쪽 타일이 있다
					else
					{
						if (((tileLayer0[leftX + STAGE_TILE_COUNTX * y].nearMaskInfo >> 2) & 1) &&
							((tileLayer0[index].thisMaskInfo >> 1) & 1))
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer0[leftX + STAGE_TILE_COUNTX * y], tileLayer1[leftX + STAGE_TILE_COUNTX * y], 1);
						}
					}
				}
				//오른 타일이 있는지 검사
				int rightX = x + 1;
				if (rightX < STAGE_TILE_COUNTX)
				{
					//오른쪽 타일이 없고 현재 타일이 오른쪽에 마스크를 그리는 걸 허용하면..
					if ((tileLayer0[rightX + STAGE_TILE_COUNTX * y].sourceIndex.x == -1) &&
						((tileLayer0[index].thisMaskInfo >> 2) & 1))
					{
						TileInfoBitmaskCopy(thisTileSprite, tileLayer0[rightX + STAGE_TILE_COUNTX * y], tileLayer1[rightX + STAGE_TILE_COUNTX * y], 2);
					}
					//오른쪽 타일이 있다
					else
					{
						if (((tileLayer0[rightX + STAGE_TILE_COUNTX * y].nearMaskInfo >> 1) & 1) &&
							((tileLayer0[index].thisMaskInfo >> 2) & 1))
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer0[rightX + STAGE_TILE_COUNTX * y], tileLayer1[rightX + STAGE_TILE_COUNTX * y], 2);
						}
					}
				}
				//아래 타일이 있는지 검사
				int lowerY = y + 1;
				if (lowerY < STAGE_TILE_COUNTY)
				{
					//아래쪽 타일이 없고 현재 타일이 아래쪽에 그리는 걸 허용하면..
					if ((tileLayer0[x + STAGE_TILE_COUNTX * lowerY].sourceIndex.x == -1) &&
						((tileLayer0[index].thisMaskInfo >> 3) & 1))
					{
						TileInfoBitmaskCopy(thisTileSprite, tileLayer0[x + STAGE_TILE_COUNTX * lowerY], tileLayer1[x + STAGE_TILE_COUNTX * lowerY], 3);
					}
					//아래쪽 타일이 있다
					else
					{
						if (((tileLayer0[x + STAGE_TILE_COUNTX * lowerY].nearMaskInfo >> 0) & 1) &&
							(tileLayer0[index].thisMaskInfo >> 3) & 1)
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer0[x + STAGE_TILE_COUNTX * lowerY], tileLayer1[x + STAGE_TILE_COUNTX * lowerY], 3);
						}
					}
				}
			}
		}
	}
}

void PlayScene::Stage::ClearAllTheBits(int xStartIndex, int yStartIndex, int width, int height)
{
	for (int i = 0; i < STAGE_TILE_COUNTX * STAGE_TILE_COUNTY; ++i)
	{
		tileLayer0[i].maskInfo = 0;
		for (int j = 0; j < 4; ++j)
		{
			tileLayer0[i].spriteMaskInfo[j].hasMask = 0;
			tileLayer0[i].spriteMaskInfo[j].maskSprite = nullptr;
		}
	}

	for (int i = 0; i < STAGE_TILE_COUNTX * STAGE_TILE_COUNTY; ++i)
	{
		tileLayer1[i].maskInfo = 0;
		for (int j = 0; j < 4; ++j)
		{
			tileLayer1[i].spriteMaskInfo[j].hasMask = 0;
			tileLayer1[i].spriteMaskInfo[j].maskSprite = nullptr;
		}
	}
}

void PlayScene::Stage::CopyTilesFromRooms(Room * rooms)
{
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			int index = x + 4 * y;
			Room &currentRoom = rooms[index];
			int xStartIndex = x * ROOM_TILE_COUNTX;
			int yStartIndex = y * ROOM_TILE_COUNTY;

			for (int j = yStartIndex; j < yStartIndex + ROOM_TILE_COUNTY; ++j)
			{
				for (int i = xStartIndex; i < xStartIndex + ROOM_TILE_COUNTX; ++i)
				{
					if ((i + STAGE_TILE_COUNTX * j) == 87)
					{
						int a = 0;
					}
					tileLayer0[i + STAGE_TILE_COUNTX * j] = currentRoom.layer0[(i - xStartIndex) + ROOM_TILE_COUNTX * (j - yStartIndex)];
					tileLayer1[i + STAGE_TILE_COUNTX * j] = currentRoom.layer1[(i - xStartIndex) + ROOM_TILE_COUNTX * (j - yStartIndex)];
				}
			}
		}
	}
}

bool PlayScene::Stage::GetRandomFileNameFromRoomType(RoomType types, WCHAR * buffer)
{
	wcscpy(buffer, L"00.rt");
	//buffer = L"resources\\data\\00.rt";
	return true;
}

void PlayScene::Stage::CheckUsingSpriteExistence(const std::wstring & key)
{
	auto &found = _usingSprites.find(key);
	if (found == _usingSprites.end())
	{
		D2DSprite *insertImage = new D2DFrameSprite;
		insertImage->Init(IMAGEMANAGER->GetImage(key), 64, 64, IntVector2());
		_usingSprites.insert(std::make_pair(key, insertImage));
	}
}

void PlayScene::Stage::BuildRoomFromFile(const WCHAR *fileName, Room *room, std::map<std::wstring, D2DSprite *> &usingSprites)
{
	FileUtils::File loadFile;

	char filePath[50];
	strcpy(filePath, dataPath);

	char tempBuffer[50];
	wcstombs(tempBuffer, fileName, 29);

	strcat(filePath, tempBuffer);
	if (loadFile.Open(filePath, FileUtils::FileAccess::Read))
	{
		loadFile.GetLine();
		loadFile.GetLine();

		BuildTileLayerFromFile(loadFile, room->layer0, usingSprites);
		BuildTileLayerFromFile(loadFile, room->layer1, usingSprites);
	}
	loadFile.Close();
}

void PlayScene::Stage::BuildTileLayerFromFile(FileUtils::File & file, Tile * tileLayer, std::map<std::wstring, D2DSprite *> &usingSprites)
{
	WCHAR buffer[40];

	for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	{
		for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
		{
			int index = x + ROOM_TILE_COUNTX * y;
			Tile &currentTile = tileLayer[index];

			file.GetLine();
			file.Read(L"Image Key : %s\n", &buffer);
			if (wcscmp(buffer, L"e") != 0)
			{
				CheckUsingSpriteExistence(buffer);
				currentTile.sprite = usingSprites.find(buffer)->second;
			}

			file.Read(L"SourceIndex X : %d, Y : %d\n", &currentTile.sourceIndex.x, &currentTile.sourceIndex.y);
			file.Read(L"Destroyed Index X : %d, Y : %d\n",
				&currentTile.destroyedIndex.x, &currentTile.destroyedIndex.y);
			file.Read(L"This Mask Info : %u\n", &currentTile.thisMaskInfo);
			file.Read(L"Near Mask Info : %u\n", &currentTile.nearMaskInfo);
			file.Read(L"Collision Type : %d\n", &currentTile.collisionType);
			file.Read(L"Mask Info : %u\n", &currentTile.maskInfo);
			file.Read(L"Layer : %d\n", &currentTile.layer);

			ZeroMemory(buffer, 40);
		}
	}
}

void PlayScene::Stage::TileInfoBitmaskCopy(D2DSprite *sourSprite, Tile &sourTile, Tile &maskTile, uint32 offset)
{
	sourTile.maskInfo |= (1 << offset);
	maskTile.maskInfo |= (1 << offset);

	maskTile.sprite = sourSprite;

	sourTile.spriteMaskInfo[offset].hasMask = true;
	sourTile.spriteMaskInfo[offset].maskSprite = sourSprite;

	maskTile.spriteMaskInfo[offset].hasMask = true;
	maskTile.spriteMaskInfo[offset].maskSprite = sourSprite;
}
