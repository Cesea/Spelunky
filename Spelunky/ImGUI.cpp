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
		uistate.lastWindowPos.emplace_back(IntVector2(x, y));
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
		uistate.lastWindowPos.pop_back();
	}

	void Label(int id, int x, int y, const WCHAR *name)
	{
		int lastX = uistate.lastWindowPos.back().x;
		int lastY = uistate.lastWindowPos.back().y;

		FillBox(gRenderTarget, lastX + x, lastY + y, 70, 30, BUTTON_COLOR);
		_dWrite.PrintText(gRenderTarget, lastX + x, lastY + y, 70, 30, name, TEXT_COLOR);
	}

	//Button imgui widget
	int Button(int id, int x, int y, const WCHAR *name)
	{
		int lastX = uistate.lastWindowPos.back().x;
		int lastY = uistate.lastWindowPos.back().y;

		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, lastX + x - OFFSET, lastY + y - OFFSET, 70 + OFFSET * 2, 30 + OFFSET * 2, HOT_COLOR);
		}
		if (RegionHit(lastX + x, lastY + y, 70, 30))
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
				FillBox(gRenderTarget, lastX + x, lastY + y, 70, 30, BUTTON_ACTIVE_COLOR);
			}
			else
			{
				// button is not hot, but it may be active    
				FillBox(gRenderTarget, lastX + x, lastY + y, 70, 30, BUTTON_HOT_COLOR);
			}
		}
		else
		{
			FillBox(gRenderTarget, lastX + x, lastY + y, 70, 30, BUTTON_COLOR);
		}

		_dWrite.PrintText(gRenderTarget, lastX + x, lastY + y, 70, 30, name, TEXT_COLOR);

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

#pragma region SLIDERS
	int VertIntSlider(int id, int x, int y, int h, int max, int & value)
	{
		int lastX = uistate.lastWindowPos.back().x + x;
		int lastY = uistate.lastWindowPos.back().y + y;
		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, lastX - OFFSET, lastY - OFFSET, SLIDER_SIZE + OFFSET * 2, h + OFFSET * 2 + SLIDER_SIZE, HOT_COLOR);
		}

		int yPos = (int)(((float)value / (float)max) * h);

		if (RegionHit(lastX, lastY, SLIDER_SIZE, h + SLIDER_SIZE))
		{
			uistate.hotItem = id;
			if (uistate.activeItem == 0 && uistate.mouseDown)
				uistate.activeItem = id;
		}
