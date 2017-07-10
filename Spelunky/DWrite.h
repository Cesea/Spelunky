#ifndef DWRITE_H
#define DWRITE_H

#include "stdafx.h"

//NOTE : DWrite 사용방법 

//1. Init 함수 호출 
//2. SetFont로 폰트의 종류랑 폰트사이즈 설정
//3.PrintText로 텍스트 출력!!

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