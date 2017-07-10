#pragma once

#include "D2D.h"

#include "ImGUI.h"

class gameNode
{
public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

	//�޽��� ���ν���
	virtual LRESULT MainProc(HWND hWnd, UINT iMessage,
		WPARAM wParam, LPARAM lParam);

	gameNode();
	virtual ~gameNode();

protected :

};

