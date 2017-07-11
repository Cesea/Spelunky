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

	_loadTextWidth = _dWrite.CalculateInputTextWidth(_load);
	_saveTextWidth = _dWrite.CalculateInputTextWidth(_save);

	//_drawingSprites.reserve(15);

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
		IM::uistate.mouseDown = true;
	}
	else if (rawInput.mouse.leftButton.released)
	{
		IM::uistate.mouseDown = false;
	}
	ControlCommand playerCommand = _inputMapper.InterpretRawInput(&rawInput);

	if (KEYMANAGER->IsOnceKeyDown('0'))
	{
		_paintingValue = 0;
	}
	else if (KEYMANAGER->IsOnceKeyDown('1'))
	{
		_paintingValue = 1;
	}
	else if(KEYMANAGER->IsOnceKeyDown('2'))
	{
		_paintingValue = 2;
	}
	else if (KEYMANAGER->IsOnceKeyDown('3'))
	{
		_paintingValue = 3;
	}

}

void MapToolScene::Render()
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(_sceneClearColor);

	//Begin of First Window
	IM::BeginWindow(20, 20, 552, WINSIZEY - 40, L"Window");
	//if (IM::GridSelector(GEN_ID, 20, 20, 512, 512, 64, 64, _xSelecting, _ySelecting))
	//{
	//	Console::Log("x : %d, y : %d\n", _xSelecting, _ySelecting);
	//}
	if (IM::ImageGridSelector(GEN_ID, 20, 20, 512, 512, 64, 64, _xSelector, _ySelector, _gridSelectorSprite))
	{
		//Console::Log("x : %d, y : %d\n", _xSelector, _ySelector);
	}

	IM::TextBox(GEN_ID, 20, 600, 400, _loadNameBuffer);
	if (IM::Button(GEN_ID, 440, 600, _loadTextWidth, _load))
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
		//Console::Log("x : %d, y : %d\n", _xPainter, _yPainter);
		PainterAction();
	}
	IM::EndWindow();
	IM::IMGUIFinish();
	//End of Second Window

	//이미지가 있으면
	if (_usingImages.size() != 0)
	{
		for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
		{
			for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
			{
				const IntVector2 &tileIndex = _editingTileSet->At(x, y).sourceIndex;
				if (tileIndex.x != -1)
				{
					const std::wstring &tileString = _editingTileSet->At(x, y).name;
					auto &found = _usingImages.find(tileString);
					found->second->FrameRender(gRenderTarget, 600 + x * TILE_SIZE, 70 + y * TILE_SIZE, tileIndex.x, tileIndex.y);
				}
			}
		}
	}
	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}

void MapToolScene::LoadButtonAction()
{
	//SAFE_DELETE(_gridSelectorSprite);
	std::wstring name = _loadNameBuffer;
	D2DImage *image = IMAGEMANAGER->GetImage(name);
	//load success
	if (image)
	{
		_gridSelectorSprite->Init(image, 0, 0, 512, 512, IntVector2());
	}
	//load failed
	else
	{
		_stprintf(_loadNameBuffer, L"ERROR!! No Such Sprite");
	}
}

void MapToolScene::PainterAction()
{
	if (_gridSelectorSprite->GetSourceImage())
	{
		const std::wstring &imageName = _gridSelectorSprite->GetSourceImage()->GetName();
		_editingTileSet->SetValue(_xPainter, _yPainter, TileInfo(imageName, IntVector2(_xSelector, _ySelector)));

		auto &found = _usingImages.find(imageName);
		if (found == _usingImages.end())
		{
			D2DSprite *newSprite = new D2DFrameSprite;
			newSprite->Init(_gridSelectorSprite->GetSourceImage(), 64, 64, IntVector2(0, 0));
			_usingImages.insert(std::make_pair(imageName, newSprite));
		}
	}
}
