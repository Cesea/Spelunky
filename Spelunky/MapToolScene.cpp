#include "stdafx.h"
#include "MapToolScene.h"

using namespace MapTool;

MapToolScene::MapToolScene()
{
}

MapToolScene::~MapToolScene()
{
}

HRESULT MapToolScene::LoadContent()
{
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\alltiles.png", L"alltiles");

	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\minetile.png", L"minetile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\woodtile.png", L"woodtile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\jungletile.png", L"jungletile");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\templetile.png", L"templetile");

	return S_OK;
}

void MapToolScene::Release(void)
{
}

HRESULT MapToolScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));
	_timer.Init(1.0f);
	_applySuccessFlagTimer.Init(1.0f);

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

	//_editingTileSet = new TileSet<TileInfo>(ROOM_TILE_COUNTX, ROOM_TILE_COUNTY, TileInfo(L"", IntVector2(-1, -1)));

	_gridSelectorSprite = new D2DSprite;

	_loadImageTextWidth = _dWrite.CalculateInputTextWidth(_loadImageText);
	_saveTextWidth = _dWrite.CalculateInputTextWidth(_save);
	_saveMapTextWidth = _dWrite.CalculateInputTextWidth(_saveMapText);
	_loadMapTextWidth = _dWrite.CalculateInputTextWidth(_loadMapText);
	_drawTextWidth = _dWrite.CalculateInputTextWidth(_drawText);
	_eraseTextWidth = _dWrite.CalculateInputTextWidth(_eraseText);
	_maskTextWidth = _dWrite.CalculateInputTextWidth(_maskText);

	_saveImagePropertyWidth = _dWrite.CalculateInputTextWidth(_saveImageProperty);

	return S_OK;
}


void MapToolScene::Update(void)
{
	float deltaTime = TIMEMANAGER->GetElapsedTime();

	IM::IMGUIPrepare();

	Win32RawInputState rawInput = {};
	PullRawInput(&rawInput);

	IM::uistate.mouseX = (int)rawInput.mouse.currentPoint.x;
	IM::uistate.mouseY = (int)rawInput.mouse.currentPoint.y;

	if (rawInput.mouse.leftButton.down)
	{
		IM::uistate.mouseLeftDown = true;
		if (rawInput.mouse.leftButton.pressed)
		{
			IM::uistate.lastLeftMouseX = (int)rawInput.mouse.currentPoint.x;
			IM::uistate.lastLeftMouseY = (int)rawInput.mouse.currentPoint.y;
		}
	}
	else if (rawInput.mouse.leftButton.released)
	{
		IM::uistate.mouseLeftDown = false;

		IM::uistate.leftDragWidth =  (int)rawInput.mouse.currentPoint.x - IM::uistate.lastLeftMouseX;
		IM::uistate.leftDragHeight = (int)rawInput.mouse.currentPoint.y - IM::uistate.lastLeftMouseY;
	}

	if (rawInput.mouse.rightButton.down)
	{
		IM::uistate.mouseRightDown = true;
	}
	else if (rawInput.mouse.rightButton.released)
	{
		IM::uistate.mouseRightRelease = true;
		IM::uistate.mouseRightDown = false;

		IM::uistate.lastRightMouseX = (rawInput.mouse.currentPoint.x);
		IM::uistate.lastRightMouseY = (rawInput.mouse.currentPoint.y);
	}
	ControlCommand playerCommand = _inputMapper.InterpretRawInput(&rawInput);
}

