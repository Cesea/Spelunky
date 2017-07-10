#ifndef DWRITE_H
#define DWRITE_H

#include "stdafx.h"

//NOTE : DWrite 사용방법 


class DWrite
{
public:
	HRESULT Init();

	HRESULT SetFont(const WCHAR *fontName, float fontSize);

	void Release();

	void PrintText(ID2D1HwndRenderTarget *renderTarget, float x, float y, float width, float height,
		const WCHAR *str, const D2D1_COLOR_F &brushColor);

private:
	IDWriteFactory *_dWriteFactory{};
	IDWriteTextFormat *_textFormat{};

	IDWriteTextLayout *_textLayout{};

	//IDWriteFontCollection *_fontCollection{};
	//IDWriteFontFamily *_fontFamily{};
	//IDWriteFont *_font{};

};


#endif