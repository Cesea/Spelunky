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

	IM::WindowStruct windows[MAX_NUM_WINDOW];
	int freeWindowTracker{};
};

	void IMGUIPrepare();
	void IMGUIFinish();

	int RegionHit(int x, int y, int w, int h);

	void BeginWindow(int x, int y, int w, int h, const std::wstring &name);
	void EndWindow();


	void Label(int id, int x, int y);
	int Button(int id, int x, int y, const WCHAR *name);
	int Slider(int id, int x, int y, int max, int &value);
	int TextField(int id, int x, int y, int w, int h, WCHAR *buffer);



	void DrawGUIS();

}


#endif