#ifndef IM_GUI_H
#define IM_GUI_H

#ifdef IMGUI_SRC_ID
#define GEN_ID ((IMGUI_SRC_ID) + (__LINE__))
#else
#define GEN_ID ((__COUNTER__) + 1)
#endif

#define MAX_NUM_WINDOW 10

namespace IM
{
struct UIState;

extern UIState uistate;

constexpr int SLIDER_SIZE = 20;
constexpr int TEXTBOX_WIDTH = 200;
constexpr int TEXTBOX_HEIGHT = 30;
constexpr int TEXTBOX_MAX_CHAR = 30;
constexpr int OFFSET = 2;

static D2D1::ColorF RED = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
static D2D1::ColorF GREEN = D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f);
static D2D1::ColorF BLUE = D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f);
static D2D1::ColorF WHITE = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
static D2D1::ColorF BLACK = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);

static D2D1::ColorF LGRAY = D2D1::ColorF(0.7f, 0.7f, 0.7f, 1.0f);
static D2D1::ColorF DGRAY = D2D1::ColorF(0.4f, 0.4f, 0.4f, 1.0f);

static D2D1::ColorF HOT_COLOR = D2D1::ColorF(1.0f, 0.0f, 0.0f, 0.5f);

static D2D1::ColorF BUTTON_COLOR = D2D1::ColorF(0.1, 0.2, 0.3, 1.0f);
static D2D1::ColorF BUTTON_HOT_COLOR = D2D1::ColorF(0.2f, 0.3f, 0.5f, 1.0f);
static D2D1::ColorF BUTTON_ACTIVE_COLOR = D2D1::ColorF(0.3f, 0.5f, 0.7f, 1.0f);

static D2D1::ColorF SLIDER_COLOR = D2D1::ColorF(0.1, 0.2, 0.3, 1.0f);
static D2D1::ColorF SLIDER_HOT_COLOR = D2D1::ColorF(0.2f, 0.3f, 0.5f, 1.0f);
static D2D1::ColorF SLIDER_ACTIVE_COLOR = D2D1::ColorF(0.3f, 0.5f, 0.7f, 1.0f);

static D2D1::ColorF TEXTBOX_COLOR = D2D1::ColorF(0.1, 0.2, 0.3, 1.0f);
static D2D1::ColorF TEXTBOX_ACTIVE_COLOR = D2D1::ColorF(0.2f, 0.3f, 0.5f, 1.0f);

static D2D1::ColorF TEXT_COLOR = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);

//D2D1::ColorF RED = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
//D2D1::ColorF RED = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
//D2D1::ColorF RED = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);



struct WindowStruct
{
	int x;
	int y;
	int width;
	int height;
	std::wstring name;

	bool hasTab{false};
	int tabCount{ 0 };
	std::vector<std::wstring> tabNames;
};

struct UIState
{
	int mouseX{};
	int mouseY{};
	int mouseDown{};

	int kbdItem{};
	int keyEntered{};
	int keyChar{};
	int keyMod{};

	int lastWidget{};

	int hotItem{};
	int activeItem;


	std::vector<IntVector2> lastWindowPos;
	//IM::WindowStruct windows[MAX_NUM_WINDOW];
	//int freeWindowTracker{};
};

	void IMGUIPrepare();
	void IMGUIFinish();

	int RegionHit(int x, int y, int w, int h);

	void BeginWindow(int x, int y, int w, int h, const std::wstring &name);
	void EndWindow();

	void Label(int id, int x, int y, const WCHAR *name);
	int Button(int id, int x, int y, const WCHAR *name);
	int VertIntSlider(int id, int x, int y, int h, int max, int &value);
	int VertFloatSlider(int id, int x, int y, int h, float max, float &value);

	int HoriIntSlider(int id, int x, int y, int w, int max, int &value);
	int HoriFloatSlider(int id, int x, int y, int w, float max, float &value);
	int TextBox(int id, int x, int y, WCHAR *buffer);



	void DrawGUIS();

}


#endif