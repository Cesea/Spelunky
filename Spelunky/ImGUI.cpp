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
		if (uistate.mouseLeftDown == 0)
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

		uistate.mouseRightRelease = 0;

		if (uistate.keyEntered == VK_TAB)
		{
			uistate.kbdItem = 0;
		}
		uistate.keyEntered = 0;
		uistate.keyChar = 0;
	}

	int RegionHit(int x, int y, int w, int h)
	{
		if (uistate.mouseX > x &&
			uistate.mouseY > y &&
			uistate.mouseX < x + w &&
			uistate.mouseY < y + h)
		{
			return 1;
		}
		return 0;
	}

	void BeginWindow(int x, int y, int w, int h, const std::wstring &name)
	{
		uistate.lastWindowPos.emplace_back(IntVector2(x, y + 30));
		FillBox(gRenderTarget, x, y, w, 30, D2D1::ColorF(0.2, 0.3, 0.5, 0.8));
		_dWrite.PrintText(gRenderTarget, x, y, w, 30, name.c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
		FillBox(gRenderTarget, x, y, w, h, D2D1::ColorF(0.5, 0.5, 0.7, 0.3));
	}

	void BeginPropertyWindow(int x, int y, int w, int h, const std::wstring & name)
	{
		uistate.lastWindowPos.emplace_back(IntVector2(x, y + 30));
		FillBox(gRenderTarget, x, y, w, h, D2D1::ColorF(0.3, 0.4, 0.6, 1.0));
		FillBox(gRenderTarget, x, y, w, 30, D2D1::ColorF(0.2, 0.3, 0.5, 0.8));
		_dWrite.PrintText(gRenderTarget, x, y, w, 30, name.c_str(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
		uistate.editorChild = true;
		uistate.editorWindowX = x;
		uistate.editorWindowY = y;
	}

	void EndWindow()
	{
		uistate.lastWindowPos.pop_back();
		uistate.editorChild = false;
	}

	int CheckEditorHit(int id, int localX, int localY, int localW, int localH)
	{
		if (uistate.editorOn && uistate.editorChild &&
			RegionHit(uistate.editorWindowX, uistate.editorWindowY, uistate.editorRect.width, uistate.editorRect.height))
		{
			//리턴을 어디서 해야할까
			if (RegionHit(localX, localY, localW, localH))
			{
				uistate.hotItem = id;
				//uistate.activeItem = 0;
				//Console::Log("this id : %d, activeItem : %d\n", id, uistate.activeItem);
				if (/*uistate.activeItem == 0 &&*/
					uistate.mouseLeftDown )
				{
					uistate.activeItem = id;
				}
			}
			return 1;
		}
		return 0;
	}

	void ImageLabel(int id, int x, int y, const WCHAR * name, D2DSprite * sprite)
	{
		int currentX = uistate.lastWindowPos.back().x + x;
		int currentY = uistate.lastWindowPos.back().y + y;

		sprite->Render(gRenderTarget, currentX, currentY);
		_dWrite.PrintText(gRenderTarget, currentX, currentY, sprite->GetWidthPerFrame(), sprite->GetHeightPerFrame(),
			name, TEXT_COLOR);
	}

	void Label(int id, int x, int y, int w, const WCHAR *name)
	{
		int currentX = uistate.lastWindowPos.back().x + x;
		int currentY = uistate.lastWindowPos.back().y + y;

		FillBox(gRenderTarget, currentX, currentY, w, LABEL_HEIGHT, LABEL_COLOR);
		_dWrite.PrintText(gRenderTarget, currentX, currentY, w, LABEL_HEIGHT, name, TEXT_COLOR);
	}

	void BorderLabel(int id, int x, int y, int w, const WCHAR * name)
	{
	}

	//Button imgui widget
	int Button(int id, int x, int y, int w, const WCHAR *name)
	{
		int currentX = uistate.lastWindowPos.back().x + x;
		int currentY = uistate.lastWindowPos.back().y + y;
		 
		w += 40;

		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, currentX - OFFSET, currentY - OFFSET, w + OFFSET * 2, BUTTON_HEIGHT + OFFSET * 2, HOT_COLOR);
		}

		if (!CheckEditorHit(id, currentX, currentY, w, BUTTON_HEIGHT))
		{
			if (RegionHit(currentX, currentY, w, BUTTON_HEIGHT))
			{
				uistate.hotItem = id;
				if (uistate.activeItem == 0 &&
					uistate.mouseLeftDown)
				{
					uistate.activeItem = id;
				}
			}
		}
		if (uistate.hotItem == id)
		{
			if (uistate.activeItem == id)
			{
				// Button is both 'hot' and 'active'
				FillBox(gRenderTarget, currentX, currentY, w, BUTTON_HEIGHT, BUTTON_ACTIVE_COLOR);
			}
			else
			{
				// button is not hot, but it may be active    
				FillBox(gRenderTarget, currentX, currentY, w, BUTTON_HEIGHT, BUTTON_HOT_COLOR);
			}
		}
		else
		{
			FillBox(gRenderTarget, currentX, currentY, w, BUTTON_HEIGHT, BUTTON_COLOR);
		}

		_dWrite.PrintText(gRenderTarget, currentX, currentY, w, BUTTON_HEIGHT, name, TEXT_COLOR);

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

		//if (uistate.editorChild)
		//{
		//Console::Log("mouseDown : %d, activeItem : %d, id : %d\n", uistate.mouseLeftDown, uistate.activeItem, id);
		//}
		if (uistate.mouseLeftDown == 0 &&
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

		if (!CheckEditorHit(id, lastX, lastY, SLIDER_SIZE, h))
		{
			if (RegionHit(lastX, lastY, SLIDER_SIZE, h + SLIDER_SIZE))
			{
				uistate.hotItem = id;
				if (uistate.activeItem == 0 && uistate.mouseLeftDown)
					uistate.activeItem = id;
			}
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

		if (!CheckEditorHit(id, lastX, lastY, SLIDER_SIZE, h))
		{
			if (RegionHit(lastX, lastY, SLIDER_SIZE, h + SLIDER_SIZE))
			{
				uistate.hotItem = id;
				if (uistate.activeItem == 0 && uistate.mouseLeftDown)
					uistate.activeItem = id;
			}
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

		if (!CheckEditorHit(id, lastX, lastY, w, SLIDER_SIZE))
		{
			if (RegionHit(lastX, lastY, w + SLIDER_SIZE, SLIDER_SIZE))
			{
				uistate.hotItem = id;
				if (uistate.activeItem == 0 && uistate.mouseLeftDown)
					uistate.activeItem = id;
			}
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

		if (!CheckEditorHit(id, lastX, lastY, w, SLIDER_SIZE))
		{
			if (RegionHit(lastX, lastY, w + SLIDER_SIZE, SLIDER_SIZE))
			{
				uistate.hotItem = id;
				if (uistate.activeItem == 0 && uistate.mouseLeftDown)
					uistate.activeItem = id;
			}
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

#pragma region TEXTBOX
	int TextBox(int id, int x, int y, int w, WCHAR *buffer)
	{
		int currentX = uistate.lastWindowPos.back().x + x;
		int currentY = uistate.lastWindowPos.back().y + y;

		int length = wcslen(buffer);
		//uistate.textTrack = length;
		int changed = 0;

		if (!CheckEditorHit(id, currentX, currentY, w, TEXTBOX_HEIGHT))
		{
			if (RegionHit(currentX, currentY, w, TEXTBOX_HEIGHT))
			{
				uistate.hotItem = id;
				if (uistate.activeItem == 0 && uistate.mouseLeftDown)
				{
					uistate.activeItem = id;
					uistate.kbdItem = id;
				}
			}
		}
		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, currentX - OFFSET, currentY - OFFSET, w + OFFSET * 2, TEXTBOX_HEIGHT + OFFSET * 2, HOT_COLOR);
		}

		if (uistate.activeItem == id || uistate.hotItem == id)
		{
			FillBox(gRenderTarget, currentX, currentY, w, TEXTBOX_HEIGHT, D2D1::ColorF(0.7, 0.7, 0.7, 1.0f));
		}
		else
		{
			FillBox(gRenderTarget, currentX, currentY, w, TEXTBOX_HEIGHT, TEXTBOX_COLOR);
		}

		_dWrite.AlignFont(ALIGN_LEFT);
		float rightMargin = _dWrite.PrintTextLayout(gRenderTarget, currentX + OFFSET, currentY + OFFSET, w, TEXTBOX_HEIGHT, buffer, D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
		_dWrite.AlignFont(ALIGN_CENTER);

		//깜빡이 그리기
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, currentX + rightMargin + 3, currentY + TEXTBOX_HEIGHT - 7, 10, 5, WHITE);
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
		if (uistate.mouseLeftDown == 0 &&
			uistate.hotItem == id &&
			uistate.activeItem == id)
		{
			uistate.kbdItem = id;
		}

		uistate.lastWidget = id;
		return changed;
	}
#pragma endregion

#pragma region GRIDS
	int GridSelector(int id, int x, int y, int totalWidth, int totalHeight,
						int frameWidth, int frameHeight, int &xIndex, int &yIndex)
	{
		int totalXIndex = totalWidth / frameWidth;
		int totalYIndex = totalHeight / frameHeight;

		int currentX = uistate.lastWindowPos.back().x + x;
		int currentY = uistate.lastWindowPos.back().y + y;

		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			FillBox(gRenderTarget, currentX - OFFSET, currentY - OFFSET, 
				totalWidth + OFFSET * 2, totalHeight + OFFSET * 2, HOT_COLOR);
		}
		if (!CheckEditorHit(id, currentX, currentY, totalWidth, totalHeight))
		{
			if (RegionHit(currentX, currentY, totalWidth, totalHeight))
			{
				if (uistate.mouseRightRelease)
				{
					uistate.editorOn = true;
				}

				uistate.hotItem = id;
				if (uistate.activeItem == 0 &&
					uistate.mouseLeftDown)
				{
					uistate.activeItem = id;
					uistate.kbdItem = id;
				}
			}
		}
		if (uistate.hotItem == id)
		{
			if (uistate.activeItem == id)
			{
				// both 'hot' and 'active'
				FillBox(gRenderTarget, currentX, currentY, totalWidth, totalHeight, BUTTON_ACTIVE_COLOR);
			}
			else
			{
				// not hot, but it may be active    
				FillBox(gRenderTarget, currentX, currentY, totalWidth, totalHeight, BUTTON_HOT_COLOR);
			}
		}
		else
		{
			FillBox(gRenderTarget, currentX,  currentY, totalWidth, totalHeight, BUTTON_COLOR);
		}

		DrawBox(gRenderTarget, currentX + xIndex * frameWidth, currentY + yIndex * frameHeight, frameWidth, frameHeight, HOT_COLOR, 2.0f);

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
			case VK_UP :
			{
				ClampInt(&--yIndex, 0, totalYIndex - 1);
				return 1;
			}break;
			case VK_DOWN :
			{
				ClampInt(&++yIndex, 0, totalYIndex - 1);
				return 1;
			}break;
			case VK_LEFT :
			{
				ClampInt(&--xIndex, 0, totalXIndex - 1);
				return 1;
			}break;
			case VK_RIGHT :
			{
				ClampInt(&++xIndex, 0, totalXIndex - 1);
				return 1;
			}break;
			case VK_RETURN:
			{
				return 1;
			}break;
			}
		}
		uistate.lastWidget = id;

		if (uistate.activeItem == id)
		{
			int mouseRelX = uistate.mouseX - currentX;
			int mouseRelY = uistate.mouseY - currentY;
			int mouseXIndex = mouseRelX / frameWidth;
			int mouseYIndex = mouseRelY / frameHeight;

			xIndex = mouseXIndex;
			yIndex = mouseYIndex;

			return 1;
		}
		return 0;
	}

	//
	int ImageGridSelector(int id, int x, int y, int totalWidth, int totalHeight, int frameWidth, int frameHeight, 
		IntRect & selectorRect, D2DSprite * sprite)
	{
		int totalXIndex = totalWidth / frameWidth;
		int totalYIndex = totalHeight / frameHeight;

		int currentX = uistate.lastWindowPos.back().x + x;
		int currentY = uistate.lastWindowPos.back().y + y;

		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			DrawBox(gRenderTarget, currentX - OFFSET * 2, currentY - OFFSET * 2,
				totalWidth + OFFSET * 4, totalHeight + OFFSET * 4, HOT_COLOR, 6.0f);
		}
		if (!CheckEditorHit(id, currentX, currentY, totalWidth, totalHeight))
		{
			if (RegionHit(currentX, currentY, totalWidth, totalHeight))
			{
				uistate.hotItem = id;
				if (uistate.activeItem == 0 &&
					uistate.mouseLeftDown || uistate.mouseRightRelease)
				{
					uistate.activeItem = id;
					uistate.kbdItem = id;
				}
			}
		}

		if (sprite->GetSourceImage() == nullptr)
		{
			FillBox(gRenderTarget, currentX, currentY, totalWidth, totalHeight, GRID_EMPTY_COLOR);
			DrawBox(gRenderTarget, currentX - OFFSET, currentY - OFFSET,
				totalWidth + OFFSET * 2, totalHeight + OFFSET * 2, BLACK, 4.0f);
		}
		else
		{
			sprite->Render(gRenderTarget, currentX, currentY);
			//Draw Grids
			for (int y = 1; y < totalXIndex; ++y)
			{
				DrawLine(gRenderTarget, currentX, currentY + y * frameHeight,
					currentX + totalWidth, currentY + y * frameHeight, BLACK, 1.5f);
			}
			for (int x = 1; x < totalXIndex; ++x)
			{
				DrawLine(gRenderTarget, currentX + x * frameWidth, currentY,
					currentX + x * frameWidth, currentY + totalHeight, BLACK, 1.5f);
			}
			DrawBox(gRenderTarget, currentX - OFFSET, currentY - OFFSET,
				totalWidth + OFFSET * 2, totalHeight + OFFSET * 2, BLACK, 4.0f);

			DrawBox(gRenderTarget, currentX + selectorRect.x * frameWidth, currentY + selectorRect.y * frameHeight,
				frameWidth * (selectorRect.width + 1), frameHeight * (selectorRect.height + 1), HOT_COLOR, 2.0f);
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
			case VK_RETURN:
			{
				return 1;
			}break;
			}
		}
		uistate.lastWidget = id;

		if (uistate.activeItem == id)
		{
			int relLastLeftX = (uistate.lastLeftMouseX - currentX) / frameWidth;
			int relLastLeftY = (uistate.lastLeftMouseY - currentY) / frameHeight;
			int relCurrentLeftX = (uistate.mouseX - currentX) / frameWidth;
			int relCurrentLeftY = (uistate.mouseY - currentY) / frameHeight;

			ClampInt(&relLastLeftX, 0, totalXIndex - 1);
			ClampInt(&relLastLeftY, 0, totalYIndex - 1);
			ClampInt(&relCurrentLeftX, 0, totalXIndex - 1);
			ClampInt(&relCurrentLeftY, 0, totalYIndex - 1);

			//셀렉터 렉트 인덱스 지정
			//만약 에디터 타일이 닫혀있을때만
			if (!uistate.editorOn)
			{
				selectorRect = IntRectMakeWidthCorners(IntVector2(relLastLeftX, relLastLeftY), 
					IntVector2(relCurrentLeftX, relCurrentLeftY));
			}

			//만약에 이 아이템이 엑티브 아이템인데 마우스 오른쪽이 false이면 에디터 온
			if (!uistate.editorOn)
			{
				if (uistate.mouseRightRelease)
				{
					selectorRect.x = (uistate.lastRightMouseX - currentX) / frameWidth;
					selectorRect.y = (uistate.lastRightMouseY - currentY) / frameHeight;
					selectorRect.width = 0;
					selectorRect.height = 0;

					uistate.editorOn = true;
					uistate.mouseRightRelease = false;
					uistate.editorRect = RectMake(uistate.lastRightMouseX, uistate.lastRightMouseY, 620, 150);

					//이 상황에서는 2를 반환하여 MapToolScene에서 알맞은 처리를 할 수 있게끔 한다.
					return 2;
				}
			}
			return 1;
		}
		return 0;
	}

	int GridPainter(int id, int x, int y, int totalWidth, int totalHeight,
						int frameWidth, int frameHeight, int &xIndex, int &yIndex)
	{
		int innerFlag = 0;

		int totalXIndex = totalWidth / frameWidth;
		int totalYIndex = totalHeight / frameHeight;

		int currentX = uistate.lastWindowPos.back().x + x;
		int currentY = uistate.lastWindowPos.back().y + y;

		if (uistate.kbdItem == 0)
		{
			uistate.kbdItem = id;
		}
		if (uistate.kbdItem == id)
		{
			DrawBox(gRenderTarget, currentX - OFFSET * 2, currentY - OFFSET * 2, 
				totalWidth + OFFSET * 4, totalHeight + OFFSET * 4, HOT_COLOR, 6.0f);
		}

		//if(!RegionHit())
		if (uistate.editorOn)
		{
			//에디터 윈도우가 켜져 있고 에디터 원도우에 충돌 되어있지 않으면...
			if (!RegionHit(uistate.editorRect.x, uistate.editorRect.y, uistate.editorRect.width, uistate.editorRect.height))
			{
				if (RegionHit(currentX, currentY, totalWidth, totalHeight))
				{
					uistate.hotItem = id;
					if (uistate.activeItem == 0 && uistate.mouseLeftDown)
					{
						uistate.activeItem = id;
						uistate.kbdItem = id;
					}
				}
			}
		}
		else
		{
			//에디터 원도우가 꺼져있으면....
			if (RegionHit(currentX, currentY, totalWidth, totalHeight))
			{
				uistate.hotItem = id;
				if (uistate.activeItem == 0 && uistate.mouseLeftDown)
				{
					uistate.activeItem = id;
					uistate.kbdItem = id;
				}
			}
		}

		//if (!CheckEditorHit(id, currentX, currentY, totalWidth, totalHeight))
		//{
		//}
		//else
		//{
		//	innerFlag = 1;
		//}

		for (int y = 1; y < totalYIndex; ++y)
		{
			DrawLine(gRenderTarget, currentX, currentY + y * frameHeight,
				currentX + totalWidth, currentY + y * frameHeight, BLACK, 1.5f);
		}
		for (int x = 1; x < totalXIndex; ++x)
		{
			DrawLine(gRenderTarget, currentX + x * frameWidth, currentY,
				currentX + x * frameWidth, currentY + totalHeight, BLACK, 1.5f);
		}

		DrawBox(gRenderTarget, currentX - OFFSET, currentY - OFFSET,
			totalWidth + OFFSET * 2, totalHeight + OFFSET * 2, BLACK, 4.0f);

		DrawBox(gRenderTarget, currentX + xIndex * frameWidth, currentY + yIndex * frameHeight,
			frameWidth, frameHeight, HOT_COLOR, 2.0f);

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
				ClampInt(&--yIndex, 0, totalYIndex - 1);
			}break;
			case VK_DOWN:
			{
				ClampInt(&++yIndex, 0, totalYIndex - 1);
			}break;
			case VK_LEFT:
			{
				ClampInt(&--xIndex, 0, totalXIndex - 1);
			}break;
			case VK_RIGHT:
			{
				ClampInt(&++xIndex, 0, totalXIndex - 1);
			}break;
			case VK_RETURN:
			{
				return 1;
			}break;
			}
		}
		uistate.lastWidget = id;

		if (uistate.activeItem == id && !innerFlag)
		{
			int mouseRelX = uistate.mouseX - currentX;
			int mouseRelY = uistate.mouseY - currentY;
			int mouseXIndex = mouseRelX / frameWidth;
			int mouseYIndex = mouseRelY / frameHeight;

			ClampInt(&mouseXIndex, 0, totalXIndex - 1);
			ClampInt(&mouseYIndex, 0, totalYIndex - 1);

			xIndex = mouseXIndex;
			yIndex = mouseYIndex;

			return 1;
		}
		return 0;
	}
#pragma endregion
}
