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

	_editingTileSet = new TileSet<IntVector2>(ROOM_TILE_COUNTX, ROOM_TILE_COUNTY, IntVector2());

	_gridSelectorSprite = new D2DSprite;


	_loadTextWidth = _dWrite.CalculateInputTextWidth(_load);
	_saveTextWidth = _dWrite.CalculateInputTextWidth(_save);

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

}

void MapToolScene::Render()
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(_sceneClearColor);


	IM::BeginWindow(20, 20, 552, WINSIZEY - 40, L"Window");

	//if (IM::GridSelector(GEN_ID, 20, 20, 512, 512, 64, 64, _xSelecting, _ySelecting))
	//{
	//	Console::Log("x : %d, y : %d\n", _xSelecting, _ySelecting);
	//}
	if (IM::ImageGridSelector(GEN_ID, 20, 20, 512, 512, 64, 64, _xSelecting, _ySelecting, _gridSelectorSprite))
	{
		Console::Log("x : %d, y : %d\n", _xSelecting, _ySelecting);
	}

	IM::TextBox(GEN_ID, 20, 600, 400, _loadNameBuffer);
	if (IM::Button(GEN_ID, 440, 600, _loadTextWidth, _load))
	{
		LoadButtonAction();
	}

	//IM::TextBox(GEN_ID, 30, 530, _buffer);
	IM::EndWindow();

	IM::BeginWindow(580, 20, 686, WINSIZEY - 40, L"Tile");
	IM::EndWindow();

	IM::IMGUIFinish();


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