// Render the scrollbar
		FillBox(gRenderTarget, lastX, lastY, SLIDER_SIZE, h + SLIDER_SIZE, SLIDER_COLOR);

		if (uistate.activeItem == id || uistate.hotItem == id)
		{
			FillBox(gRenderTarget, lastX, lastY + yPos, SLIDER_SIZE, SLIDER_SIZE, WHITE);
		}
		else
		{
			FillBox(gRenderTarget, lastX, lastY + yPos, SLIDER_SIZE, SLIDER_SIZE, LGRAY);
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
			int mousepos = uistate.mouseY - lastY;
			if (mousepos < 0) mousepos = 0;
			if (mousepos > h) mousepos = h;
			int v = ((float)mousepos / (float)h) * max;
			if (v != value)
			{
				value = v;
				return 1;
			}
		}
		return 0;
	}
	int VertFloatSlider(int id, int x, int y, int h, float max, float & value)
	{
		int lastX = uistate.lastWindowPos.back().x + x;
		int lastY = uistate.lastWindowPos.back().y + y;
		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, lastX - OFFSET, lastY - OFFSET, SLIDER_SIZE + OFFSET + 2, h + OFFSET * 2 + SLIDER_SIZE, HOT_COLOR);
		}

		float yPos = (int)(((float)value / (float)max) * h);

		if (RegionHit(lastX, lastY, SLIDER_SIZE, h + SLIDER_SIZE))
		{
			uistate.hotItem = id;
			if (uistate.activeItem == 0 && uistate.mouseDown)
				uistate.activeItem = id;
		}
		// Render the scrollbar
		FillBox(gRenderTarget, lastX, lastY, SLIDER_SIZE, h + SLIDER_SIZE, SLIDER_COLOR);

		if (uistate.activeItem == id || uistate.hotItem == id)
		{
			FillBox(gRenderTarget, lastX, lastY + yPos, SLIDER_SIZE, SLIDER_SIZE, WHITE);
		}
		else
		{
			FillBox(gRenderTarget, lastX, lastY + yPos, SLIDER_SIZE, SLIDER_SIZE, LGRAY);
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
					value -= (1.0f / (max * 2));
					ClampFloat(&value, 0, max);
					return 1;
				}
			}break;
			case VK_DOWN:
			{
				if (value < max)
				{
					value += (1.0f / (max * 2));
					ClampFloat(&value, 0, max);
					return 1;
				}
			}break;
			}
		}
		uistate.lastWidget = id;
		// Update widget value
		if (uistate.activeItem == id)
		{
			int mousepos = uistate.mouseY - lastY;
			if (mousepos < 0) mousepos = 0;
			if (mousepos > h) mousepos = h;
			float v = ((float)mousepos / (float)h) * max;
			if(v != value)
			{
				value = v;
				return 1;
			}
		}
		return 0;
	}
	int HoriIntSlider(int id, int x, int y, int w, int max, int & value)
	{
		int lastX = uistate.lastWindowPos.back().x + x;
		int lastY = uistate.lastWindowPos.back().y + y;
		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, lastX - OFFSET, lastY - OFFSET, w + OFFSET * 2 + SLIDER_SIZE, SLIDER_SIZE + OFFSET * 2, HOT_COLOR);
		}

		int xPos = (int)(((float)value / (float)max) * w);

		if (RegionHit(lastX, lastY, w + SLIDER_SIZE, SLIDER_SIZE))
		{
			uistate.hotItem = id;
			if (uistate.activeItem == 0 && uistate.mouseDown)
				uistate.activeItem = id;
		}
		// Render the scrollbar
		FillBox(gRenderTarget, lastX, lastY, w + SLIDER_SIZE, SLIDER_SIZE, SLIDER_COLOR);

		if (uistate.activeItem == id || uistate.hotItem == id)
		{
			FillBox(gRenderTarget, lastX + xPos, lastY, SLIDER_SIZE, SLIDER_SIZE, WHITE);
		}
		else
		{
			FillBox(gRenderTarget, lastX + xPos, lastY, SLIDER_SIZE, SLIDER_SIZE, LGRAY);
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
			case VK_LEFT:
			{
				if (value > 0)
				{
					value--;
					return 1;
				}
			}break;
			case VK_RIGHT:
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
			int mousepos = uistate.mouseX - lastX;
			if (mousepos < 0) mousepos = 0;
			if (mousepos > w) mousepos = w;
			int v = ((float)mousepos / (float)w) * max;
			if (v != value)
			{
				value = v;
				return 1;
			}
		}
		return 0;
	}
	int HoriFloatSlider(int id, int x, int y, int w, float max, float & value)
	{
		int lastX = uistate.lastWindowPos.back().x + x;
		int lastY = uistate.lastWindowPos.back().y + y;
		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, lastX - OFFSET, lastY - OFFSET, w + OFFSET * 2 + SLIDER_SIZE, SLIDER_SIZE + OFFSET * 2, HOT_COLOR);
		}

		float xPos = (((float)value / (float)max) * (float)w);

		if (RegionHit(lastX, lastY, w + SLIDER_SIZE, SLIDER_SIZE))
		{
			uistate.hotItem = id;
			if (uistate.activeItem == 0 && uistate.mouseDown)
				uistate.activeItem = id;
		}
		// Render the scrollbar
		FillBox(gRenderTarget, lastX, lastY, w + SLIDER_SIZE, SLIDER_SIZE, SLIDER_COLOR);

		if (uistate.activeItem == id || uistate.hotItem == id)
		{
			FillBox(gRenderTarget, lastX + xPos, lastY, SLIDER_SIZE, SLIDER_SIZE, WHITE);
		}
		else
		{
			FillBox(gRenderTarget, lastX + xPos, lastY, SLIDER_SIZE, SLIDER_SIZE, LGRAY);
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
			case VK_LEFT:
			{
				if (value > 0)
				{
					value -= (1.0f / (max * 2));
					ClampFloat(&value, 0, max);
					return 1;
				}
			}break;
			case VK_RIGHT:
			{
				if (value < max)
				{
					value += (1.0f / (max * 2));
					ClampFloat(&value, 0, max);
					return 1;
				}
			}break;
			}
		}
		uistate.lastWidget = id;
		// Update widget value
		if (uistate.activeItem == id)
		{
			int mousepos = uistate.mouseX - lastX;
			if (mousepos < 0) mousepos = 0;
			if (mousepos > w) mousepos = w;
			float v = ((float)mousepos / (float)w) * max;
			if (v != value)
			{
				value = v;
				return 1;
			}
		}
		return 0;
	}
#pragma endregion



	int TextBox(int id, int x, int y, WCHAR *buffer)
	{
		int currentX = uistate.lastWindowPos.back().x + x;
		int currentY = uistate.lastWindowPos.back().y + y;

		int length = wcslen(buffer);
		int changed = 0;

		if (RegionHit(currentX, currentY, TEXTBOX_WIDTH, TEXTBOX_HEIGHT))
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
			FillBox(gRenderTarget, currentX - OFFSET, currentY - OFFSET, TEXTBOX_WIDTH + OFFSET * 2, TEXTBOX_HEIGHT + OFFSET * 2, HOT_COLOR);
		}

		if (uistate.activeItem == id || uistate.hotItem == id)
		{
			FillBox(gRenderTarget, currentX, currentY, TEXTBOX_WIDTH, TEXTBOX_HEIGHT, D2D1::ColorF(0.7, 0.7, 0.7, 1.0f));
		}
		else
		{
			FillBox(gRenderTarget, currentX, currentY, TEXTBOX_WIDTH, TEXTBOX_HEIGHT, TEXTBOX_COLOR);
		}

		_dWrite.AlignFont(ALIGN_LEFT);
		_dWrite.PrintText(gRenderTarget, currentX + OFFSET, currentY + OFFSET, TEXTBOX_WIDTH, TEXTBOX_HEIGHT, buffer, D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
		_dWrite.AlignFont(ALIGN_CENTER);
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
		//for (int i = 0; i < uistate.freeWindowTracker; ++i)
		//{
		//	WindowStruct &windowRef = uistate.windows[i];
		//	FillBox(gRenderTarget, windowRef.x, windowRef.y, windowRef.width, 30, D2D1::ColorF(0.2, 0.3, 0.5, 0.8));
		//	_dWrite.PrintText(gRenderTarget, windowRef.x + 10, windowRef.y + 2, windowRef.width, 30, windowRef.name.c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
		//	FillBox(gRenderTarget, windowRef.x, windowRef.y, windowRef.width, windowRef.height, D2D1::ColorF(0.5, 0.5, 0.7, 0.3));
		//}
	}

}