void MapToolScene::Render()
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(_sceneClearColor);

	//Begin of First Window
	IM::BeginWindow(20, 20, 552, WINSIZEY - 40, L"Window");

	int selectorResult = IM::ImageGridSelector(GEN_ID, 20, 20, 512, 512, 64, 64, _selectorRect, _gridSelectorSprite);
	if (selectorResult == 2)
	{
		InSyncImageInfo();
	}

	if (IM::Button(GEN_ID, 20, 550, _saveImagePropertyWidth, _saveImageProperty))
	{
		SaveCurrentEditingImageInfoAction();
	}

	IM::Label(GEN_ID, 264, 550, 130, _applySuccessBuffer);
	if (_applyTimerFlag)
	{
		if (_applySuccessFlagTimer.Tick(TIMEMANAGER->GetElapsedTime()))
		{
			_stprintf(_applySuccessBuffer, L"");
			_applyTimerFlag = false;
		}
	}

	IM::TextBox(GEN_ID, 20, 600, 400, _loadImageNameBuffer);
	if (IM::Button(GEN_ID, 430, 600, _loadImageTextWidth, _loadImageText))
	{
		LoadButtonAction();
	}
	IM::EndWindow();
	//End of First Window

	//Begin of Second Window
	IM::BeginWindow(580, 20, 680, WINSIZEY - 40, L"Tile");
	if (IM::GridPainter(GEN_ID, 20, 20, ROOM_TILE_COUNTX * TILE_SIZE, ROOM_TILE_COUNTY * TILE_SIZE,
		TILE_SIZE, TILE_SIZE, _xPainter, _yPainter))
	{
		if (!IM::uistate.editorOn)
		{
			PainterAction();
		}
	}
	//Draw Button
	if (_drawingMode == DrawingMode::Draw)
	{
		FillBox(gRenderTarget, 595, 605, 92, 40, IM::RED);
	}
	if (IM::Button(GEN_ID, 20, 560, _drawTextWidth, _drawText))
	{
		_drawingMode = DrawingMode::Draw;
	}

	if (_drawingMode == DrawingMode::Erase)
	{
		FillBox(gRenderTarget, 694, 605, 92, 40, IM::RED);
	}
	//Erase Button
	if (IM::Button(GEN_ID, 120, 560, _eraseTextWidth, _eraseText))
	{
		_drawingMode = DrawingMode::Erase;
	}
	//BitMask Button
	if (IM::Button(GEN_ID, 240, 560, _maskTextWidth, _maskText))
	{
		CalculateBitMask(_roomInfo.layer0, _roomInfo.layer2);
	}

	IM::TextBox(GEN_ID, 20, 600, 400, _mapLoadSaveNameBuffer);
	//Save Map Action
	if (IM::Button(GEN_ID, 440, 600, _saveMapTextWidth, _saveMapText))
	{
		SaveMapButtonAction();
	}
	//Load Map Action
	if (IM::Button(GEN_ID, 560, 600, _loadMapTextWidth, _loadMapText))
	{
		LoadMapButtonAction();
	}

	IM::EndWindow();
	//End of Second Window

	//이미지가 있으면
	if (_usingImages.size() != 0)
	{
		for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
		{
			for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
			{
				int index = x + y * ROOM_TILE_COUNTX;
				if (_roomInfo.layer0[index].sourceIndex.x != -1)
				{
					const auto & infoRef = _roomInfo.layer0[index];
					const IntVector2 &tileIndex = _roomInfo.layer0[index].sourceIndex;
					const std::wstring &tileString = _roomInfo.layer0[index].imageKey;
					auto &found = _usingImages.find(tileString);
					found->second->FrameRender(gRenderTarget, 600 + x * TILE_SIZE, 70 + y * TILE_SIZE, tileIndex.x, tileIndex.y);
				}
				//RenderMasks(x, y, infoRef.maskInfo, found->second);
			}
		}
		for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
		{
			for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
			{
				int index = x + y * ROOM_TILE_COUNTX;
				if ((_roomInfo.layer2[index].sourceIndex.x != -1) || 
					(_roomInfo.layer2[index].maskInfo != 0))
				{
					const IntVector2 &tileIndex = _roomInfo.layer2[index].sourceIndex;
					const std::wstring &tileString = _roomInfo.layer2[index].imageKey;
					auto &found = _usingImages.find(tileString);
					found->second->FrameRender(gRenderTarget, 600 + x * TILE_SIZE, 70 + y * TILE_SIZE, tileIndex.x, tileIndex.y);

					for (int i = 0; i < 4; ++i)
					{
						if (_roomInfo.layer2[index].imageMaskInfo[i].hasMask)
						{
							auto &found = _usingImages.find(_roomInfo.layer2[index].imageMaskInfo[i].maskImageKey);
							if (i == 0)
							{
								found->second->FrameRender(gRenderTarget, 600 + x * TILE_SIZE, 70 + y * TILE_SIZE + TILE_SIZE_HALF, 5, 0);
							}
							else if (i == 1)
							{
								found->second->FrameRender(gRenderTarget, 600 + x * TILE_SIZE + TILE_SIZE_HALF, 70 + y * TILE_SIZE, 7, 2);
							}
							else if (i == 2)
							{
								found->second->FrameRender(gRenderTarget, 600 + x * TILE_SIZE - TILE_SIZE_HALF, 70 + y * TILE_SIZE, 7, 1);
							}
							else if (i == 3)
							{
								found->second->FrameRender(gRenderTarget, 600 + x * TILE_SIZE, 70 + y * TILE_SIZE - TILE_SIZE_HALF, 5, 1);
							}
						}
					}
				}
			}
		}
	}

	if (IM::uistate.editorOn)
	{
		if ( IM::uistate.mouseRightRelease && 
			IM::RegionHit(IM::uistate.editorWindowX, IM::uistate.editorWindowY, 620, 150))
		{
			if (OutSyncImageInfo())
			{
				_applyTimerFlag = true;
				_stprintf(_applySuccessBuffer, L"Applied");
			}
			else
			{
				_applyTimerFlag = true;
				_stprintf(_applySuccessBuffer, L"Failed");
			}
			IM::uistate.editorOn = false;
			IM::uistate.editorWindowX = 0;
			IM::uistate.editorWindowY = 0;
		}
		else
		{
			WCHAR editorText[20];
			_stprintf(editorText, L"Tile At %d, %d", _selectorRect.x, _selectorRect.y);
			IM::BeginPropertyWindow(IM::uistate.lastRightMouseX, IM::uistate.lastRightMouseY, 620, 150, editorText);

			IM::Label(GEN_ID, 10, 10, 150, L"Bom Destroy");
			IM::Label(GEN_ID, 10, 45, 150, L"Destroyed Index");
			IM::Label(GEN_ID, 10, 80, 150, L"This Mask");
			
			IM::Label(GEN_ID, 310, 10, 150, L"Near Mask");
			IM::Label(GEN_ID, 310, 45, 150, L"Collision Type");
			IM::Label(GEN_ID, 310, 80, 150, L"Layer");
			IM::TextBox(GEN_ID, 170, 10, 130, _boomDestroyBuffer);
			IM::TextBox(GEN_ID, 170, 45, 130, _destroyedIndexBuffer);
			IM::TextBox(GEN_ID, 170, 80, 130, _thisMaskBuffer);
			IM::TextBox(GEN_ID, 470, 10, 130, _nearMaskBuffer);
			IM::TextBox(GEN_ID, 470, 45, 130, _collisionTypeBuffer);
			IM::TextBox(GEN_ID, 470, 80, 130, _layerBuffer);
		}
	}
	IM::IMGUIFinish();
	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}

