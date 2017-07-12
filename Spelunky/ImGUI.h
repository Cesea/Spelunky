#ifndef IM_GUI_H
#define IM_GUI_H

#ifdef IMGUI_SRC_ID
#define GEN_ID ((IMGUI_SRC_ID) + (__LINE__))
#else
#define GEN_ID ((__COUNTER__) + 1)
#endif

#include "stdafx.h"

#define MAX_NUM_WINDOW 10

class D2DSprite;

namespace IM
{
struct UIState;

extern UIState uistate;

constexpr int SLIDER_SIZE = 20;
constexpr int TEXTBOX_WIDTH = 200;
constexpr int TEXTBOX_HEIGHT = 30;
constexpr int TEXTBOX_MAX_CHAR = 30;
constexpr int OFFSET = 2;
constexpr int BUTTON_HEIGHT = 30;
constexpr int BUTTON_MARGIN = 10;

constexpr int TEXT_WIDTH = 11;

static D2D1::ColorF RED = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
static D2D1::ColorF GREEN = D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f);
static D2D1::ColorF BLUE = D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f);
static D2D1::ColorF WHITE = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
static D2D1::ColorF BLACK = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);

static D2D1::ColorF LGRAY = D2D1::ColorF(0.7f, 0.7f, 0.7f, 1.0f);
static D2D1::ColorF DGRAY = D2D1::ColorF(0.4f, 0.4f, 0.4f, 1.0f);

static D2D1::ColorF HOT_COLOR = D2D1::ColorF(1.0f, 0.0f, 0.0f, 0.5f);

static D2D1::ColorF LABEL_COLOR = D2D1::ColorF(0.1f, 0.2f, 0.3f, 1.0f);

static D2D1::ColorF BUTTON_COLOR = D2D1::ColorF(0.1, 0.2, 0.3, 1.0f);
static D2D1::ColorF BUTTON_HOT_COLOR = D2D1::ColorF(0.2f, 0.3f, 0.5f, 1.0f);
static D2D1::ColorF BUTTON_ACTIVE_COLOR = D2D1::ColorF(0.3f, 0.5f, 0.7f, 1.0f);

static D2D1::ColorF SLIDER_COLOR = D2D1::ColorF(0.1, 0.2, 0.3, 1.0f);
static D2D1::ColorF SLIDER_HOT_COLOR = D2D1::ColorF(0.2f, 0.3f, 0.5f, 1.0f);
static D2D1::ColorF SLIDER_ACTIVE_COLOR = D2D1::ColorF(0.3f, 0.5f, 0.7f, 1.0f);

static D2D1::ColorF TEXTBOX_COLOR = D2D1::ColorF(0.1, 0.2, 0.3, 1.0f);
static D2D1::ColorF TEXTBOX_ACTIVE_COLOR = D2D1::ColorF(0.2f, 0.3f, 0.5f, 1.0f);

static D2D1::ColorF TEXT_COLOR = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);

static D2D1::ColorF GRID_EMPTY_COLOR = D2D1::ColorF(0.2f, 0.3f, 0.5f, 0.4f);

//D2D1::ColorF RED = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
//D2D1::ColorF RED = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
//D2D1::ColorF RED = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);


struct UIState
{
	int mouseX{};
	int mouseY{};
	int mouseLeftDown{};
	int mouseRightDown{};
	int mouseRightRelease{};

	int kbdItem{};
	int keyEntered{};
	int keyChar{};
	int keyMod{};

	int lastWidget{};

	int lastRightMouseX{};
	int lastRightMouseY{};

	int hotItem{};
	int activeItem{};

	int editorOn{ 0 };
	int editorChild{ 0 };
	Rect editorRect{};

	std::vector<IntVector2> lastWindowPos;
};

	void IMGUIPrepare();
	void IMGUIFinish();

	int RegionHit(int x, int y, int w, int h);

	void BeginWindow(int x, int y, int w, int h, const std::wstring &name);
	void BeginPropertyWindow(int x, int y, int w, int h, const std::wstring &name);
	void EndWindow();

	int CheckEditorHit(int id, int localX, int localY, int localW, int localH);


	void ImageLabel(int id, int x, int y, const WCHAR *name, D2DSprite *sprite);

	void Label(int id, int x, int y, const WCHAR *name);
	int Button(int id, int x, int y, int w, const WCHAR *name);
	int VertIntSlider(int id, int x, int y, int h, int max, int &value);
	int VertFloatSlider(int id, int x, int y, int h, float max, float &value);

	int HoriIntSlider(int id, int x, int y, int w, int max, int &value);
	int HoriFloatSlider(int id, int x, int y, int w, float max, float &value);
	int TextBox(int id, int x, int y, int w, WCHAR *buffer);

	int GridSelector(int id, int x, int y, int totalWidth, int totalHeight,
						int frameWidth, int frameHeight, int &xIndex, int &yIndex);
	int ImageGridSelector(int id, int x, int y, int totalWidth, int totalHeight,
							int frameWidth, int frameHeight, int &xIndex, int &yIndex, D2DSprite *sprite);

	int GridPainter(int id, int x, int y, int totalWidth, int totalHeight,
						int frameWidth, int frameHeight, int &xIndex, int &yIndex);


	void DrawGUIS();

}


#endif