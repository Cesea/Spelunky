#include "stdafx.h"
#include "DWrite.h"

HRESULT DWrite::Init()
{
	HRESULT result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown **>(&_dWriteFactory));

	//result = _dWriteFactory->CreateTextFormat(L"Gabriola", NULL,
	//	DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en_us", &_textFormat);

	//NOTE : This is for center alignment


	return S_OK;
}

HRESULT DWrite::SetFont(const WCHAR *fontName, float fontSize)
{
	HRESULT	result = E_FAIL;
	//���� �̹� �ִٸ� �����ϰ� �����Ѵ�.
	SAFE_RELEASE(_textFormat);

	result = _dWriteFactory->CreateTextFormat(L"Andy", NULL,
		DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"en_us", &_textFormat);


	_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	return result;
}

void DWrite::Release()
{
	SAFE_RELEASE(_textFormat);
	SAFE_RELEASE(_dWriteFactory);

}

void DWrite::PrintText(ID2D1HwndRenderTarget *renderTarget, float x, float y,float width, float height,
	const WCHAR * str, const D2D1_COLOR_F &brushColor)
{
	ID2D1SolidColorBrush *brush = nullptr;
	renderTarget->CreateSolidColorBrush(brushColor, &brush);

	D2D1_RECT_F layoutRect = { x, y, x + width, y + height };
	renderTarget->DrawTextW(str, wcslen(str), _textFormat, layoutRect, brush);

	brush->Release();
	brush = nullptr;
}

float DWrite::PrintTextLayout(ID2D1HwndRenderTarget * renderTarget, float x, float y, float width, float height, const WCHAR * str, const D2D1_COLOR_F & brushColor)
{
	ID2D1SolidColorBrush *brush = nullptr;
	renderTarget->CreateSolidColorBrush(brushColor, &brush);

	_dWriteFactory->CreateTextLayout(str, wcslen(str), _textFormat, width, height, &_textLayout);

	D2D1_RECT_F layoutRect = { x, y, x + width, y + height };
	renderTarget->DrawTextLayout(D2D1::Point2F(x, y), _textLayout, brush);

	DWRITE_TEXT_METRICS metrics;
	_textLayout->GetMetrics(&metrics);

	brush->Release();
	brush = nullptr;

	return metrics.width;
}

void DWrite::AlignFont(Alignment alignment)
{
	if (alignment == ALIGN_CENTER)
	{
		_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	else if(alignment == ALIGN_LEFT)
	{
		_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	}
}

float DWrite::CalculateInputTextWidth(const WCHAR * str)
{
	_dWriteFactory->CreateTextLayout(str, wcslen(str), _textFormat, WINSIZEX, WINSIZEY, &_textLayout);
	DWRITE_TEXT_METRICS metrics;
	_textLayout->GetMetrics(&metrics);

	return metrics.width;
}
