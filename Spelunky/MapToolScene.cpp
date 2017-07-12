#include "stdafx.h"
#include "MapToolScene.h"

MapToolScene::MapToolScene()
{
}

MapToolScene::~MapToolScene()
{
}

HRESULT MapToolScene::LoadContent()
{
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\alltiles.png", L"alltiles");
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\testImage.png", L"testImage");

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

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

	_editingTileSet = new TileSet<TileInfo>(ROOM_TILE_COUNTX, ROOM_TILE_COUNTY, TileInfo(L"", IntVector2(-1, -1)));

	_gridSelectorSprite = new D2DSprite;

	_loadImageTextWidth = _dWrite.CalculateInputTextWidth(_loadImageText);
	_saveTextWidth = _dWrite.CalculateInputTextWidth(_save);
	_saveMapTextWidth = _dWrite.CalculateInputTextWidth(_saveMapText);
	_loadMapTextWidth = _dWrite.CalculateInputTextWidth(_loadMapText);
	_drawTextWidth = _dWrite.CalculateInputTextWidth(_drawText);
	_eraseTextWidth = _dWrite.CalculateInputTextWidth(_eraseText);
	_maskTextWidth = _dWrite.CalculateInputTextWidth(_maskText);

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

	//Console::Log("lastX : %d, lastY : %d, dragWidth : %d, dragHeight : %d\n",
	//	IM::uistate.lastLeftMouseX, IM::uistate.lastLeftMouseY, IM::uistate.leftDragWidth, IM::uistate.leftDragHeight);
}

void MapToolScene::Render()
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(_sceneClearColor);

	//Begin of First Window
	IM::BeginWindow(20, 20, 552, WINSIZEY - 40, L"Window");
	IM::ImageGridSelector(GEN_ID, 20, 20, 512, 512, 64, 64, _selectorRect, _gridSelectorSprite);
	//if (IM::ImageGridSelector(GEN_ID, 20, 20, 512, 512, 64, 64, _xSelector, _ySelector, _gridSelectorSprite))

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
		PainterAction();
	}

	//Draw Button
	if (IM::Button(GEN_ID, 20, 560, _drawTextWidth, _drawText))
	{
		_drawingMode = DrawingMode::Draw;
	}
	//Erase Button
	if (IM::Button(GEN_ID, 120, 560, _eraseTextWidth, _eraseText))
	{
		_drawingMode = DrawingMode::Erase;
	}
	if (IM::Button(GEN_ID, 240, 560, _maskTextWidth, _maskText))
	{
		CalculateBitMask(*_editingTileSet);
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
				if (_editingTileSet->GetValue(x, y))
				{
					const auto & infoRef = _editingTileSet->AtInfo(x, y);
					const IntVector2 &tileIndex = _editingTileSet->AtInfo(x, y).sourceIndex;
					const std::wstring &tileString = _editingTileSet->AtInfo(x, y).name;
					auto &found = _usingImages.find(tileString);
					found->second->FrameRender(gRenderTarget, 600 + x * TILE_SIZE, 70 + y * TILE_SIZE, tileIndex.x, tileIndex.y);
					RenderMasks(x, y, infoRef.maskInfo, found->second);
				}
			}
		}
	}
	//그린 후에는 항상 EndDraw()

	if (IM::uistate.editorOn)
	{
		if (IM::uistate.mouseRightRelease == true)
		{
			IM::uistate.editorOn = false;
		}
		else
		{
			IM::BeginPropertyWindow(IM::uistate.lastRightMouseX, IM::uistate.lastRightMouseY, 300, 300, _propertyEditorText);
			if (IM::Button(GEN_ID, 20, 100, 80, L"HI"))
			{
				int a = 0;
			}
			IM::TextBox(GEN_ID, 20, 150, 150, _propertyEditBuffer);
			IM::EndWindow();
		}
	}
	IM::IMGUIFinish();
	gRenderTarget->EndDraw();
}

