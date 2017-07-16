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
	BuildBorder();

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
	if (maxX > STAGE_TOTAL_COUNTX - 1) { maxX = STAGE_TOTAL_COUNTX - 1; }
	if (maxY > STAGE_TOTAL_COUNTY - 1) { maxY = STAGE_TOTAL_COUNTY - 1; }
	//ClampInt(&minX, 0, STAGE_TILE_COUNTX);
	//ClampInt(&minY, 0, STAGE_TILE_COUNTY);

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			int index = x + y * STAGE_TOTAL_COUNTX;
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
	if (maxX > STAGE_TOTAL_COUNTX - 1) { maxX = STAGE_TOTAL_COUNTX - 1; }
	if (maxY > STAGE_TOTAL_COUNTY - 1) { maxY = STAGE_TOTAL_COUNTY - 1; }

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			int index = x + y * STAGE_TOTAL_COUNTX;
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
	return tileLayer0[x + (STAGE_TOTAL_COUNTX * y)];
}
Tile & PlayScene::Stage::GetTileRef(int x, int y)
{
	return tileLayer0[x + (STAGE_TOTAL_COUNTX* y)];
}

ReturnTile PlayScene::Stage::GetAdjacent4(const IntVector2 &p)
{
	ReturnTile result{};
	result.tileNum = 4;

	int index = p.x + p.y * STAGE_TOTAL_COUNTX;

	if (p.x > 0)
	{
		result.tiles[0] = &tileLayer0[index - 1];
	}
	if (p.x < STAGE_TOTAL_COUNTX - 2)
	{
		result.tiles[2] = &tileLayer0[index + 1];
	}
	if (p.y > 0)
	{
		result.tiles[1] = &tileLayer0[index - STAGE_TOTAL_COUNTY];
	}
	if (p.y < STAGE_TOTAL_COUNTY - 2)
	{
		result.tiles[3] = &tileLayer0[index + STAGE_TOTAL_COUNTY];
	}

	return result;
}

ReturnTile PlayScene::Stage::GetAdjacent5(const IntVector2 &p)
{
	ReturnTile result{};
	result.tileNum = 5;

	int index = p.x + p.y * STAGE_TOTAL_COUNTX;

	if (p.x > 0)
	{
		result.tiles[0] = &tileLayer0[index - 1];
	}
	if (p.x < STAGE_TOTAL_COUNTX - 2)
	{
		result.tiles[2] = &tileLayer0[index + 1];
	}
	if (p.y > 0)
	{
		result.tiles[1] = &tileLayer0[index - STAGE_TOTAL_COUNTY];
	}
	if (p.y < STAGE_TOTAL_COUNTY - 2)
	{
		result.tiles[3] = &tileLayer0[index + STAGE_TOTAL_COUNTY];
	}

	result.tiles[4] = &tileLayer0[index];

	return result;
}

ReturnTile PlayScene::Stage::GetAdjacentTiles8(const IntVector2 &p)
{
	ReturnTile result{};
	result.tileNum = 8;

	int index = 0;

	for (int y = 1; y >= -1; --y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			if (x == 0 && y == 0) { continue; }

			IntVector2 absTilePos(p.x + x, p.y + y);
			if (absTilePos.x < 0 || absTilePos.x > STAGE_TOTAL_COUNTX - 1 ||
				absTilePos.y < 0 || absTilePos.y > STAGE_TOTAL_COUNTY - 1)
			{
				result.tiles[index] = nullptr;
				index++;
				continue;
			}
			result.tiles[index] = &tileLayer0[absTilePos.x + absTilePos.y * STAGE_TOTAL_COUNTX];
			index++;
		}
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

	return result;
}

ReturnTile PlayScene::Stage::GetAdjacentTiles9(const IntVector2 &p)
{
	ReturnTile result{};
	result.tileNum = 9;

	int index = 0;

	for (int y = 1; y >= -1; --y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			if (x == 0 && y == 0) { continue; }

			IntVector2 absTilePos(p.x + x, p.y + y);
			if (absTilePos.x < 0 || absTilePos.x > STAGE_TOTAL_COUNTX - 1 ||
				absTilePos.y < 0 || absTilePos.y > STAGE_TOTAL_COUNTY - 1)
			{
				result.tiles[index] = nullptr;
				index++;
				continue;
			}
			result.tiles[index] = &tileLayer0[absTilePos.x + absTilePos.y * STAGE_TOTAL_COUNTX];
			index++;
		}
	}

	result.tiles[8] = &tileLayer0[p.x + p.y * STAGE_TOTAL_COUNTX];

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

	return result;
}

