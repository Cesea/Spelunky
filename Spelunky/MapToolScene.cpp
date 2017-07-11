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


	_editingTileSet = new TileSet(ROOM_TILE_COUNTX, ROOM_TILE_COUNTY, 1);

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


	IM::BeginWindow(50, 50, 570, 630, L"Window");

	IM::Button(GEN_ID, 100, 570, L"Save");
	IM::Button(GEN_ID, 200, 570, L"Load");

	IM::VertIntSlider(GEN_ID, 400, 50, 200, 5, _slider1Value);
	IM::HoriFloatSlider(GEN_ID, 50, 50, 200, 5.0f, _slider2Value);

	IM::TextBox(GEN_ID, 100, 300, _buffer);


	IM::EndWindow();

	IM::IMGUIFinish();

	//IM::DrawGUIS();


	//Slider(GEN_ID, 100, 100, 200, _slider1Value);

	//if (Button(GEN_ID, 200, 50))
	//{
	//	_sceneClearColor = D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f);
	//}
	//if (Button(GEN_ID, 300, 50))
	//{
	//	_sceneClearColor = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
	//}

	//static WCHAR buffer[60]{};

	//TextField(GEN_ID, 500, 100, buffer);


	//for (int y = 0; y < ROOM_TILE_COUNTY; ++y)
	//{
	//	for (int x = 0; x < ROOM_TILE_COUNTX; ++x)
	//	{
	//		if (_editingTileSet->GetValue(x, y) == 0)
	//		{
	//			FillBox(gRenderTarget, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
	//		}
	//		else
	//		{
	//			FillBox(gRenderTarget, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f));
	//		}
	//	}
	//}

	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}