void MapToolScene::LoadButtonAction()
{
	//SAFE_DELETE(_gridSelectorSprite);
	std::wstring name = _loadImageNameBuffer;
	D2DImage *image = IMAGEMANAGER->GetImage(name);
	//load success
	if (image)
	{
		_gridSelectorSprite->Init(image, 0, 0, 512, 512, IntVector2());
		for (int i = 0; i < 8 * 8; ++i)
		{
			_editingTileImageInfo.applied[i] = false;
			_editingTileImageInfo.tileInfos[i] = TileInfo();
		}
		for (int y = 0; y < 8; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				_editingTileImageInfo.tileInfos[x + 8 * y].sourceIndex.x = x;
				_editingTileImageInfo.tileInfos[x + 8 * y].sourceIndex.y = y;
				_editingTileImageInfo.tileInfos[x + 8 * y].imageKey = name;
			}
		}
		LoadEditingTileImageInfo();
	}
	//load failed
	else
	{
		_stprintf(_loadImageNameBuffer, L"ERROR!! No Such Sprite");
	}
}
void MapToolScene::PainterAction()
{
	if (_gridSelectorSprite->GetSourceImage())
	{
		if (_drawingMode == DrawingMode::Draw)
		{
			const std::wstring &imageName = _gridSelectorSprite->GetSourceImage()->GetName();
			//SelectorRect의 크기만큼 루프를 돈다
			for (int y = 0; y <= _selectorRect.height; ++y)
			{
				for (int x = 0; x <= _selectorRect.width; ++x)
				{
					int paintX = _xPainter + x;
					int paintY = _yPainter + y;
					ClampInt(&paintX, 0, ROOM_TILE_COUNTX - 1);
					ClampInt(&paintY, 0, ROOM_TILE_COUNTY - 1);

					int selectX = _selectorRect.x + x;
					int selectY = _selectorRect.y + y;

					const TileInfo & selectorInfo = _editingTileImageInfo.tileInfos[selectX + 8 * selectY];
					//LAYER_TILE일때
					if (selectorInfo.layer == 0)
					{
						_roomInfo.layer0[paintX + ROOM_TILE_COUNTX * paintY] = selectorInfo;
					}
					else if (selectorInfo.layer == 2)
					{
						_roomInfo.layer2[paintX + ROOM_TILE_COUNTX * paintY] = selectorInfo;
					}
				}
			}

			auto &found = _usingImages.find(imageName);
			if (found == _usingImages.end())
			{
				D2DSprite *newSprite = new D2DFrameSprite;
				newSprite->Init(_gridSelectorSprite->GetSourceImage(), 64, 64, IntVector2(0, 0));
				_usingImages.insert(std::make_pair(imageName, newSprite));
			}
		}
		else if(_drawingMode == DrawingMode::Erase)
		{
			_roomInfo.layer0[_xPainter + ROOM_TILE_COUNTX * _yPainter] = TileInfo();
			_roomInfo.layer2[_xPainter + ROOM_TILE_COUNTX * _yPainter] = TileInfo();
		}
	}
}
void MapToolScene::SaveMapButtonAction()
{
	FileUtils::File saveFile;

	char filePath[40];
	strcpy(filePath, dataPath);

	char tempBuffer[40];
	wcstombs(tempBuffer, _mapLoadSaveNameBuffer, 29);

	strcat(filePath, tempBuffer);
	if (saveFile.Open(filePath, FileUtils::FileAccess::Write))
	{
		WCHAR wBuffer[40];
		mbstowcs(wBuffer, filePath, 39);

		saveFile.Write(L"--%s--\n\n", wBuffer);
		//Write for first chunk
		WriteTileInfoChunkForMap(saveFile, _roomInfo.layer0, ROOM_TILE_COUNTX, ROOM_TILE_COUNTY);
		//Write for second chunk
		WriteTileInfoChunkForMap(saveFile, _roomInfo.layer1, ROOM_TILE_COUNTX, ROOM_TILE_COUNTY);
		//Write for third chunk
		WriteTileInfoChunkForMap(saveFile, _roomInfo.layer2, ROOM_TILE_COUNTX, ROOM_TILE_COUNTY);
	}

	saveFile.Close();
}
void MapToolScene::LoadMapButtonAction()
{
	FileUtils::File loadFile;

	char filePath[40];
	strcpy(filePath, dataPath);

	char tempBuffer[40];
	wcstombs(tempBuffer, _mapLoadSaveNameBuffer, 29);

	strcat(filePath, tempBuffer);
	if (loadFile.Open(filePath, FileUtils::FileAccess::Read))
	{
		loadFile.GetLine();
		loadFile.GetLine();

		ReadTileInfoChunkForMap(loadFile, _roomInfo.layer0, ROOM_TILE_COUNTX, ROOM_TILE_COUNTY);
		ReadTileInfoChunkForMap(loadFile, _roomInfo.layer1, ROOM_TILE_COUNTX, ROOM_TILE_COUNTY);
		ReadTileInfoChunkForMap(loadFile, _roomInfo.layer2, ROOM_TILE_COUNTX, ROOM_TILE_COUNTY);
	}
	loadFile.Close();
}
void MapToolScene::CheckUsingImageExistence(const std::wstring &key)
{
	auto &found = _usingImages.find(key);
	if (found == _usingImages.end())
	{
		D2DSprite *insertImage = new D2DFrameSprite;
		insertImage->Init(IMAGEMANAGER->GetImage(key), 64, 64, IntVector2());
		_usingImages.insert(std::make_pair(key, insertImage));
	}
}
void MapToolScene::SaveCurrentEditingImageInfoAction()
{
	FileUtils::File saveFile;

	char filePath[40];
	strcpy(filePath, dataPath);

	char tempBuffer[40];
	wcstombs(tempBuffer, _loadImageNameBuffer, 29);

	strcat(filePath, tempBuffer);
	strcat(filePath, ".itf");
	saveFile.Open(filePath, FileUtils::FileAccess::Write);

	WCHAR wBuffer[40];
	mbstowcs(wBuffer, filePath, 39);

	saveFile.Write(L"--%s--\n", wBuffer);
	saveFile.Write(L"%s\n", _loadImageNameBuffer);
	
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			int index = x + 8 * y;
			const TileInfo &currentTileInfo = _editingTileImageInfo.tileInfos[index];
			saveFile.Write(L"At X : %d, Y : %d\n", x, y);
			saveFile.Write(L"Boomb Destroy : %d\n", currentTileInfo.canBeDestroyedByBomb);
			saveFile.Write(L"Destroyed Index X : %d, Y : %d\n", 
				currentTileInfo.destroyedIndex.x, currentTileInfo.destroyedIndex.y);
			saveFile.Write(L"This Mask Info : %u\n", currentTileInfo.thisMaskInfo);
			saveFile.Write(L"Near Mask Info : %u\n", currentTileInfo.nearMaskInfo);
			saveFile.Write(L"Collision Type : %d\n", (int)currentTileInfo.collisionType);
			saveFile.Write(L"Mask Info : %u\n", currentTileInfo.maskInfo);
			saveFile.Write(L"Layer : %d\n", currentTileInfo.layer);
			saveFile.Write(L"Applied : %d\n", _editingTileImageInfo.applied[index]);
		}
	}
	saveFile.Close();
}
void MapToolScene::LoadEditingTileImageInfo()
{
	FileUtils::File loadFile;

	char filePath[40];
	strcpy(filePath, dataPath);

	char tempBuffer[40];
	wcstombs(tempBuffer, _loadImageNameBuffer, 29);

	strcat(filePath, tempBuffer);
	strcat(filePath, ".itf");

	if (loadFile.Open(filePath, FileUtils::FileAccess::Read))
	{

		loadFile.GetLine();
		loadFile.GetLine();

		for (int y = 0; y < 8; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				int index = x + 8 * y;
				const TileInfo &currentTileInfo = _editingTileImageInfo.tileInfos[index];
				loadFile.GetLine();
				loadFile.Read(L"Boomb Destroy : %d\n", &currentTileInfo.canBeDestroyedByBomb);
				loadFile.Read(L"Destroyed Index X : %d, Y : %d\n",
					&currentTileInfo.destroyedIndex.x, &currentTileInfo.destroyedIndex.y);
				loadFile.Read(L"This Mask Info : %u\n", &currentTileInfo.thisMaskInfo);
				loadFile.Read(L"Near Mask Info : %u\n", &currentTileInfo.nearMaskInfo);
				loadFile.Read(L"Collision Type : %d\n", &currentTileInfo.collisionType);
				loadFile.Read(L"Mask Info : %u\n", &currentTileInfo.maskInfo);
				loadFile.Read(L"Layer : %d\n", &currentTileInfo.layer);
				loadFile.Read(L"Applied : %d\n", &_editingTileImageInfo.applied[index]);
			}
		}
		loadFile.Close();
	}
}

