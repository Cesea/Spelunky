#pragma once

#include "D2D.h"

//#include "InputRelated.h"

class gameNode
{
public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update();
	virtual void Render(ID2D1HwndRenderTarget *renderTarget);

	//메시지 프로시져
	virtual LRESULT MainProc(HWND hWnd, UINT iMessage,
		WPARAM wParam, LPARAM lParam);

	gameNode();
	virtual ~gameNode();

protected :

};