void PlayScene::Stage::CalculateMask(int xStartIndex, int yStartIndex, int width, int height)
{
	ClearAllTheBits(xStartIndex, yStartIndex, width, height);
	for (int y = yStartIndex; y < yStartIndex + height; ++y)
	{
		for (int x = xStartIndex; x < xStartIndex + width; ++x)
		{
			int index = x + STAGE_TOTAL_COUNTX * y;
			if (tileLayer0[index].thisMaskInfo)
			{
				D2DSprite *thisTileSprite = tileLayer0[index].sprite;
				//위에 타일이 있는지 검사
				int upperY = y - 1;
				if (upperY >= 0)
				{
					//위에 타일이 없고 현재 타일이 위쪽에 마스크를 그리는 걸 허용하면..
					if ((tileLayer0[x + STAGE_TOTAL_COUNTX * upperY].sourceIndex.x == -1) &&
						((tileLayer0[index].thisMaskInfo >> 0) & 1))
					{
						TileInfoBitmaskCopy(thisTileSprite, tileLayer0[x + STAGE_TOTAL_COUNTX * upperY], tileLayer1[x + STAGE_TOTAL_COUNTX * upperY], 0);
					}
					//위에 타일이 있다. 
					else
					{
						if (((tileLayer0[x + STAGE_TOTAL_COUNTX * upperY].nearMaskInfo >> 3) & 1) &&
							(tileLayer0[index].thisMaskInfo >> 0) & 1)
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer0[x + STAGE_TOTAL_COUNTX * upperY], tileLayer1[x + STAGE_TOTAL_COUNTX * upperY], 0);
						}
					}
				}
				//왼쪽 타일이 있는지 검사
				int leftX = x - 1;
				if (leftX >= 0)
				{
					//왼쪽 타일이 없고 현재 타일이 왼쪽에 마스크를 그리는 걸 허용하면..
					if ((tileLayer0[leftX + STAGE_TOTAL_COUNTX * y].sourceIndex.x == -1) &&
						((tileLayer0[index].thisMaskInfo >> 1) & 1))
					{
						TileInfoBitmaskCopy(thisTileSprite, tileLayer0[leftX + STAGE_TOTAL_COUNTX * y], tileLayer1[leftX + STAGE_TOTAL_COUNTX * y], 1);
					}
					//왼쪽 타일이 있다
					else
					{
						if (((tileLayer0[leftX + STAGE_TOTAL_COUNTX * y].nearMaskInfo >> 2) & 1) &&
							((tileLayer0[index].thisMaskInfo >> 1) & 1))
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer0[leftX + STAGE_TOTAL_COUNTX * y], tileLayer1[leftX + STAGE_TOTAL_COUNTX * y], 1);
						}
					}
				}
				//오른 타일이 있는지 검사
				int rightX = x + 1;
				if (rightX < STAGE_TOTAL_COUNTX)
				{
					//오른쪽 타일이 없고 현재 타일이 오른쪽에 마스크를 그리는 걸 허용하면..
					if ((tileLayer0[rightX + STAGE_TOTAL_COUNTX * y].sourceIndex.x == -1) &&
						((tileLayer0[index].thisMaskInfo >> 2) & 1))
					{
						TileInfoBitmaskCopy(thisTileSprite, tileLayer0[rightX + STAGE_TOTAL_COUNTX * y], tileLayer1[rightX + STAGE_TOTAL_COUNTX * y], 2);
					}
					//오른쪽 타일이 있다
					else
					{
						if (((tileLayer0[rightX + STAGE_TOTAL_COUNTX * y].nearMaskInfo >> 1) & 1) &&
							((tileLayer0[index].thisMaskInfo >> 2) & 1))
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer0[rightX + STAGE_TOTAL_COUNTX * y], tileLayer1[rightX + STAGE_TOTAL_COUNTX * y], 2);
						}
					}
				}
				//아래 타일이 있는지 검사
				int lowerY = y + 1;
				if (lowerY < STAGE_TOTAL_COUNTY)
				{
					//아래쪽 타일이 없고 현재 타일이 아래쪽에 그리는 걸 허용하면..
					if ((tileLayer0[x + STAGE_TOTAL_COUNTX * lowerY].sourceIndex.x == -1) &&
						((tileLayer0[index].thisMaskInfo >> 3) & 1))
					{
						TileInfoBitmaskCopy(thisTileSprite, tileLayer0[x + STAGE_TOTAL_COUNTX * lowerY], tileLayer1[x + STAGE_TOTAL_COUNTX * lowerY], 3);
					}
					//아래쪽 타일이 있다
					else
					{
						if (((tileLayer0[x + STAGE_TOTAL_COUNTX * lowerY].nearMaskInfo >> 0) & 1) &&
							(tileLayer0[index].thisMaskInfo >> 3) & 1)
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer0[x + STAGE_TOTAL_COUNTX * lowerY], tileLayer1[x + STAGE_TOTAL_COUNTX * lowerY], 3);
						}
					}
				}
			}
		}
	}
}

