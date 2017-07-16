// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
//#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "WindowsCodecs.lib")

#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "Shlwapi.lib")

//=================================================
//======== 20170309 #�������# ========
//=================================================
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN //MFC�� �ƴ� �������η��� ��쿡��
//WIN32_LEAN_AND_MEAN�� ������ ���� �ð��� �����Ų��....
//commdlg.h���.... �ʿ���� ������� ��Ŭ��� ���� �ʴ´�

//������ ������� ��Ŭ���
#define NOMINMAX
#include <windows.h>

#include <stdint.h>

#include <Shlwapi.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodec.h>

#include <dwrite.h>

//���Ĵٵ� ����� ��� ��Ŭ���.. (printf, scanf, puts, gets.. ���..)
#include <stdio.h>	

//�����쿡�� ����� ���ڿ� ��� ��� ��Ŭ��� �Ѵ�...
//(TextOut, DrawText, wsprintf���..)
#include <tchar.h>
#include "randomFunction.h"
#include "keyManager.h"
#include "Layer.h"

//#include "GameCommonMacroFunctions.h"


#if defined _DEBUG
	#define Assert(expression) if((!expression)){*(int *)0 = 0;}
#else
	#define Assert(expression)
#endif

#include "ImageManager.h"
#include "FileManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "TextDataManager.h"

#include "KeyAnimationManager.h"

//=================================================
//======== 20170309 #�����ι�#======
//=================================================

#define WINNAME (LPTSTR)(TEXT("APIWindow"))
#define WINSTARTX 200
#define WINSTARTY 100

#define WINSIZEX 1280
#define WINSIZEY 720

#define WINSTYLE WS_CAPTION | WS_SYSMENU

#define M_PI 3.141592
#define M_PI2 (M_PI/2)
#define M_PI4 (M_PI/4)
#define M_PI8 (M_PI/8)
#define M_PI16 (M_PI/16)
#define M_PI32 (M_PI/32)

#define EPSILON 0.1f

#define RND randomFunction::getSingleton()
#define KEYMANAGER keyManager::getSingleton()
#define IMAGEMANAGER ImageManager::getSingleton()
#define FILEMANAGER FileManager::getSingleton()
#define TIMEMANAGER TimeManager::getSingleton()
#define SCENEMANAGER SceneManager::getSingleton()
#define TEXTDATAMANAGER TextDataManager::getSingleton()
#define KEYANIMANAGER KeyAnimationManager::getSingleton()

#define INTERPOLATIONMODE D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR

//��ũ�� �Լ�... (�����ϰ� �����Ѵ�)
#define SAFE_DELETE(p)			{if(p) {delete (p);		(p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p);	(p) = NULL;}}
#define SAFE_RELEASE(p)			{if(p) {(p)->Release(); (p) = NULL;}}
#define SAFE_RELEASE_AND_DELETE(p) {if(p) {(p)->Release(); delete(p); (p) = NULL;}}

#define GRAVITY 1250.0f

#define TILE_SIZE 64
#define TILE_SIZE_HALF 32

#define ROOM_TILE_COUNTX 10
#define ROOM_TILE_COUNTY 8

#define ROOM_COUNTX 4
#define ROOM_COUTY 4

#define STAGE_TILE_COUNTX 40
#define STAGE_TILE_COUNTY 32

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef int32 bool32;

#include "Vector2.h"
#include "Vector3.h"

#include "IntVector2.h"

//typedefs
typedef Vector2 ScreenVector2;
typedef Vector2 MeterVector2;

typedef Vector3 ScreenVector3;
typedef Vector3 MeterVector3;

#include "D2D.h"
#include "DWrite.h"

#include "commonMacroFunction.h"
#include "GameCommonMacroFunctions.h"

#include "Event.h"
#include "EventManager.h"
#include "GameObjectManager.h"


#define EVENTMANAGER EventManager::getSingleton()
#define OBJECTMANAGER GameObjectManager::getSingleton()

#include "Win32Input.h"
#include "Utillities.h"

#include "Console.h"

#include "D2DAnimationSprite.h"
#include "D2DFrameSprite.h"

#include "File.h"

#include "StageManager.h"

#define STAGEMANAGER PlayScene::StageManager::getSingleton()

//=================================================
//======== 20170309 #��������# ========
//=================================================
extern HINSTANCE _hInstance; //���α׷� �ν��Ͻ�
extern HWND _hWnd; //�������ڵ�
extern ScreenVector2 currentMouse; //���콺~~
extern D2D _d2d;
extern DWrite _dWrite;

extern ID2D1HwndRenderTarget *gRenderTarget;

extern bool _running;
extern Console _console;


extern char dataPath[50];
extern char gfxPath[50];
extern char sfxPath[50];

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

#endif
