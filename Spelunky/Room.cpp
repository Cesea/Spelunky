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
	RegisterDelegates();
}

Stage::~Stage()
{
	UnRegisterDelegates();
	for (int y = 0; y < STAGE_TOTAL_COUNTY; ++y)
	{
		for (int x = 0; x < STAGE_TOTAL_COUNTX; ++x)
		{
			Tile *pLayer0Tile = tileLayer0[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)];
			Tile *pLayer1Tile = tileLayer1[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)];
			Tile *pLayer2Tile = tileLayer2[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)];

			if (pLayer0Tile)
			{
				pLayer0Tile->Release();
				OBJECTMANAGER->DestroyObject(pLayer0Tile->GetId());
			}
			if (pLayer1Tile)
			{
				pLayer1Tile->Release();
				OBJECTMANAGER->DestroyObject(pLayer1Tile->GetId());
			}
			if (pLayer2Tile)
			{
				pLayer2Tile->Release();
				OBJECTMANAGER->DestroyObject(pLayer2Tile->GetId());
			}
		}
	}
	_tunnels[0]->Release();
	_tunnels[1]->Release();
	OBJECTMANAGER->DestroyObject(_tunnels[0]->GetId());
	OBJECTMANAGER->DestroyObject(_tunnels[1]->GetId());

	for (auto &gem : _gems)
	{
		gem->Release();
		OBJECTMANAGER->DestroyObject(gem->GetId());
	}

	for (auto &throws : _throws)
	{
		throws->Release();
		OBJECTMANAGER->DestroyObject(throws->GetId());
	}

	for (auto &enemy : _enemies)
	{
		enemy->Release();
		OBJECTMANAGER->DestroyObject(enemy->GetId());
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
	BuildEnemies();
	BuildCrates();
	BuildObstacles();
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

void Stage::PostInit()
{
	for (auto &enemy : _enemies)
	{
		enemy->PostInit();
	}
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
	CalculateMask(xStartIndex, yStartIndex, width, height, 0);
	CalculateMask(xStartIndex, yStartIndex, width, height, 1);
	CalculateMask(xStartIndex, yStartIndex, width, height, 2);
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
	else if (layer == 2)
	{
		tileLayer = tileLayer2;
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
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y],  0);
						}
					}
					//위에 타일이 있다. 
					else
					{
						if (((tileLayer[x + STAGE_TOTAL_COUNTX * upperY]->nearMaskInfo >> 3) & 1) &&
							(tileLayer[index]->thisMaskInfo >> 0) & 1)
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y],  0);
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
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y],  1);
						}
					}
					//왼쪽 타일이 있다
					else
					{
						if (((tileLayer[leftX + STAGE_TOTAL_COUNTX * y]->nearMaskInfo >> 2) & 1) &&
							((tileLayer[index]->thisMaskInfo >> 1) & 1))
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y], 1);
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
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y],  2);
						}
					}
					//오른쪽 타일이 있다
					else
					{
						if (((tileLayer[rightX + STAGE_TOTAL_COUNTX * y]->nearMaskInfo >> 1) & 1) &&
							((tileLayer[index]->thisMaskInfo >> 2) & 1))
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y], 2);
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
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y],  3);
						}
					}
					//아래쪽 타일이 있다
					else
					{
						if (((tileLayer[x + STAGE_TOTAL_COUNTX * lowerY]->nearMaskInfo >> 0) & 1) &&
							(tileLayer[index]->thisMaskInfo >> 3) & 1)
						{
							TileInfoBitmaskCopy(thisTileSprite, tileLayer[x + STAGE_TOTAL_COUNTX * y], 3);
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
	for (auto &gem : _gemFowardRenderList)
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
	for (auto &bomb : _bombs)
	{
		bomb->Update(deltaTime);
	}

	for (auto &crate : _crates)
	{
		crate->Update(deltaTime);
	}
	for (auto &obstacle : _obstacles)
	{
		obstacle->Update(deltaTime);
	}

	for (auto &eatable : _eatables)
	{
		eatable->Update(deltaTime);
	}

	for (auto &enemy : _enemies)
	{
		enemy->Update(deltaTime);
	}
}

void Stage::Render(const TilePosition &camPos)
{
	int minX = camPos.tileX;
	int maxX = camPos.tileX + 21;
	int minY = camPos.tileY;
	int maxY = camPos.tileY + 12;
	if (minX < 0) { minX = 0; }
	if (minY < 0) { minY = 0; }
	if (maxX > STAGE_TOTAL_COUNTX - 1) { maxX = STAGE_TOTAL_COUNTX - 1; }
	if (maxY > STAGE_TOTAL_COUNTY - 1) { maxY = STAGE_TOTAL_COUNTY - 1; }

	Vector2 untileldCamPos = camPos.UnTilelize();

}

void Stage::RenderDeeperLayer(int minX, int maxX, int minY, int maxY, const Vector2 &camPos)
{
	int backMinX = minX / 4;
	int backMaxX = maxX / 4;
	int backMinY = minY / 4;
	int backMaxY = maxY / 4;

	for (int y = backMinY; y <= backMaxY; ++y)
	{
		for (int x = backMinX; x <= backMaxX; ++x)
		{
			_backgroundSprite->Render(gRenderTarget, x * BACKGROUND_SIZE - camPos.x, 
				y * BACKGROUND_SIZE - camPos.y);
		}
	}

	_tunnels[0]->Render(gRenderTarget, camPos);
	_tunnels[1]->Render(gRenderTarget, camPos);

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			if (tileLayer0[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)])
			{
				tileLayer0[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)]->Render(gRenderTarget, camPos);
			}
		}
	}
	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			if (tileLayer3[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)])
			{
				tileLayer3[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)]->Render(gRenderTarget, camPos);
			}
		}
	}
}