void MapToolScene::DoGUIS()
{
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
			_editingTileSet->SetInfo(_xPainter, _yPainter, TileInfo(imageName, IntVector2(_selectorRect.x, _selectorRect.y)) );
			_editingTileSet->SetValue(_xPainter, _yPainter, 1);

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
			_editingTileSet->SetValue(_xPainter, _yPainter, 0);
			_editingTileSet->AtInfo(_xPainter, _yPainter).maskInfo = 0;
			_editingTileSet->AtInfo(_xPainter, _yPainter).name.clear();
			_editingTileSet->AtInfo(_xPainter, _yPainter).sourceIndex = IntVector2(-1, -1);
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
	saveFile.Open(filePath, FileUtils::FileAccess::Write);

	WCHAR wBuffer[40];
	mbstowcs(wBuffer, filePath, 39);

	saveFile.Write(L"--%s--\n\n", wBuffer);

	char textureName[32];

	for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	{
		for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
		{
			saveFile.Write(L"texture name : %s\n", _editingTileSet->AtInfo(x, y).name.c_str());
			saveFile.Write(L"%d, %d\n\n", 
				_editingTileSet->AtInfo(x, y).sourceIndex.x, _editingTileSet->AtInfo(x, y).sourceIndex.y);
		}
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
	loadFile.Open(filePath, FileUtils::FileAccess::Read);

	loadFile.GetLine();
	loadFile.GetLine();

	WCHAR buffer[32];
	for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	{
		for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
		{
			loadFile.Read(L"texture name : %s\n", buffer);
			if (buffer[0] == L'-')
			{
				loadFile.GetLine();
				continue;
			}
			_editingTileSet->AtInfo(x, y).name = buffer;
			CheckUsingImageExistence(buffer);

			int tempX{};
			int tempY{};
			loadFile.Read(L"%d, %d\n", &tempX, &tempY);
			_editingTileSet->AtInfo(x, y).sourceIndex.x = tempX;
			_editingTileSet->AtInfo(x, y).sourceIndex.y = tempY;
			if (tempX != -1)
			{
				_editingTileSet->SetValue(x, y, 1);
			}
			loadFile.GetLine();
		}
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

void MapToolScene::CalculateBitMask(TileSet<TileInfo>& tileSet)
{
	for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	{
		for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
		{
			if (tileSet.GetValue(x, y))
			{
				//위에 타일이 있는지 검사
				int upperY = y - 1;
				if (upperY >= 0)
				{
					if (!tileSet.GetValue(x, upperY))
					{
						tileSet.AtInfo(x, y).maskInfo |= (1 << 0);
					}
				}
				//왼쪽 타일이 있는지 검사
				int leftX = x - 1;
				if (leftX >= 0)
				{
					if (!tileSet.GetValue(leftX, y))
					{
						tileSet.AtInfo(x, y).maskInfo |= (1 << 1);
					}
				}
				//오른 타일이 있는지 검사
				int rightX = x + 1;
				if (rightX < ROOM_TILE_COUNTX)
				{
					if (!tileSet.GetValue(rightX, y))
					{
						tileSet.AtInfo(x, y).maskInfo |= (1 << 2);
					}
				}
				//아래 타일이 있는지 검사
				int lowerY = y + 1;
				if (lowerY < ROOM_TILE_COUNTY)
				{
					if (!tileSet.GetValue(x, lowerY))
					{
						tileSet.AtInfo(x, y).maskInfo |= (1 << 3);
					}
				}
			}
		}
	}
}

void MapToolScene::RenderMasks(int drawXIndex, int drawYIndex, const uint16 maskInfo, D2DSprite *image)
{
	//위쪽에 마스크해야하면...
	if (maskInfo & (1 << 0))
	{
		image->FrameRender(gRenderTarget, 600 + drawXIndex * TILE_SIZE, 70 + drawYIndex * TILE_SIZE - TILE_SIZE_HALF, 5, 0);
	}
	//왼쪽에 마스크해야하면...
	if (maskInfo & (1 << 1))
	{
		image->FrameRender(gRenderTarget, 600 + drawXIndex * TILE_SIZE - TILE_SIZE_HALF, 70 + drawYIndex * TILE_SIZE, 7, 2);
	}
	//오른쪽에 마스크해야하면...
	if (maskInfo & (1 << 2))
	{
		image->FrameRender(gRenderTarget, 600 + drawXIndex * TILE_SIZE + TILE_SIZE_HALF, 70 + drawYIndex * TILE_SIZE, 7, 1);
	}
	//아래쪽에 마스크해야하면...
	if (maskInfo & (1 << 3))
	{
		image->FrameRender(gRenderTarget, 600 + drawXIndex * TILE_SIZE, 70 + drawYIndex * TILE_SIZE + TILE_SIZE_HALF, 5, 1);
	}
}
