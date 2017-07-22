#include "stdafx.h"
#include "Game.h"

//=================================================
//======== 20170309 #전역변수# ========
//=================================================
HINSTANCE		_hInstance; //프로그램 인스턴스
HWND			_hWnd; //윈도우핸들
Game			_game;
ScreenVector2	currentMouse = { 0, 0 }; //마우스 위치
D2D				_d2d;
DWrite			_dWrite;
Console			_console;
bool			_leftDown = false;


char dataPath[50] = "resources\\data\\";
char gfxPath[50] = "resources\\gfx";
char sfxPath[50] = "resources\\sfx";

WCHAR LdataPath[50] = L"resources\\data\\";;
WCHAR LgfxPath[50]= L"resources\\gfx";;
WCHAR LsfxPath[50] = L"resources\\sfx";;

ID2D1HwndRenderTarget *	gRenderTarget;

bool _running = false;

//=================================================
//======== 20170309 #함수 프로토타입# ========
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

	//윈도우 값 셋팅
	_hInstance = hInstance; //메시지 처리함수 지정해준다
	wndClass.cbClsExtra = 0; //클래스 여분 메모리
	wndClass.cbWndExtra = 0; //윈도우 여분 메모리
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //백그라운드 색상
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //커서모양
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘
	wndClass.hInstance = hInstance;  //인스턴스
	wndClass.lpfnWndProc = (WNDPROC)WndProc; //프로시져 (메시지 처리함수)
	wndClass.lpszClassName = WINNAME; //클래스이름
	wndClass.lpszMenuName = NULL; //메뉴이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW; //윈도우 스타일

	//윈도우 클래스 등록
	RegisterClass(&wndClass);

	//윈도우 생성
	_hWnd = CreateWindow(
		WINNAME,				//윈도우클래스이름
		WINNAME,				//윈도우타이틀바에 나타날 이름
		WINSTYLE,				//윈도우스타일
		WINSTARTX,					//윈도우 화면 좌표 x
		WINSTARTY,					//윈도우 화면 좌표 y
		WINSIZEX,					//윈도우 화면 좌표 width
		WINSIZEY,					//윈도우 화면 좌표 height
		NULL,					//부모 윈도우
		(HMENU)NULL,			//메뉴핸들
		hInstance,				//윈도우 인스턴스 지정
		NULL					//mdi클라이언트 윈도우 및 자식 윈도우를 생성하면
		);						//지정해주되 사용하지 않을 경우는... NULL

	setWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	_d2d.Init();
	_d2d.CreateRenderTarget(_hWnd);

	_dWrite.Init();
	_dWrite.SetFont(L"Tekton", 18);
	
	//화면에 윈도우 보여준다
	ShowWindow(_hWnd, cmdShow);
	
	//PeekMessage : 메시지가 없어도 리턴되는 함수이다... 넌블럭... (뺑뺑이~~)
	//GetMessage : 메시지를 꺼내올때까지 블럭되는 함수...
	_console.Init();

	//노드 초기화
	if (FAILED(_game.Init()))
	{
		//실패시 바로 종료한다...
		return 0;
	}
	_running = true;

	
	Console::Log("Hello\n");
	BringWindowToTop(_hWnd);

	gRenderTarget = _d2d.GetRenderTarget();

	bool firstUpdata = false;


	TIMEMANAGER->Tick();
	while (_running) //게임용
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
			if (!firstUpdata)
			{
				TIMEMANAGER->Tick();
				firstUpdata = true;
			}
			_game.Update();
			_game.Render();
			EVENTMANAGER->Update(TIMEMANAGER->GetElapsedTime());
		}
	}

	//메시지루프
	//while (GetMessage(&message, 0, 0, 0)) //일반 프로그램용
	//{
	//	TranslateMessage(&message);
	//	DispatchMessage(&message);
	//}

	//메인게임 해제....
	_game.Release();

	_dWrite.Release();

	//등록된 윈도우 클래스 삭제한다
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
	AdjustWindowRect(&rc, WINSTYLE, false); //false는 메뉴가 있는지 없는지..
	SetWindowPos(_hWnd, NULL, x, y, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOZORDER | SWP_NOMOVE);
}