void Stage::RenderBlockingLayer(int minX, int maxX, int minY, int maxY, const Vector2 &camPos)
{
	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			if (tileLayer1[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)])
			{
				tileLayer1[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)]->Render(gRenderTarget, camPos);
			}
		}
	}

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			if (tileLayer2[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)])
			{
				tileLayer2[GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX)]->Render(gRenderTarget, camPos);
			}
		}
	}

	for (auto &gem : _gemFowardRenderList)
	{
		gem->Render(gRenderTarget, camPos);
	}
}

void Stage::RenderObjects(const Vector2 & camPos)
{
	for (auto &throws : _throws)
	{
		throws->Render(gRenderTarget, camPos);
	}
	for (auto &gem : _gems)
	{
		gem->Render(gRenderTarget, camPos);
	}
	for (auto &bomb : _bombs)
	{
		bomb->Render(gRenderTarget, camPos);
	}
	for (auto &eatable : _eatables)
	{
		eatable->Render(gRenderTarget, camPos);
	}
	for (auto &crate : _crates)
	{
		crate->Render(gRenderTarget, camPos);
	}
	for (auto &enemy : _enemies)
	{
		enemy->Render(gRenderTarget, camPos);
	}
	for (auto &obstacle : _obstacles)
	{
		obstacle->Render(gRenderTarget, camPos);
	}
}

void Stage::DestroyTile(const IntVector2 & tilePos)
{
	bool destroyed = false;

	int index = GetIndexFromXY(tilePos.x, tilePos.y, STAGE_TOTAL_COUNTX);

	Tile *pLayer0Tile = tileLayer0[index];
	Tile *pLayer1Tile = tileLayer1[index];
	Tile *pLayer2Tile = tileLayer2[index];
	Tile *pLayer3Tile = tileLayer3[index];

	TileCollisionType breakTileCollType{ TILE_COLLISION_NONE };

	if (pLayer0Tile != nullptr &&
		pLayer0Tile->canBeDestroyedByBomb)
	{
		breakTileCollType = pLayer0Tile->collisionType;

		pLayer0Tile->canBeDestroyedByBomb = false;
		pLayer0Tile->sourceIndex = pLayer0Tile->destroyedIndex;
		pLayer0Tile->collisionType = TileCollisionType::TILE_COLLISION_NONE;
		pLayer0Tile->thisMaskInfo = 0;
		pLayer0Tile->nearMaskInfo = 15;
		destroyed = true;
	}
	else if(pLayer1Tile != nullptr &&
		pLayer1Tile->canBeDestroyedByBomb)
	{
		breakTileCollType = pLayer1Tile->collisionType;

		pLayer1Tile->canBeDestroyedByBomb = false;
		pLayer1Tile->sourceIndex = pLayer1Tile->destroyedIndex;
		pLayer1Tile->collisionType = TileCollisionType::TILE_COLLISION_NONE;
		pLayer1Tile->thisMaskInfo = 0;
		pLayer1Tile->nearMaskInfo = 15;
		destroyed = true;

		tileLayer0[index] = std::move(pLayer1Tile);
		tileLayer1[index] = nullptr;
	}
	else if (pLayer2Tile != nullptr &&
		pLayer2Tile->canBeDestroyedByBomb)
	{
		pLayer2Tile->canBeDestroyedByBomb = false;
		pLayer2Tile->sourceIndex = pLayer2Tile->destroyedIndex;
		pLayer2Tile->collisionType = TileCollisionType::TILE_COLLISION_NONE;
		pLayer2Tile->thisMaskInfo = 0;
		pLayer2Tile->nearMaskInfo = 15;
		destroyed = true;

		tileLayer0[index] = std::move(pLayer2Tile);
		tileLayer2[index] = nullptr;
	}

	if (pLayer3Tile != nullptr &&
		pLayer3Tile->canBeDestroyedByBomb)
	{
		OBJECTMANAGER->DestroyObject(pLayer3Tile->GetId());
		tileLayer3[index] = nullptr;
	}


	if (destroyed)
	{
		for (auto &iter = _gemFowardRenderList.begin(); iter != _gemFowardRenderList.end();)
		{
			if ((*iter)->position.tileX == tilePos.x && (*iter)->position.tileY == tilePos.y)
			{
				if ((*iter)->IsStone())
				{
					(*iter)->Digged();
					_gems.push_back(*iter);
					_gemFowardRenderList.erase(iter);
				}
				break;
			}
			else
			{
				++iter;
			}
		}

		for (auto &gem : _gems)
		{
			if (gem->position.tileX == tilePos.x && gem->position.tileY == tilePos.y)
			{
				gem->Digged();
				break;
			}
		}

		for (auto &throws : _throws)
		{
			if ((throws->position.tileX == tilePos.x)
				&& (throws->position.tileY == tilePos.y))
			{
				EVENTMANAGER->QueueEvent(new ItemBreakEvent(throws->GetId(), throws->GetBreakType()));
			}
		}

		int upperIndex = GetIndexFromXY(tilePos.x, tilePos.y - 1, STAGE_TOTAL_COUNTX);
		Tile *upperTile = tileLayer1[upperIndex];
		if (upperTile &&
			upperTile->collisionType == TileCollisionType::TILE_COLLISION_UPPER_DEAD)
		{
			tileLayer1[upperIndex] = nullptr;
			OBJECTMANAGER->DestroyObject(upperTile->GetId());
			EFFECTMANAGER->PlayThornParticle(TilePosition(tilePos.x, tilePos.y, 32, 32).UnTilelize());
		}

		ClearAllTheBits(tilePos.x - 1, tilePos.y - 1, 3, 3);
		CalculateMask(tilePos.x - 1, tilePos.y - 1, 3, 3, 0);
		CalculateMask(tilePos.x - 1, tilePos.y - 1, 3, 3, 1);
		CalculateMask(tilePos.x - 1, tilePos.y - 1, 3, 3, 2);

		if (breakTileCollType == TileCollisionType::TILE_COLLISION_BLOCK)
		{
			EFFECTMANAGER->PlayTileParticle(TilePosition(tilePos.x, tilePos.y, 32, 32).UnTilelize());
		}
		else if (breakTileCollType == TileCollisionType::TILE_COLLISION_LADDER ||
			breakTileCollType == TileCollisionType::TILE_COLLISION_EOF_LADDER)
		{
			EFFECTMANAGER->PlayWoodParticle(TilePosition(tilePos.x, tilePos.y, 32, 32).UnTilelize());
		}
		else if (breakTileCollType == TileCollisionType::TILE_COLLISION_UPPER_DEAD)
		{
			EFFECTMANAGER->PlayThornParticle(TilePosition(tilePos.x, tilePos.y, 32, 32).UnTilelize());
		}

		SOUNDMANAGER->Play(L"boulder_hit_" + std::to_wstring(index % 4));
	}
}