//비트 마스크 검사...
//만약에 현재 타일에다가 비트마스크를 적용을 할수 있는지에 대한정보를 확인 한다.
//만약 현재 타일에 비트마스크를 그릴 수 있다면...
//상, 하, 좌, 우 타일이 있는지 검사를 한다.
//만약 타일이 없다면 현재 타일에다 해당되는 방향에 비트마스크를 적용 시킨다.
//만약 타일이 있다면 해당 타일이 특정 방향의 비트마스크를 적용시키는지에 대한 정보를 확인한다.
//정보에 따라서 현재 타일에 대한 비트마스크 정보를 업데이트 시킨다.
void MapToolScene::CalculateBitMask(TileInfo *sourceLayer, TileInfo *maskLayer)
{
	ClearAllTheBits(&_roomInfo);
	for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	{
		for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
		{
			int index = x + ROOM_TILE_COUNTX * y;
			if (sourceLayer[index].thisMaskInfo)
			{
				const std::wstring &imageKey = sourceLayer[index].imageKey;
				//위에 타일이 있는지 검사
				int upperY = y - 1;
				if (upperY >= 0)
				{
					//위에 타일이 없고 현재 타일이 위쪽에 마스크를 그리는 걸 허용하면..
					if ((sourceLayer[x + ROOM_TILE_COUNTX * upperY].sourceIndex.x == -1) && 
						((sourceLayer[index].thisMaskInfo >> 0) & 1))
					{
						TileInfoBitmaskCopy(imageKey,
							sourceLayer[x + ROOM_TILE_COUNTX * upperY], maskLayer[x + ROOM_TILE_COUNTX * upperY], 0);
					}
					//위에 타일이 있다. 
					else
					{
						if (((sourceLayer[x + ROOM_TILE_COUNTX * upperY].nearMaskInfo >> 3) & 1) &&
							(sourceLayer[index].thisMaskInfo >> 0) & 1)
						{
							TileInfoBitmaskCopy(imageKey,
								sourceLayer[x + ROOM_TILE_COUNTX * upperY], maskLayer[x + ROOM_TILE_COUNTX * upperY], 0);
						}
					}
				}
				//왼쪽 타일이 있는지 검사
				int leftX = x - 1;
				if (leftX >= 0)
				{
					//왼쪽 타일이 없고 현재 타일이 왼쪽에 마스크를 그리는 걸 허용하면..
					if ((sourceLayer[leftX + ROOM_TILE_COUNTX * y].sourceIndex.x == -1) &&
						((sourceLayer[index].thisMaskInfo >> 1) & 1))
					{
						TileInfoBitmaskCopy(imageKey,
							sourceLayer[leftX + ROOM_TILE_COUNTX * y], maskLayer[leftX + ROOM_TILE_COUNTX * y], 1);
					}
					//왼쪽 타일이 있다
					else
					{
						if (((sourceLayer[leftX + ROOM_TILE_COUNTX * y].nearMaskInfo >> 2) & 1) &&
							((sourceLayer[index].thisMaskInfo >> 1 ) & 1))
						{
							TileInfoBitmaskCopy(imageKey,
								sourceLayer[leftX + ROOM_TILE_COUNTX * y], maskLayer[leftX + ROOM_TILE_COUNTX * y], 1);
						}
					}
				}
				//오른 타일이 있는지 검사
				int rightX = x + 1;
				if (rightX < ROOM_TILE_COUNTX)
				{
					//오른쪽 타일이 없고 현재 타일이 오른쪽에 마스크를 그리는 걸 허용하면..
					if ((sourceLayer[rightX + ROOM_TILE_COUNTX * y].sourceIndex.x == -1) && 
						((sourceLayer[index].thisMaskInfo >> 2) & 1))
					{
						TileInfoBitmaskCopy(imageKey,
							sourceLayer[rightX + ROOM_TILE_COUNTX * y], maskLayer[rightX + ROOM_TILE_COUNTX * y], 2);
					}
					//오른쪽 타일이 있다
					else
					{
						if (((sourceLayer[rightX + ROOM_TILE_COUNTX * y].nearMaskInfo >> 1) & 1) &&
							((sourceLayer[index].thisMaskInfo >> 2) & 1))
						{
							TileInfoBitmaskCopy(imageKey,
								sourceLayer[rightX + ROOM_TILE_COUNTX * y], maskLayer[rightX + ROOM_TILE_COUNTX * y], 2);
						}
					}
				}
				//아래 타일이 있는지 검사
				int lowerY = y + 1;
				if (lowerY < ROOM_TILE_COUNTY)
				{
					//아래쪽 타일이 없고 현재 타일이 아래쪽에 그리는 걸 허용하면..
					if ((sourceLayer[x + ROOM_TILE_COUNTX * lowerY].sourceIndex.x == -1) &&
						((sourceLayer[index].thisMaskInfo >> 3) & 1))
					{
						TileInfoBitmaskCopy(imageKey,
							sourceLayer[x + ROOM_TILE_COUNTX * lowerY], maskLayer[x + ROOM_TILE_COUNTX * lowerY], 3);
					}
					//아래쪽 타일이 있다
					else
					{
						if (((sourceLayer[x + ROOM_TILE_COUNTX * lowerY].nearMaskInfo >> 0) & 1) &&
							(sourceLayer[index].thisMaskInfo >> 3) & 1)
						{
							TileInfoBitmaskCopy(imageKey,
								sourceLayer[x + ROOM_TILE_COUNTX * lowerY], maskLayer[x + ROOM_TILE_COUNTX * lowerY], 3);
						}
					}
				}
			}
		}
	}
}

