#include "stdafx.h"
#include "Win32Input.h"

void PullDigitalButton(DigitalButton *digitalButton, uint32 vkCode)
{
	digitalButton->down = KEYMANAGER->IsStayKeyDown(vkCode);
	digitalButton->pressed = KEYMANAGER->IsOnceKeyDown(vkCode);
	digitalButton->released = KEYMANAGER->IsOnceKeyUp(vkCode);
}

void PullRawInput(Win32RawInputState * rawInput)
{
	//키보드 상태 최신화
	PullDigitalButton(&rawInput->keyboard.up, VK_UP);
	PullDigitalButton(&rawInput->keyboard.down, VK_DOWN);
	PullDigitalButton(&rawInput->keyboard.left, VK_LEFT);
	PullDigitalButton(&rawInput->keyboard.right, VK_RIGHT);
	PullDigitalButton(&rawInput->keyboard.space, VK_SPACE);


	PullDigitalButton(&rawInput->keyboard.shift, VK_SHIFT);
	PullDigitalButton(&rawInput->keyboard.s, 'S');
	PullDigitalButton(&rawInput->keyboard.z, 'Z');
	PullDigitalButton(&rawInput->keyboard.x, 'X');

	//PullDigitalButton(&rawInput->mouse.leftButton, VK_LBUTTON);
	//PullDigitalButton(&rawInput->mouse.rightButton, VK_RBUTTON);
}


