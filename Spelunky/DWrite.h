#ifndef DWRITE_H
#define DWRITE_H

#include "stdafx.h"

//NOTE : DWrite ����� 

//1. Init �Լ� ȣ�� 
//2. SetFont�� ��Ʈ�� ������ ��Ʈ������ ����
//3.PrintText�� �ؽ�Ʈ ���!!

class DWrite
{
public:
	HRESULT Init();

	HRESULT SetFont(const WCHAR *fontName, float fontSize);

	void Release();

	void PrintText(ID2D1HwndRenderTarget *renderTarget, float x, float y, const WCHAR *str, const D2D1_COLOR_F &brushColor);

private:
	IDWriteFactory *_dWriteFactory{};
	IDWriteTextFormat *_textFormat{};

};


#endif