void MapToolScene::RenderMasks(int drawXIndex, int drawYIndex, const uint32 maskInfo, D2DSprite *image)
{
	////위쪽에 마스크해야하면...
	//if (maskInfo & (1 << 0))
	//{
	//	image->FrameRender(gRenderTarget, 600 + drawXIndex * TILE_SIZE, 70 + drawYIndex * TILE_SIZE + TILE_SIZE_HALF, 5, 0);
	//}
	////왼쪽에 마스크해야하면...
	//if (maskInfo & (1 << 1))
	//{
	//	image->FrameRender(gRenderTarget, 600 + drawXIndex * TILE_SIZE + TILE_SIZE_HALF, 70 + drawYIndex * TILE_SIZE, 7, 2);
	//}
	////오른쪽에 마스크해야하면...
	//if (maskInfo & (1 << 2))
	//{
	//	image->FrameRender(gRenderTarget, 600 + drawXIndex * TILE_SIZE - TILE_SIZE_HALF, 70 + drawYIndex * TILE_SIZE, 7, 1);
	//}
	////아래쪽에 마스크해야하면...
	//if (maskInfo & (1 << 3))
	//{
	//	image->FrameRender(gRenderTarget, 600 + drawXIndex * TILE_SIZE, 70 + drawYIndex * TILE_SIZE - TILE_SIZE_HALF, 5, 1);
	//}

}

