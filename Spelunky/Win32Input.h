#ifndef WIN32_INPUT_H
#define WIN32_INPUT_H

#include "stdafx.h"

struct DigitalButton
{
	DigitalButton() {}

	bool32 down{};
	bool32 pressed{};
	bool32 released{};

	DigitalButton &operator=(const DigitalButton &other)
	{
		down = other.down;
		pressed = other.pressed;
		released = other.released;
		return *this;
	}
};

//게임에서 사용되는 모든 키를 여기에
struct Win32KeyboardState
{
	Win32KeyboardState() {}
	union
	{
		struct
		{
			DigitalButton up;
			DigitalButton down;
			DigitalButton left;
			DigitalButton right;
			DigitalButton shift;
			DigitalButton z;
			DigitalButton x;
			DigitalButton s;
			DigitalButton space;
			DigitalButton escape;
		};
		DigitalButton buttons[10];
	};
};

struct Win32MouseState
{
	Win32MouseState() {}
	union
	{
		struct
		{
			DigitalButton leftButton;
			DigitalButton rightButton;
		};
		DigitalButton buttons[2];
	};
	Vector2 currentPoint;
	Vector2 prevPoint;
	Vector2 offset;
};

struct Win32RawInputState
{
	Win32RawInputState() {}
	Win32KeyboardState keyboard;
	Win32MouseState mouse;
};

void PullDigitalButton(DigitalButton *digitalButton, uint32 vkCode);
void PullRawInput(Win32RawInputState *rawInput);


#endif