void Stage::DestroyTile(int xStartIndex, int yStartIndex, int width, int height)
{
	bool destroyed = false;

	for (int y = yStartIndex; y <= yStartIndex + height; ++y)
	{
		for (int x = xStartIndex; x <= xStartIndex + width; ++x)
		{
			if ((x == xStartIndex  && y == yStartIndex) || (x == xStartIndex && y == yStartIndex + height) ||
				(x == xStartIndex + width && y == yStartIndex) || (x == xStartIndex + width && y == yStartIndex + height))
			{
				continue;
			}

			if (xStartIndex < 0 || yStartIndex < 0 ||
				xStartIndex + width > STAGE_TOTAL_COUNTX - 1 ||
				yStartIndex + height > STAGE_TOTAL_COUNTY - 1)
			{
				continue;
			}

			int currentIndex = GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX);

			Tile *pLayer0Tile = tileLayer0[currentIndex];
			Tile *pLayer1Tile = tileLayer1[currentIndex];
			Tile *pLayer2Tile = tileLayer2[currentIndex];
			Tile *pLayer3Tile = tileLayer3[currentIndex];

			TileCollisionType breakTileCollType{TILE_COLLISION_NONE};

			if (pLayer0Tile != nullptr &&
				pLayer0Tile->canBeDestroyedByBomb)
			{
				breakTileCollType = pLayer0Tile->collisionType;

				pLayer0Tile->canBeDestroyedByBomb = false;
				pLayer0Tile->sourceIndex = pLayer0Tile->destroyedIndex;
				pLayer0Tile->collisionType = TileCollisionType::TILE_COLLISION_NONE;
				pLayer0Tile->thisMaskInfo = 0;
				pLayer0Tile->nearMaskInfo = 15;
				destroyed = true;

			}
			else if (pLayer1Tile != nullptr &&
				pLayer1Tile->canBeDestroyedByBomb)
			{
				breakTileCollType = pLayer1Tile->collisionType;

				pLayer1Tile->canBeDestroyedByBomb = false;
				pLayer1Tile->sourceIndex = pLayer1Tile->destroyedIndex;
				pLayer1Tile->collisionType = TileCollisionType::TILE_COLLISION_NONE;
				pLayer1Tile->thisMaskInfo = 0;
				pLayer1Tile->nearMaskInfo = 15;
				destroyed = true;

				tileLayer0[currentIndex] = std::move(pLayer1Tile);
				tileLayer1[currentIndex] = nullptr;
			}
			else if (pLayer2Tile != nullptr &&
				pLayer2Tile->canBeDestroyedByBomb)
			{
				pLayer2Tile->canBeDestroyedByBomb = false;
				pLayer2Tile->sourceIndex = pLayer2Tile->destroyedIndex;
				pLayer2Tile->collisionType = TileCollisionType::TILE_COLLISION_NONE;
				pLayer2Tile->thisMaskInfo = 0;
				pLayer2Tile->nearMaskInfo = 15;
				destroyed = true;

				tileLayer0[currentIndex] = std::move(pLayer2Tile);
				tileLayer2[currentIndex] = nullptr;
			}
			if (pLayer3Tile != nullptr &&
				pLayer3Tile->canBeDestroyedByBomb)
			{
				OBJECTMANAGER->DestroyObject(pLayer3Tile->GetId());
				tileLayer3[currentIndex] = nullptr;
			}
			if (destroyed)
			{
				if (y == yStartIndex || y == yStartIndex + 1)
				{
					int upperIndex = GetIndexFromXY(x, y - 1, STAGE_TOTAL_COUNTX);
					Tile *upperTile = tileLayer1[upperIndex];
					if (upperTile &&
						upperTile->collisionType == TileCollisionType::TILE_COLLISION_UPPER_DEAD)
					{
						tileLayer1[upperIndex] = nullptr;

						OBJECTMANAGER->DestroyObject(upperTile->GetId());
						EFFECTMANAGER->PlayThornParticle(TilePosition(x, y, 32, 32).UnTilelize());
					}
				}

				for (auto &iter = _gemFowardRenderList.begin(); iter != _gemFowardRenderList.end();)
				{
					if ((*iter)->position.tileX == x && (*iter)->position.tileY == y)
					{
						if ((*iter)->IsStone())
						{
							(*iter)->Digged();
							_gems.push_back(*iter);
							_gemFowardRenderList.erase(iter);
						}
						break;
					}
					else
					{
						++iter;
					}
				}

				for (auto &gem : _gems)
				{
					if (gem->position.tileX == x && gem->position.tileY == y)
					{
						gem->Digged();
						break;
					}
				}
				for (auto &throws : _throws)
				{
					if ((throws->position.tileX == x) && 
						(throws->position.tileY == y))
					{
						EVENTMANAGER->QueueEvent(new ItemBreakEvent(throws->GetId(), throws->GetBreakType()));
					}
				}
				if (breakTileCollType == TileCollisionType::TILE_COLLISION_BLOCK)
				{
					EFFECTMANAGER->PlayTileParticle(TilePosition(x, y, 32, 32).UnTilelize());
				}
				else if (breakTileCollType == TileCollisionType::TILE_COLLISION_LADDER ||
					breakTileCollType == TileCollisionType::TILE_COLLISION_EOF_LADDER)
				{
					EFFECTMANAGER->PlayWoodParticle(TilePosition(x, y, 32, 32).UnTilelize());
				}
				else if (breakTileCollType == TileCollisionType::TILE_COLLISION_UPPER_DEAD)
				{
					EFFECTMANAGER->PlayThornParticle(TilePosition(x, y, 32, 32).UnTilelize());
				}
				SOUNDMANAGER->Play(L"boulder_hit_" + std::to_wstring(currentIndex % 4));
			}
		}
	}
	if (destroyed)
	{
		ClearAllTheBits(xStartIndex -2 , yStartIndex - 2 , width + 6 , height + 6 );
		CalculateMask(xStartIndex - 2, yStartIndex - 2, width + 6, height + 6, 0);
		CalculateMask(xStartIndex - 2, yStartIndex - 2, width + 6, height + 6, 1);
		CalculateMask(xStartIndex - 2, yStartIndex - 2, width + 6, height + 6, 2);
	}
}

