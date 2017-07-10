#include "stdafx.h"
#include "gameNode.h"

gameNode::gameNode()
{
}

gameNode::~gameNode()
{
}

//ÃÊ±âÈ­...
HRESULT gameNode::Init(void)
{
	//_d2d.Init();
	//_d2d.CreateRenderTarget(_hWnd);
	
	return S_OK;
}

//¹ÝÀå ÇØÁ¦ÇÏÀÚ..
void gameNode::Release(void)
{
	//·£´ý ½Ì±ÛÅæ ÇØÁ¦..
	//RND->releaseSingleton();
	//KEYMANAGER->releaseSingleton();
	//IMAGEMANAGER->Release();

}

void gameNode::Update()
{
}

void gameNode::Render()
{
}

//void gameNode::setBackBuffer(void)
//{
//	_backBuffer = new Image;
//	_backBuffer->Init(WINSIZEX, WINSIZEY);
//}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, 
	WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (iMessage)
	{
		case WM_MOUSEMOVE:
		{
			currentMouse.x = static_cast<float>(LOWORD(lParam));
			currentMouse.y = static_cast<float>(HIWORD(lParam));
		} break;
		case WM_KEYDOWN:
		{
			IM::uistate.keyEntered = wParam;

			switch (wParam)
			{
			case VK_ESCAPE:
				PostMessage(hWnd, WM_DESTROY, 0, 0);
				break;
			}
		} break;

		case WM_CHAR :
		{
			IM::uistate.keyChar = wParam;
		}break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		} break;
		default :
		{
			result = DefWindowProc(hWnd, iMessage, wParam, lParam);
		}break;
	}
	return result;
}