void MapToolScene::ClearAllTheBits(RoomInfo * roomInfo)
{
	for (int i = 0; i < ROOM_TILE_COUNTX * ROOM_TILE_COUNTY; ++i)
	{
		roomInfo->layer0[i].maskInfo = 0;
		for (int j = 0; j < 4; ++j)
		{
			roomInfo->layer0[i].imageMaskInfo[j].hasMask = false;
			roomInfo->layer0[i].imageMaskInfo[j].maskImageKey.clear();
		}
	}
	for (int i = 0; i < ROOM_TILE_COUNTX * ROOM_TILE_COUNTY; ++i)
	{
		roomInfo->layer2[i].maskInfo = 0;
		for (int j = 0; j < 4; ++j)
		{
			roomInfo->layer2[i].imageMaskInfo[j].hasMask = false;
			roomInfo->layer2[i].imageMaskInfo[j].maskImageKey.clear();
		}
	}
}

void MapToolScene::TileInfoBitmaskCopy(const std::wstring imageKey, TileInfo &sourTile, TileInfo &maskTile, uint32 offset)
{
	sourTile.maskInfo |= (1 << offset);

	maskTile.maskInfo |= (1 << offset);
	maskTile.imageKey = imageKey;

	sourTile.imageMaskInfo[offset].hasMask = true;
	sourTile.imageMaskInfo[offset].maskImageKey = imageKey;

	maskTile.imageMaskInfo[offset].hasMask = true;
	maskTile.imageMaskInfo[offset].maskImageKey = imageKey;
}