void Stage::RegisterDelegates()
{
	EVENTMANAGER->RegisterDelegate(EVENT_COLLECT_MONEY, EventDelegate::FromFunction<Stage, &Stage::HandleCollectMoneyEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_ITEM_BREAK, EventDelegate::FromFunction < Stage, &Stage::HandleItemBreakEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_THROW_BOMB, EventDelegate::FromFunction<Stage, &Stage::HandleThrowBombEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_ENEMY_DEAD, EventDelegate::FromFunction<Stage, &Stage::HandleEnemyDeadEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_DESTROY_A_TILE, EventDelegate::FromFunction<Stage, &Stage::HandleDestroyATileEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_SPAWN_OBJECT, EventDelegate::FromFunction<Stage, &Stage::HandleSpawnObjectEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_COLLECT_EATABLE, EventDelegate::FromFunction<Stage, &Stage::HandleCollectEatableEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_FIRE_ARROW, EventDelegate::FromFunction<Stage, &Stage::HandleFireArrowEvent>(this));
}


void Stage::UnRegisterDelegates()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_COLLECT_MONEY, EventDelegate::FromFunction<Stage, &Stage::HandleCollectMoneyEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_ITEM_BREAK, EventDelegate::FromFunction < Stage, &Stage::HandleItemBreakEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_THROW_BOMB, EventDelegate::FromFunction<Stage, &Stage::HandleThrowBombEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_ENEMY_DEAD, EventDelegate::FromFunction<Stage, &Stage::HandleEnemyDeadEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_DESTROY_A_TILE, EventDelegate::FromFunction<Stage, &Stage::HandleDestroyATileEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_SPAWN_OBJECT, EventDelegate::FromFunction<Stage, &Stage::HandleSpawnObjectEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_COLLECT_EATABLE, EventDelegate::FromFunction<Stage, &Stage::HandleCollectEatableEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_FIRE_ARROW, EventDelegate::FromFunction<Stage, &Stage::HandleFireArrowEvent>(this));
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
	int randInt = (int)(RND->GetFloat() * 10);

	//switch (types)
	//{
	//case RoomType::ROOM_BLOCK:
	//{
	//	str += L"_block_0" + std::to_wstring(randInt) + L".rt";
	//}break;
	//case RoomType::ROOM_AISLE :
	//{
	//	str += L"_aisle_0" + std::to_wstring(randInt) + L".rt";
	//}break;
	//case RoomType::ROOM_TOP_OPEN :
	//{
	//	str += L"_topopen_0" + std::to_wstring(randInt) + L".rt";
	//}break;
	//case RoomType::ROOM_BOTTOM_OPEN :
	//{
	//	str += L"_bottomopen_0" + std::to_wstring(randInt) + L".rt";
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
	filePath += L"mine_room_type\\";
	filePath += fileName;

	if (loadFile.Open(filePath, FileUtils::FileAccess::Read))
	{
		loadFile.GetLine();
		loadFile.GetLine();

		BuildTileLayerFromFile(loadFile, roomX, roomY, border, usingSprites);
		BuildOrnamentsFromFile(loadFile, roomX, roomY, border, usingSprites);

		CollectRoomPropertyFromFile(loadFile, room);

		loadFile.Close();
	}
}

