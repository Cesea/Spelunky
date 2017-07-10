#ifndef D2D_H
#define D2D_H

#include "stdafx.h"
#include "D2DImage.h"

#include <wrl.h>

class D2D
{
public :

	D2D();
	~D2D();

	void Init();
	void CreateRenderTarget(HWND hwnd);

	void BeginDraw();
	void EndDraw();

	HRESULT LoadBitmapFromFile( LPCTSTR fileName,
		D2DImage *pImage);

//	void ProcessImageAppropreatly();

	ID2D1HwndRenderTarget *GetRenderTarget();

private :

	ID2D1Factory *D2DFactory;
	ID2D1HwndRenderTarget *D2DRenderTarget;
	IWICImagingFactory *_pWICFactory;
	//Microsoft::WRL::ComPtr<IWICFormatConverter> WICFormatConverter;

	//ID2D1HwndRenderTarget *D2DRenderTarget{nullptr};
	////extern ID2D1SolidColorBrush *gBrush;
	//IWICImagingFactory *WICFactory{nullptr};
	//IWICFormatConverter *WICFormatConverter{nullptr};
//	ID2D1Bitmap *D2DBitmap;

};


#endif