#ifndef DWRITE_H
#define DWRITE_H

#include "stdafx.h"

//NOTE : DWrite ����� 

enum Alignment
{
	ALIGN_CENTER,
	ALIGN_LEFT
};

class DWrite
{
public:
	HRESULT Init();

	HRESULT SetFont(const WCHAR *fontName, float fontSize);

	void Release();

	void PrintTextFromFormat(ID2D1HwndRenderTarget *renderTarget, float x, float y, float width, float height,
		const WCHAR *str, const D2D1_COLOR_F &brushColor, IDWriteTextFormat *format);

	void PrintText(ID2D1HwndRenderTarget *renderTarget, float x, float y, float width, float height,
		const WCHAR *str, const D2D1_COLOR_F &brushColor);

	//�� �Լ��� �׸��� �� ���Ŀ� �� x ��ǥ�� ��ȯ
	float PrintTextLayout(ID2D1HwndRenderTarget *renderTarget, float x, float y, float width, float height,
		const WCHAR *str, const D2D1_COLOR_F &brushColor);

	void AlignFont(Alignment alignment);
	float CalculateInputTextWidth(const WCHAR *str);

	HRESULT CreateTextFormat(IDWriteTextFormat **format, const WCHAR *fontName, float fontSize);

private:
	IDWriteFactory *_dWriteFactory{};
	IDWriteTextFormat *_textFormat{};

	IDWriteTextLayout *_textLayout{};

	//IDWriteFontCollection *_fontCollection{};
	//IDWriteFontFamily *_fontFamily{};
	//IDWriteFont *_font{};

};


#endif