void Stage::BuildTileLayerFromFile(FileUtils::File & file, 
	int roomX, int roomY, bool border, std::map<std::wstring, D2DSprite *> &usingSprites)
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

				if (wcscmp(buffer, L"woodtile") == 0)
				{
					int a = 0;
				}

				IntVector2 worldPositon{};
				Tile *newTile = (Tile *)OBJECTMANAGER->CreateObject(L"tile", property);

				if (border)
				{
					worldPositon = CalculateBorderWorldIndex(roomX, roomY, x, y);
					newTile->position.tileX = worldPositon.x;
					newTile->position.tileY = worldPositon.y;
					newTile->desiredPosition = newTile->position;
					//if (property->layer == 0)
					//{
					//	tileLayer0[GetIndexFromXY(worldPositon.x, worldPositon.y, STAGE_TOTAL_COUNTX)] = newTile;
					//}
					/*else */
					if (property->layer == 2)
					{
						tileLayer2[GetIndexFromXY(worldPositon.x, worldPositon.y, STAGE_TOTAL_COUNTX)] = newTile;
					}
				}
				else
				{
					worldPositon = CalculateTileWorldIndex(roomX, roomY, x, y);
					newTile->position.tileX = worldPositon.x;
					newTile->position.tileY = worldPositon.y;
					newTile->desiredPosition = newTile->position;
					if (property->layer == 0)
					{
						tileLayer0[GetIndexFromXY(worldPositon.x, worldPositon.y, STAGE_TOTAL_COUNTX)] = newTile;
					}
					else if (property->layer == 1)
					{
						tileLayer1[GetIndexFromXY(worldPositon.x, worldPositon.y, STAGE_TOTAL_COUNTX)] = newTile;
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

void Stage::BuildOrnamentsFromFile(FileUtils::File & file, int roomX, int roomY, bool border, std::map<std::wstring, D2DSprite*>& usingSprites)
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

				if (wcscmp(buffer, L"woodtile") == 0)
				{
					int a = 0;
				}

				IntVector2 worldPositon{};
				Tile *newTile = (Tile *)OBJECTMANAGER->CreateObject(L"tile", property);
				
				if (border)
				{
					worldPositon = CalculateBorderWorldIndex(roomX, roomY, x, y);
					newTile->position.tileX = worldPositon.x;
					newTile->position.tileY = worldPositon.y;
					newTile->desiredPosition = newTile->position;
					if (property->layer == 3)
					{
						tileLayer3[GetIndexFromXY(worldPositon.x, worldPositon.y, STAGE_TOTAL_COUNTX)] = newTile;
					}
				}
				else
				{
					worldPositon = CalculateTileWorldIndex(roomX, roomY, x, y);
					newTile->position.tileX = worldPositon.x;
					newTile->position.tileY = worldPositon.y;
					newTile->desiredPosition = newTile->position;
					if (property->layer == 3)
					{
						tileLayer3[GetIndexFromXY(worldPositon.x, worldPositon.y, STAGE_TOTAL_COUNTX)] = newTile;
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



void Stage::TileInfoBitmaskCopy(D2DSprite * sourSprite, Tile * sourTile, uint32 offset)
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
			auto &found = _rooms[GetIndexFromXY(x, y, 4)].properties.find(L"gem");
			if (found != _rooms[GetIndexFromXY(x, y, 4)].properties.end())
			{
				for (auto &prop : found->second)
				{
					IntVector2 worldPosition = CalculateTileWorldIndex(x, y, prop->position.x, prop->position.y);

					GemProperty *convertedProperty = (GemProperty *)(prop);
					if (convertedProperty->sourceIndex.x == -2)
					{
						if (RND->GetFloat() > 0.4)
						{
							BuildRandomGem(worldPosition);
						}
					}
					else
					{
						Gem *newGem = (Gem *)OBJECTMANAGER->CreateObject(L"gem", prop);
						newGem->position.tileX = worldPosition.x;
						newGem->position.tileY = worldPosition.y;
						//newGem->position.AddToTileRel(32, 32);

						int worldIndex = GetIndexFromXY(newGem->position.tileX, newGem->position.tileY, STAGE_TOTAL_COUNTX);

						if (newGem->IsStone())
						{
							if (tileLayer0[worldIndex] &&
								tileLayer0[worldIndex]->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
							{
								newGem->SetIsInTile(true);
							}
							else
							{
								if (tileLayer1[worldIndex] &&
									tileLayer1[worldIndex]->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
								{
									newGem->SetIsInTile(true);
								}
							}
							newGem->desiredPosition = newGem->position;
							if (newGem->position.tileX)
								_gemFowardRenderList.push_back(newGem);
						}
						else
						{
							if ( tileLayer0[worldIndex] &&
								tileLayer0[worldIndex]->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
							{
								newGem->SetIsInTile(true);
							}
							else
							{
								if (tileLayer1[worldIndex] &&
									tileLayer1[worldIndex]->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
								{
									newGem->SetIsInTile(true);
								}
							}
							newGem->desiredPosition = newGem->position;
							if (newGem->position.tileX)
								_gems.push_back(newGem);
						}
					}
				}
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
			auto &found = _rooms[GetIndexFromXY(x, y, 4)].properties.find(L"throws");
			if (found != _rooms[GetIndexFromXY(x, y, 4)].properties.end())
			{
				for (auto &prop : found->second)
				{
					ThrowProperty *convertedProperty = (ThrowProperty *)(prop);
					IntVector2 worldPosition = CalculateTileWorldIndex(x, y, prop->position.x, prop->position.y);
					if (convertedProperty->sourceIndex.x == -2)
					{
						if (RND->GetFloat() > 0.4)
						{
							ThrowProperty randomProperty{};
							int randSourceX = RND->GetFromIntTo(0, 2);
							randomProperty.sourceIndex.x = randSourceX;
							randomProperty.sourceIndex.y = 0;
							if (randSourceX == 0)
							{
								randomProperty.breakable = false;
							}
							else if (randSourceX == 1)
							{
								randomProperty.breakable = true;
							}
							else if (randSourceX == 2)
							{
								randomProperty.breakable = true;
							}

							if (randSourceX == 2)
							{
								Throws *newThrows = (Throws *)OBJECTMANAGER->CreateObject(L"throws", &randomProperty);

								newThrows->position.tileX = worldPosition.x;
								newThrows->position.tileY = worldPosition.y;
								newThrows->position.AddToTileRelX(16);
								newThrows->position.AddToTileRelY(32);
								newThrows->desiredPosition = newThrows->position;

								randomProperty.sourceIndex.x += 1;
								Throws *newThrowsSecond = (Throws *)OBJECTMANAGER->CreateObject(L"throws", &randomProperty);

								newThrowsSecond->position.tileX = worldPosition.x;
								newThrowsSecond->position.tileY = worldPosition.y;
								newThrows->position.AddToTileRelX(32);
								newThrows->position.AddToTileRelY(32);
								newThrowsSecond->desiredPosition = newThrows->position;

								_throws.push_back(newThrows);
								_throws.push_back(newThrowsSecond);
							}
							else
							{
								Throws *newThrows = (Throws *)OBJECTMANAGER->CreateObject(L"throws", &randomProperty);

								newThrows->position.tileX = worldPosition.x;
								newThrows->position.tileY = worldPosition.y;
								newThrows->position.AddToTileRelX(16);
								newThrows->position.AddToTileRelY(32);
								newThrows->desiredPosition = newThrows->position;

								if (newThrows->position.tileX)
									_throws.push_back(newThrows);
							}
						}
					}
					else
					{
						if (convertedProperty->sourceIndex.x == 2)
						{
							Throws *newThrows = (Throws *)OBJECTMANAGER->CreateObject(L"throws", prop);

							newThrows->position.tileX = worldPosition.x;
							newThrows->position.tileY = worldPosition.y;
							newThrows->position.AddToTileRelX(16);
							newThrows->position.AddToTileRelY(32);
							newThrows->desiredPosition = newThrows->position;

							convertedProperty->sourceIndex.x += 1;
							Throws *newThrowsSecond = (Throws *)OBJECTMANAGER->CreateObject(L"throws", prop);

							newThrowsSecond->position.tileX = worldPosition.x;
							newThrowsSecond->position.tileY = worldPosition.y;
							newThrows->position.AddToTileRelX(32);
							newThrows->position.AddToTileRelY(32);
							newThrowsSecond->desiredPosition = newThrows->position;

							_throws.push_back(newThrows);
							_throws.push_back(newThrowsSecond);
						}
						else
						{
							Throws *newThrows = (Throws *)OBJECTMANAGER->CreateObject(L"throws", prop);

							newThrows->position.tileX = worldPosition.x;
							newThrows->position.tileY = worldPosition.y;
							newThrows->position.AddToTileRelX(16);
							newThrows->position.AddToTileRelY(32);
							newThrows->desiredPosition = newThrows->position;

							if (newThrows->position.tileX)
								_throws.push_back(newThrows);
						}
					}
				}
			}
		}
	}
}

void Stage::BuildEnemies()
{
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			auto &found = _rooms[GetIndexFromXY(x, y, 4)].properties.find(L"enemy");
			if (found != _rooms[GetIndexFromXY(x, y, 4)].properties.end())
			{
				for (auto &prop : found->second)
				{
					IntVector2 worldPosition = CalculateTileWorldIndex(x, y, prop->position.x, prop->position.y);

					EnemyProperty *convertedProperty = (EnemyProperty *)(prop);
					convertedProperty->position.x = worldPosition.x;
					convertedProperty->position.y = worldPosition.y;

					Enemy *newEnemy = (Enemy *)OBJECTMANAGER->CreateObject(L"enemy", convertedProperty);

					if (newEnemy)
					{
						_enemies.push_back(newEnemy);
					}

				}
			}
		}
	}
}

void Stage::BuildCrates()
{
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			auto &found = _rooms[GetIndexFromXY(x, y, 4)].properties.find(L"crate");
			if (found != _rooms[GetIndexFromXY(x, y, 4)].properties.end())
			{
				for (auto &prop : found->second)
				{
					IntVector2 worldPosition = CalculateTileWorldIndex(x, y, prop->position.x, prop->position.y);

					CrateProperty *convertedProperty = (CrateProperty *)(prop);
					convertedProperty->position.x = worldPosition.x;
					convertedProperty->position.y = worldPosition.y;

					Crate *newCrate = (Crate *)OBJECTMANAGER->CreateObject(L"crate", convertedProperty);

					if (newCrate)
					{
						_crates.push_back(newCrate);
					}

				}
			}
		}
	}
}

void Stage::BuildObstacles()
{
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			auto &found = _rooms[GetIndexFromXY(x, y, 4)].properties.find(L"obstacle");
			if (found != _rooms[GetIndexFromXY(x, y, 4)].properties.end())
			{
				for (auto &prop : found->second)
				{
					IntVector2 worldPosition = CalculateTileWorldIndex(x, y, prop->position.x, prop->position.y);

					ObstacleProperty *convertedProperty = (ObstacleProperty *)(prop);
					convertedProperty->position.x = worldPosition.x;
					convertedProperty->position.y = worldPosition.y;

					Obstacle *newObstacle = (Obstacle *)OBJECTMANAGER->CreateObject(L"obstacle", convertedProperty);

					if (newObstacle)
					{
						_obstacles.push_back(newObstacle);
					}

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
	_moneyCollected += convertedEvent->GetValue();

	GameObject *object = OBJECTMANAGER->FindObjectId(convertedEvent->GetId());
	for (auto &iter = _gems.begin(); iter != _gems.end();)
	{
		if ((*iter) == object)
		{
			(*iter)->Release();
			_gems.erase(iter);
			break;
		}
		else
		{
			iter++;
		}
	}
	TilePosition objectPosition = object->position;
	objectPosition.AddToTileRelY(-20);
	EFFECTMANAGER->PlayBlingParticle(objectPosition.UnTilelize());
	object->Release();
	OBJECTMANAGER->DestroyObject(convertedEvent->GetId());
}

void Stage::HandleItemBreakEvent(const IEvent * event)
{
	ItemBreakEvent *convertedEvent = (ItemBreakEvent *)event;
	GameObject *object = OBJECTMANAGER->FindObjectId(convertedEvent->GetId());
	if (object)
	{
		Vector2 objectPosition = object->position.UnTilelize();
		objectPosition.y -= 16;

		BreakType breakType = convertedEvent->GetBreakType();
		switch (breakType)
		{
		case BREAK_Jar:
		{
			EFFECTMANAGER->PlayJarParticle(objectPosition, 0.3f);
			SOUNDMANAGER->Play(L"shatter");
		}break;
		case BREAK_Bone:
		{
			EFFECTMANAGER->PlayBoneParticle(objectPosition, 0.3f);
			SOUNDMANAGER->Play(L"bone_shatter");
		}break;
		case BREAK_Rock:
		{
			EFFECTMANAGER->PlayRockParticle(objectPosition, 0.3f);
		}break;
		case BREAK_BackBone:
		{
			EFFECTMANAGER->PlayBackBoneParticle(objectPosition, 0.3f);
			SOUNDMANAGER->Play(L"bone_shatter");
		}break;
		case BREAK_Crate:
		{
			EFFECTMANAGER->PlayWoodParticle(objectPosition, 0.3f);
			SOUNDMANAGER->Play(L"crate_open");
		}break;
		case BREAK_BombCrate :
		{

		}break;
		case BREAK_Arrow :
		{

		}break;

		case BREAK_Live:
		{

		}break;
		}
		if (breakType == BreakType::BREAK_Jar ||
			breakType == BreakType::BREAK_Bone ||
			breakType == BreakType::BREAK_BackBone ||
			breakType == BreakType::BREAK_Rock)
		{
			for (auto &iter = _throws.begin(); iter != _throws.end();)
			{
				if ((*iter) == object)
				{
					(*iter)->Release();
					_throws.erase(iter);
					break;
				}
				else
				{
					iter++;
				}
			}
		}
		else if (breakType == BreakType::BREAK_Bomb)
		{
			EVENTMANAGER->QueueEvent(new DamageEvent(UNVALID_OBJECT_ID, 5, object->position, RectMake(0, 0, 320, 256), Vector2(-160, -120)));
			for (auto &iter = _bombs.begin(); iter != _bombs.end();)
			{
				if ((*iter) == object)
				{
					(*iter)->Release();
					_bombs.erase(iter);
					break;
				}
				else
				{
					iter++;
				}
			}
		}
		else if (breakType == BreakType::BREAK_Crate)
		{
			for (auto &iter = _crates.begin(); iter != _crates.end();)
			{
				if ((*iter) == object)
				{
					(*iter)->Release();
					_crates.erase(iter);
					break;
				}
				else
				{
					iter++;
				}
			}
		}
		else if (breakType == BreakType::BREAK_BombCrate)
		{
			for (auto &iter = _obstacles.begin(); iter != _obstacles.end();)
			{
				if ((*iter) == object)
				{
					(*iter)->Release();
					_obstacles.erase(iter);
					break;
				}
				else
				{
					iter++;
				}
			}
		}

		Console::Log("destroyed Item Id %d\n", object->GetId());
		OBJECTMANAGER->DestroyObject(convertedEvent->GetId());
	}
}

void Stage::HandleEnemyDeadEvent(const IEvent * event)
{
	EnemyDeadEvent *convertedEvent = (EnemyDeadEvent *)event;

	GameObject *object = OBJECTMANAGER->FindObjectId(convertedEvent->GetId());
	if (object)
	{
		Vector2 effectPosition = object->position.UnTilelize();
		effectPosition.y -= 32;
		EFFECTMANAGER->PlayBloodParticle(effectPosition, 0.3f);
		for (auto &iter = _enemies.begin(); iter != _enemies.end();)
		{
			if ((*iter) == object)
			{
				_enemies.erase(iter);
				break;
			}
			else
			{
				iter++;
			}
		}
		object->Release();
		OBJECTMANAGER->DestroyObject(convertedEvent->GetId());
	}
}

void Stage::HandleThrowBombEvent(const IEvent * event)
{
	ThrowBombEvent *convertedEvent = (ThrowBombEvent *)event;
	BombProperty property;
	property.sticky = convertedEvent->GetIsSticky();
	property.initialVelocity = convertedEvent->GetInitialVelocity();

	Bomb *newBomb = (Bomb *)OBJECTMANAGER->CreateObject(L"bomb", &property);
	const TilePosition &throwPosition = convertedEvent->GetThrowPosition();
	newBomb->position = throwPosition;
	newBomb->desiredPosition = newBomb->position;

	_bombs.push_back(newBomb);
}

void Stage::HandleDestroyATileEvent(const IEvent * event)
{
	DestroyATileEvent *convertedEvent = (DestroyATileEvent *)(event);
	const TilePosition &tilePosition = convertedEvent->GetTilePosition();
	IntVector2 destroyIndex(tilePosition.tileX, tilePosition.tileY );

	if (convertedEvent->GetDirection() == Direction::Left)
	{
		destroyIndex.x -= 1;
	}
	else if(convertedEvent->GetDirection() == Direction::Right)
	{
		destroyIndex.x += 1;
	}

	Tile *sideTile = GetValidTileAt(destroyIndex.x, destroyIndex.y);
	if (sideTile && sideTile->canBeDestroyedByBomb)
	{
		DestroyTile(destroyIndex);
	}
	else
	{
		destroyIndex.y += 1;
		DestroyTile(destroyIndex);
	}

}

void Stage::HandleSpawnObjectEvent(const IEvent * event)
{
	SpawnObjectEvent *convertedEvent = (SpawnObjectEvent *)(event);
	const TilePosition &eventPos = convertedEvent->GetTilePosition();
	if (convertedEvent->GetKey() == L"gem")
	{
		BuildRandomGem(IntVector2(eventPos.tileX, eventPos.tileY));
	}
	else if (convertedEvent->GetKey() == L"eatable")
	{
		BuildRandomEatable(IntVector2(eventPos.tileX, eventPos.tileY));
	}
}

void Stage::HandleCollectEatableEvent(const IEvent * event)
{
	CollectEatableEvent *convertedEvent = (CollectEatableEvent *)event;

	GameObject *object = OBJECTMANAGER->FindObjectId(convertedEvent->GetId());
	if (object)
	{
		for (auto &iter = _eatables.begin(); iter != _eatables.end();)
		{
			if ((*iter) == object)
			{
				_eatables.erase(iter);
				break;
			}
			else
			{
				iter++;
			}
		}
		object->Release();
		OBJECTMANAGER->DestroyObject(convertedEvent->GetId());
	}
}

void Stage::HandleFireArrowEvent(const IEvent * event)
{
	FireArrowEvent *convertedEvent = (FireArrowEvent *)event;
	ThrowProperty arrowProperty{};
	arrowProperty.sourceIndex.x = 4;
	arrowProperty.sourceIndex.y = 0;

	Throws *arrow = (Throws *)OBJECTMANAGER->CreateObject(L"arrow", &arrowProperty);
	const TilePosition &firePosition = convertedEvent->GetFirePosition();
	arrow->position = firePosition;
	arrow->SetThrowDir(convertedEvent->GetFireDirection());
	arrow->PostInit();

	_throws.push_back(arrow);
}

void Stage::BuildRandomGem(const IntVector2 &genPos)
{
	GemProperty randomGemProperty{};
	randomGemProperty.sourceIndex.y = 0;
	int randSourceX = RND->GetFromIntTo(2, 5);
	randomGemProperty.sourceIndex.x = randSourceX;
	if (randSourceX == 2) { randomGemProperty.value = 800; randomGemProperty.type = GEM_Emerald; }
	else if (randSourceX == 3) { randomGemProperty.value = 1200; randomGemProperty.type = GEM_Saphire; }
	else if (randSourceX == 4) { randomGemProperty.value = 1600; randomGemProperty.type = GEM_Ruby; }
	else if (randSourceX == 5) { randomGemProperty.value = 2000; randomGemProperty.type = GEM_Diamond; }
	Gem *randomGem = (Gem *)OBJECTMANAGER->CreateObject(L"gem", &randomGemProperty);
	randomGem->position.tileX = genPos.x;
	randomGem->position.tileY = genPos.y;
	//randomGem->position.AddToTileRelX(-16.0f);
	randomGem->desiredPosition = randomGem->position;

	int worldIndex = GetIndexFromXY(randomGem->position.tileX, randomGem->position.tileY, STAGE_TOTAL_COUNTX);
	if (tileLayer0[worldIndex] &&
		tileLayer0[worldIndex]->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
	{
		randomGem->SetIsInTile(true);
	}
	else
	{
		if (tileLayer1[worldIndex] &&
			tileLayer1[worldIndex]->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
		{
			randomGem->SetIsInTile(true);
		}
	}

	if (randomGem->position.tileX)
		_gems.push_back(randomGem);
}

void Stage::BuildRandomEatable(const IntVector2 & genPos)
{
	EatableProperty randEatableProperty{};
	randEatableProperty.position = genPos;

	float randFloat = RND->GetFloat();
	if (randFloat < 0.5f)
	{
		randEatableProperty.type = EATABLE_Bomb;
		randEatableProperty.sourceIndex = IntVector2(0, 0);
	}
	else
	{
		randEatableProperty.type = EATABLE_Rope;
		randEatableProperty.sourceIndex = IntVector2(1, 0);
	}
	Eatables *randomEatable = (Eatables *)OBJECTMANAGER->CreateObject(L"eatables", &randEatableProperty);
	randomEatable->position.tileX = genPos.x;
	randomEatable->position.tileY = genPos.y;
	randomEatable->position.AddToTileRelX(-16.0f);
	randomEatable->desiredPosition = randomEatable->position;

	_eatables.push_back(randomEatable);
}

Tile * Stage::GetValidTileAt(int x, int y)
{
	Tile *result = nullptr;

	int index = GetIndexFromXY(x, y, STAGE_TOTAL_COUNTX);

	result = tileLayer0[index];
	if (result == nullptr)
	{
		result = tileLayer1[index];
	}
	if (result == nullptr)
	{
		result = tileLayer2[index];
	}
	return result;
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
			int worldIndex = absTilePos.x + absTilePos.y * STAGE_TOTAL_COUNTX;
			result.tiles[index] = tileLayer0[worldIndex];
			if (result.tiles[index] == nullptr)
			{
				result.tiles[index] = tileLayer1[worldIndex];
			}
			if (result.tiles[index] == nullptr)
			{
				result.tiles[index] = tileLayer2[worldIndex];
			}
			index++;
		}
	}

	result.tiles[8] = tileLayer0[GetIndexFromXY(pos.x, pos.y, STAGE_TOTAL_COUNTX)];
	if (result.tiles[8] == nullptr)
	{
		result.tiles[8] = tileLayer1[GetIndexFromXY(pos.x, pos.y, STAGE_TOTAL_COUNTX)];
	}
	if (result.tiles[8] == nullptr)
	{
		result.tiles[8] = tileLayer2[GetIndexFromXY(pos.x, pos.y, STAGE_TOTAL_COUNTX)];
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