int MapToolScene::InSyncImageInfo()
{
	if (_editingTileImageInfo.applied[_selectorRect.x + 8 * _selectorRect.y])
	{

		TileInfo &currentTileInfo = _editingTileImageInfo.tileInfos[_selectorRect.x + 8 * _selectorRect.y];

		std::wstring str{};

		if (currentTileInfo.canBeDestroyedByBomb) { str = L"true"; }
		else { str = L"false"; }
		wcscpy(_boomDestroyBuffer, str.c_str());

		str = std::to_wstring(currentTileInfo.destroyedIndex.x);
		str += L",";
		str += std::to_wstring(currentTileInfo.destroyedIndex.y);
		wcscpy(_destroyedIndexBuffer, str.c_str());

		str = L"";
		int thisMaskValue = currentTileInfo.thisMaskInfo;
		for (int i = 3; i >= 0; --i)
		{
			if ((thisMaskValue >> i) & 1)
			{
				str += L"1";
			}
			else
			{
				str += L"0";
			}
		}
		wcscpy(_thisMaskBuffer, str.c_str());


		str = L"";
		int nearMaskValue = currentTileInfo.nearMaskInfo;
		for (int i = 3; i >= 0; --i)
		{
			if ((nearMaskValue >> i) & 1)
			{
				str += L"1";
			}
			else
			{
				str += L"0";
			}
		}
		wcscpy(_nearMaskBuffer, str.c_str());

		str = std::to_wstring((int)currentTileInfo.collisionType);
		wcscpy(_collisionTypeBuffer, str.c_str());

		str = std::to_wstring((int)currentTileInfo.layer);
		wcscpy(_layerBuffer, str.c_str());
	}
	return 0;
}
int MapToolScene::OutSyncImageInfo()
{
	int success = true;

	TileInfo &currentTileInfo = _editingTileImageInfo.tileInfos[_selectorRect.x + 8 * _selectorRect.y];

	//폭탄으로 터지는지 안터지는지 체크
	bool boomDes = false;
	if (wcscmp(L"true", _boomDestroyBuffer) == 0 ||
		wcscmp(L"True", _boomDestroyBuffer) == 0)
	{
		boomDes = true;
		//currentTileInfo.canBeDestroyedByBomb = true;
	}
	else if (wcscmp(L"false", _boomDestroyBuffer) == 0 ||
		wcscmp(L"False", _boomDestroyBuffer) == 0)
	{
		boomDes = false;
		//currentTileInfo.canBeDestroyedByBomb = true;
	}
	else
	{
		success = false;
	}

	//파괴되었을때 이미지
	int desX{};
	int desY{};
	if (swscanf(_destroyedIndexBuffer, L"%d,%d",
		&desX, &desY) == 2)
	{
		//currentTileInfo.destroyedIndex = IntVector2(desX, desY);
	}
	else
	{
		success = false;
	}


	//현재 타일의 주변에 마스크를 칠 할수 있는지 없는지 체크
	uint32 thisBitValue = 0;
	if (wcslen(_thisMaskBuffer) == 4)
	{
		WCHAR *pointer = _thisMaskBuffer;
		while (*pointer)
		{
			if (*pointer != L'0' && *pointer != L'1')
			{
				_stprintf(_nearMaskBuffer, L"");
				success = false;
				break;
			}
			pointer++;
		}
		for (int i = 0; i < 4; ++i)
		{
			uint32 temp = _thisMaskBuffer[i] - 48;
			thisBitValue |= (temp << (3 - i));
		}
	}
	else
	{
		success = false;
	}

	//주변에 마스크를 칠 할 수 있는지 없는지 체크
	uint32 nearBitValue = 0;
	if (wcslen(_nearMaskBuffer) == 4)
	{
		WCHAR *pointer = _nearMaskBuffer;
		//값 테스트 체크
		while (*pointer)
		{
			if (*pointer != L'0' && *pointer != L'1')
			{
				_stprintf(_nearMaskBuffer, L"");
				success = false;
				break;
			}
			pointer++;
		}
		for (int i = 0; i < 4; ++i)
		{
			uint32 temp = _nearMaskBuffer[i] - 48;
			nearBitValue |= (temp << (3 - i));
		}
	}
	else
	{
		success = false;
	}

	//콜리젼 타입 체크
	int colType{};
	if (swscanf(_collisionTypeBuffer, L"%d", &colType) == 1)
	{
		//currentTileInfo.collisionType = (TileCollisionType)colType;
	}
	else
	{
		success = false;
	}

	//레이어 순서
	int lay{};
	if (swscanf(_layerBuffer, L"%d", &lay) == 1)
	{
		//currentTileInfo.layer = (ObjectLayer)lay;
	}
	else
	{
		success = false;
	}

	//여기서 타일 인포에 대한 값을 최종 최신화를 한다.
	if (success)
	{
		_editingTileImageInfo.applied[_selectorRect.x + 8 * _selectorRect.y] = true;
		currentTileInfo.canBeDestroyedByBomb = boomDes;
		currentTileInfo.destroyedIndex = IntVector2(desX, desY);
		currentTileInfo.thisMaskInfo = thisBitValue;
		currentTileInfo.nearMaskInfo = nearBitValue;
		currentTileInfo.collisionType = (TileCollisionType)(colType);
		currentTileInfo.layer = (ObjectLayer)(lay);

	}

	_stprintf(_boomDestroyBuffer, L"");
	_stprintf(_destroyedIndexBuffer, L"");
	_stprintf(_thisMaskBuffer, L"");
	_stprintf(_nearMaskBuffer, L"");
	_stprintf(_collisionTypeBuffer, L"");
	_stprintf(_layerBuffer, L"");

	return success;
}