void PlayScene::Stage::ClearAllTheBits(int xStartIndex, int yStartIndex, int width, int height)
{
	for (int i = 0; i < STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY; ++i)
	{
		tileLayer0[i].maskInfo = 0;
		for (int j = 0; j < 4; ++j)
		{
			tileLayer0[i].spriteMaskInfo[j].hasMask = 0;
			tileLayer0[i].spriteMaskInfo[j].maskSprite = nullptr;
		}
	}

	for (int i = 0; i < STAGE_TOTAL_COUNTX * STAGE_TOTAL_COUNTY; ++i)
	{
		tileLayer1[i].maskInfo = 0;
		for (int j = 0; j < 4; ++j)
		{
			tileLayer1[i].spriteMaskInfo[j].hasMask = 0;
			tileLayer1[i].spriteMaskInfo[j].maskSprite = nullptr;
		}
	}
}

void PlayScene::Stage::BuildBorder()
{
	std::wstring path = LdataPath;
	Room borderRooms[4];
	BuildRoomFromFile( L"border_top_left.rt", &borderRooms[0], _usingSprites);
	//BuildRoomFromFile(path + L"border_top.rt", &borderRooms[1], _usingSprites);
	BuildRoomFromFile(L"border_top_right.rt", &borderRooms[1], _usingSprites);
	//BuildRoomFromFile(path + L"border_right.rt", &borderRooms[3], _usingSprites);
	BuildRoomFromFile(L"border_bottom_right.rt", &borderRooms[2], _usingSprites);
	//BuildRoomFromFile(path + L"border_bottom.rt", &borderRooms[5], _usingSprites);
	BuildRoomFromFile(L"border_bottom_left.rt", &borderRooms[3], _usingSprites);
	//BuildRoomFromFile(path + L"border_left.rt", &borderRooms[7], _usingSprites);
	CopyBorderTiles(borderRooms);
}

void PlayScene::Stage::CopyBorderTiles(Room * rooms)
{
	for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	{
		for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
		{
			int worldIndex = x + STAGE_TOTAL_COUNTX * y;
			int localIndex = x + ROOM_TILE_COUNTX * y;
			tileLayer0[worldIndex] = rooms[0].layer0[localIndex];
			tileLayer1[worldIndex] = rooms[0].layer1[localIndex];
			tileLayer0[worldIndex].position = IntVector2(x, y);
			tileLayer1[worldIndex].position = IntVector2(x, y);
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
			tileLayer1[worldIndex] = rooms[1].layer1[localX + localY * ROOM_TILE_COUNTX];
			tileLayer0[worldIndex].position = IntVector2(localX, localY);
			tileLayer1[worldIndex].position = IntVector2(localX, localY);
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
			tileLayer1[worldIndex] = rooms[2].layer1[localX + ROOM_TILE_COUNTX * localY];
			tileLayer0[worldIndex].position = IntVector2(localX, localY);
			tileLayer1[worldIndex].position = IntVector2(localX, localY);
		}
	}

	for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	{
		for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
		{
			int worldIndex = (STAGE_TOTAL_COUNTX - ROOM_TILE_COUNTX + x) + STAGE_TOTAL_COUNTX * (STAGE_TOTAL_COUNTY - ROOM_TILE_COUNTY + y);
			int localIndex = x + ROOM_TILE_COUNTX * y;
			tileLayer0[worldIndex] = rooms[3].layer0[localIndex];
			tileLayer1[worldIndex] = rooms[3].layer1[localIndex];
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

			int xStartIndex = x * ROOM_TILE_COUNTX + 3;
			int yStartIndex = y * ROOM_TILE_COUNTY + 3;

			for (int j = yStartIndex; j < yStartIndex + ROOM_TILE_COUNTY; ++j)
			{
				for (int i = xStartIndex; i < xStartIndex + ROOM_TILE_COUNTX; ++i)
				{
					tileLayer0[i + STAGE_TOTAL_COUNTX * j] = currentRoom.layer0[(i - xStartIndex) + ROOM_TILE_COUNTX * (j - yStartIndex)];
					tileLayer1[i + STAGE_TOTAL_COUNTX * j] = currentRoom.layer1[(i - xStartIndex) + ROOM_TILE_COUNTX * (j - yStartIndex)];

					tileLayer0[i + STAGE_TOTAL_COUNTX * j].position = IntVector2(i, j);
					tileLayer1[i + STAGE_TOTAL_COUNTX * j].position = IntVector2(i, j);
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

void PlayScene::Stage::BuildRoomFromFile(const std::wstring &fileName, Room *room, std::map<std::wstring, D2DSprite *> &usingSprites)
{
	FileUtils::File loadFile;

	std::wstring filePath;
	filePath += LdataPath;
	filePath += fileName;

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
