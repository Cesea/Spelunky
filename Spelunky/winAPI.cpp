#include "stdafx.h"
#include "Game.h"

//=================================================
//======== 20170309 #��������# ========
//=================================================
HINSTANCE		_hInstance; //���α׷� �ν��Ͻ�
HWND			_hWnd; //�������ڵ�
Game			_game;
ScreenVector2	currentMouse = { 0, 0 }; //���콺 ��ġ
D2D				_d2d;
DWrite			_dWrite;
Console			_console;

bool _running = false;

//=================================================
//======== 20170309 #�Լ� ������Ÿ��# ========
//=================================================
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowSize(int x, int y, int width, int height);
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPTSTR lpszCmdParam, int cmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpszCmdParam);
	MSG message;
	WNDCLASS wndClass;

	//������ �� ����
	_hInstance = hInstance; //�޽��� ó���Լ� �������ش�
	wndClass.cbClsExtra = 0; //Ŭ���� ���� �޸�
	wndClass.cbWndExtra = 0; //������ ���� �޸�
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //��׶��� ����
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ�����
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������
	wndClass.hInstance = hInstance;  //�ν��Ͻ�
	wndClass.lpfnWndProc = (WNDPROC)WndProc; //���ν��� (�޽��� ó���Լ�)
	wndClass.lpszClassName = WINNAME; //Ŭ�����̸�
	wndClass.lpszMenuName = NULL; //�޴��̸�
	wndClass.style = CS_HREDRAW | CS_VREDRAW; //������ ��Ÿ��

	//������ Ŭ���� ���
	RegisterClass(&wndClass);

	//������ ����
	_hWnd = CreateWindow(
		WINNAME,				//������Ŭ�����̸�
		WINNAME,				//������Ÿ��Ʋ�ٿ� ��Ÿ�� �̸�
		WINSTYLE,				//�����콺Ÿ��
		WINSTARTX,					//������ ȭ�� ��ǥ x
		WINSTARTY,					//������ ȭ�� ��ǥ y
		WINSIZEX,					//������ ȭ�� ��ǥ width
		WINSIZEY,					//������ ȭ�� ��ǥ height
		NULL,					//�θ� ������
		(HMENU)NULL,			//�޴��ڵ�
		hInstance,				//������ �ν��Ͻ� ����
		NULL					//mdiŬ���̾�Ʈ ������ �� �ڽ� �����츦 �����ϸ�
		);						//�������ֵ� ������� ���� ����... NULL

	setWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	_d2d.Init();
	_d2d.CreateRenderTarget(_hWnd);

	_dWrite.Init();
	_dWrite.SetFont(L"resources/fonts/DK Appelstroop.otf", 16);
	
	//ȭ�鿡 ������ �����ش�
	ShowWindow(_hWnd, cmdShow);
	
	//PeekMessage : �޽����� ��� ���ϵǴ� �Լ��̴�... �ͺ�... (������~~)
	//GetMessage : �޽����� �����ö����� ���Ǵ� �Լ�...

	//��� �ʱ�ȭ
	if (FAILED(_game.Init()))
	{
		//���н� �ٷ� �����Ѵ�...
		return 0;
	}
	_running = true;

	_console.Init();
	
	Console::Log("Hello\n");

	BringWindowToTop(_hWnd);

	while (true) //���ӿ�
	{
		BOOL result = PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE);
		if (result > 0)
		{
			if (message.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			_game.Update();
			_game.Render(_d2d.GetRenderTarget());
			EVENTMANAGER->Update(TIMEMANAGER->GetElapsedTime());
		}
	}

	//�޽�������
	//while (GetMessage(&message, 0, 0, 0)) //�Ϲ� ���α׷���
	//{
	//	TranslateMessage(&message);
	//	DispatchMessage(&message);
	//}

	//���ΰ��� ����....
	_game.Release();

	_dWrite.Release();

	//��ϵ� ������ Ŭ���� �����Ѵ�
	UnregisterClass(WINNAME, hInstance);

	_console.Release();

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage,
	WPARAM wParam, LPARAM lParam)
{
	return _game.MainProc(hWnd, iMessage, wParam, lParam);
}

void setWindowSize(int x, int y, int width, int height)
{
	//POINT pt;
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WINSTYLE, false); //false�� �޴��� �ִ��� ������..
	SetWindowPos(_hWnd, NULL, x, y, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOZORDER | SWP_NOMOVE);
}
