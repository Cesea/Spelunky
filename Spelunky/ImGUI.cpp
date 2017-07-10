#include "stdafx.h"
#include "ImGUI.h"

namespace IM 
{
UIState uistate;

	void IMGUIPrepare()
	{
		uistate.hotItem = 0;
	}

	void IMGUIFinish()
	{
		if (uistate.mouseDown == 0)
		{
			uistate.activeItem = 0;
		}
		else
		{
			if (uistate.activeItem == 0)
			{
				uistate.activeItem = -1;
			}
		}

		if (uistate.keyEntered == VK_TAB)
		{
			uistate.kbdItem = 0;
		}
		uistate.keyEntered = 0;
		uistate.keyChar = 0;
	}

	int RegionHit(int x, int y, int w, int h)
	{
		if (uistate.mouseX >= x&&
			uistate.mouseY >= y&&
			uistate.mouseX <= x + w&&
			uistate.mouseY <= y + h)
		{
			return 1;
		}
		return 0;
	}

	void BeginWindow(int x, int y, int w, int h, const std::wstring &name)
	{
		FillBox(gRenderTarget, x, y, w, 30, D2D1::ColorF(0.2, 0.3, 0.5, 0.8));
		_dWrite.PrintText(gRenderTarget, x, y, w, 30, name.c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
		FillBox(gRenderTarget, x, y, w, h, D2D1::ColorF(0.5, 0.5, 0.7, 0.3));
		//uistate.windows[uistate.freeWindowTracker].x = x;
		//uistate.windows[uistate.freeWindowTracker].y = y;
		//uistate.windows[uistate.freeWindowTracker].width = w;
		//uistate.windows[uistate.freeWindowTracker].height = h;
		//uistate.windows[uistate.freeWindowTracker].name = name;
		//uistate.windows[uistate.freeWindowTracker].hasTab = false;
		//uistate.windows[uistate.freeWindowTracker].tabCount = 0;
		//uistate.windows[uistate.freeWindowTracker].tabNames = {};

		//uistate.freeWindowTracker++;

	}


	void EndWindow()
	{
	}

	void Label(int id, int x, int y)
	{
	}

	//Button imgui widget
	int Button(int id, int x, int y, const WCHAR *name)
	{
		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, x - 4, y - 2, 78, 34, D2D1::ColorF(1.0f, 0.0f, 0.0f, 0.5f));
		}
		if (RegionHit(x, y, 70, 30))
		{
			uistate.hotItem = id;
			if (uistate.activeItem == 0 &&
				uistate.mouseDown)
			{
				uistate.activeItem = id;
			}
		}
		if (uistate.hotItem == id)
		{
			if (uistate.activeItem == id)
			{
				// Button is both 'hot' and 'active'
				FillBox(gRenderTarget, x, y, 70, 30, D2D1::ColorF(0.3f, 0.5f, 0.7f, 1.0f));
			}
			else
			{
				// button is not hot, but it may be active    
				FillBox(gRenderTarget, x, y, 70, 30, D2D1::ColorF(0.2f, 0.3f, 0.5f, 1.0f));
			}
		}
		else
		{
			FillBox(gRenderTarget, x, y, 70, 30, D2D1::ColorF(0.1, 0.2, 0.3, 1.0f));
		}