void MapToolScene::WriteTileInfoChunkForMap(FileUtils::File &file, const TileInfo *infos, int xCount, int yCount)
{
	for (int y = 0; y < yCount; ++y)
	{
		for (int x = 0; x < xCount; ++x)
		{
			int index = x + xCount * y;
			const TileInfo &currentTileInfo = infos[index];
			file.Write(L"At X : %d, Y : %d\n", x, y);
			file.Write(L"Image Key : %s\n", currentTileInfo.imageKey.empty() ? L"e" : currentTileInfo.imageKey.c_str());
			file.Write(L"SourceIndex X : %d, Y : %d\n", currentTileInfo.sourceIndex.x, currentTileInfo.sourceIndex.y);
			file.Write(L"Destroyed Index X : %d, Y : %d\n",
				currentTileInfo.destroyedIndex.x, currentTileInfo.destroyedIndex.y);
			file.Write(L"This Mask Info : %u\n", currentTileInfo.thisMaskInfo);
			file.Write(L"Near Mask Info : %u\n", currentTileInfo.nearMaskInfo);
			file.Write(L"Collision Type : %d\n", (int)currentTileInfo.collisionType);
			file.Write(L"Mask Info : %u\n", currentTileInfo.maskInfo);
			file.Write(L"Layer : %d\n", currentTileInfo.layer);
		}
	}
}

void MapToolScene::ReadTileInfoChunkForMap(FileUtils::File & file, TileInfo * infos, int xCount, int yCount)
{
	WCHAR buffer[40];

	for (int y = 0; y < yCount; ++y)
	{
		for (int x = 0; x < xCount; ++x)
		{

			int index = x + xCount * y;
			TileInfo &currentTileInfo = infos[index];

			std::wstring str = file.GetLine();
			file.Read(L"Image Key : %s\n", &buffer);
			file.Read(L"SourceIndex X : %d, Y : %d\n", &currentTileInfo.sourceIndex.x, &currentTileInfo.sourceIndex.y);
			file.Read(L"Destroyed Index X : %d, Y : %d\n",
				&currentTileInfo.destroyedIndex.x, &currentTileInfo.destroyedIndex.y);
			file.Read(L"This Mask Info : %u\n", &currentTileInfo.thisMaskInfo);
			file.Read(L"Near Mask Info : %u\n", &currentTileInfo.nearMaskInfo);
			file.Read(L"Collision Type : %d\n", &currentTileInfo.collisionType);
			file.Read(L"Mask Info : %u\n", &currentTileInfo.maskInfo);
			file.Read(L"Layer : %d\n", &currentTileInfo.layer);

			if (wcscmp(buffer, L"e") == 0)
			{
				currentTileInfo.imageKey.clear();
			}
			else
			{
				currentTileInfo.imageKey = buffer;
				CheckUsingImageExistence(currentTileInfo.imageKey);
			}

		}
	}
}