		_dWrite.PrintText(gRenderTarget, x, y, 70, 30, name, D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));

		if (uistate.kbdItem == id)
		{
			switch (uistate.keyEntered)
			{
			case VK_TAB:
			{
				uistate.kbdItem = 0;
				if (KEYMANAGER->IsStayKeyDown(VK_SHIFT))
				{
					uistate.kbdItem = uistate.lastWidget;
				}
				uistate.keyEntered = 0;
			}break;
			case VK_RETURN:
			{
				return 1;
			}break;
			}
		}
		uistate.lastWidget = id;


		if (uistate.mouseDown == 0 &&
			uistate.activeItem == id &&
			uistate.hotItem == id)
		{
			return 1;
		}
		return 0;
	}

	int Slider(int id, int x, int y, int max, int & value)
	{
		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, x - 6, y - 6, 44, 278, D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
		}

		// Calculate mouse cursor's relative y offset
		int yPos = ((256 - 16) * value) / max;

		// Check for hotness
		if (RegionHit(x, y, 32, 255))
		{
			uistate.hotItem = id;
			if (uistate.activeItem == 0 && uistate.mouseDown)
				uistate.activeItem = id;
		}

		// Render the scrollbar
		FillBox(gRenderTarget, x, y, 32, 255, D2D1::ColorF(0.5, 0.5, 0.5, 1.0f));

		if (uistate.activeItem == id || uistate.hotItem == id)
		{
			FillBox(gRenderTarget, x, y + yPos, 32, 16, D2D1::ColorF(1.0, 1.0, 1.0, 1.0f));
		}
		else
		{
			FillBox(gRenderTarget, x, y + yPos, 32, 16, D2D1::ColorF(0.7, 0.7, 0.7, 1.0f));
		}

		if (uistate.kbdItem == id)
		{
			switch (uistate.keyEntered)
			{
			case VK_TAB:
			{
				uistate.kbdItem = 0;
				if (KEYMANAGER->IsStayKeyDown(VK_SHIFT))
				{
					uistate.kbdItem = uistate.lastWidget;
				}
				uistate.keyEntered = 0;
			}break;
			case VK_UP:
			{
				if (value > 0)
				{
					value--;
					return 1;
				}
			}break;
			case VK_DOWN:
			{
				if (value < max)
				{
					value++;
					return 1;
				}
			}break;
			}
		}
		uistate.lastWidget = id;

		// Update widget value
		if (uistate.activeItem == id)
		{
			int mousepos = uistate.mouseY - (y + 8);
			if (mousepos < 0) mousepos = 0;
			if (mousepos > 255) mousepos = 255;
			int v = (mousepos * max) / 255;
			if (v != value)
			{
				value = v;
				return 1;
			}
		}

		return 0;
	}

	int TextField(int id, int x, int y, int w, int h, WCHAR *buffer)
	{
		int length = wcslen(buffer);
		int changed = 0;

		if (RegionHit(x, y, 30 * 14, 24))
		{
			uistate.hotItem = id;
			if (uistate.activeItem == 0 && uistate.mouseDown)
				uistate.activeItem = id;
		}
		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, x - 6, y - 6, 30 * 14 + 12, 36, D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if (uistate.activeItem == id || uistate.hotItem == id)
		{
			FillBox(gRenderTarget, x - 3, y - 3, 30 * 14 + 6, 33, D2D1::ColorF(0.7, 0.7, 0.7, 1.0f));
		}
		else
		{
			FillBox(gRenderTarget, x - 3, y - 3, 30 * 14 + 6, 33, D2D1::ColorF(0.4, 0.4, 0.4, 1.0f));
		}

		_dWrite.PrintText(gRenderTarget, x, y, 420, 24, buffer, D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
		if (uistate.kbdItem == id)
		{
			switch (uistate.keyEntered)
			{
			case VK_TAB:
			{
				uistate.kbdItem = 0;
				if (KEYMANAGER->IsStayKeyDown(VK_SHIFT))
				{
					uistate.kbdItem = uistate.lastWidget;
				}
				uistate.keyEntered = 0;
			}break;
			case VK_BACK:
			{
				if (length > 0)
				{
					buffer[length - 1] = 0;
					changed = 1;
				}
			}break;
			}

			if (uistate.keyChar > 32 && uistate.keyChar < 127 && length < 30)
			{
				buffer[length] = uistate.keyChar;
				length++;
				buffer[length] = 0;
				changed = 1;
			}
		}
		if (uistate.mouseDown == 0 &&
			uistate.hotItem == id &&
			uistate.activeItem == id)
		{
			uistate.kbdItem = id;
		}

		uistate.lastWidget = id;
		return changed;
	}

	void DrawGUIS()
	{
		for (int i = 0; i < uistate.freeWindowTracker; ++i)
		{
			WindowStruct &windowRef = uistate.windows[i];
			FillBox(gRenderTarget, windowRef.x, windowRef.y, windowRef.width, 30, D2D1::ColorF(0.2, 0.3, 0.5, 0.8));
			_dWrite.PrintText(gRenderTarget, windowRef.x + 10, windowRef.y + 2, windowRef.width, 30, windowRef.name.c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
			FillBox(gRenderTarget, windowRef.x, windowRef.y, windowRef.width, windowRef.height, D2D1::ColorF(0.5, 0.5, 0.7, 0.3));
		}
	}

}
