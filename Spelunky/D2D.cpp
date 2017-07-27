#include "stdafx.h"
#include "D2D.h"

D2D::D2D()
{
}

D2D::~D2D()
{
	//SAFE_RELEASE(WICFormatConverter);
	SAFE_RELEASE(_pWICFactory);
	SAFE_RELEASE(D2DRenderTarget);
	SAFE_RELEASE(D2DFactory);
}

void D2D::Init()
{
	HRESULT result = E_FAIL;

	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &D2DFactory);
	Assert(SUCCEEDED(result));

	CoInitialize(NULL);

	result = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&_pWICFactory));
	Assert(SUCCEEDED(result));
}

void D2D::CreateRenderTarget(HWND hwnd)
{
	HRESULT result = E_FAIL;

	RECT clientRect; 
	GetClientRect(hwnd, &clientRect);

	int32 width = clientRect.right - clientRect.left;
	int32 height = clientRect.bottom - clientRect.top;

	D2D1_SIZE_U size = { (uint32)width, (uint32)height };
	result = D2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, size),
		&D2DRenderTarget);
	Assert(SUCCEEDED(result));
}

void D2D::BeginDraw()
{
	D2DRenderTarget->BeginDraw();
	D2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2D::EndDraw()
{
	D2DRenderTarget->EndDraw();
}

HRESULT D2D::LoadBitmapFromFile(const WCHAR *fileName, ID2D1Bitmap **ppBitmap)
{
	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode * pSource = nullptr;
	IWICStream *pStream = nullptr;
	IWICFormatConverter *pConverter = nullptr;

	HRESULT result = _pWICFactory->CreateDecoderFromFilename(fileName,
		NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
	if (SUCCEEDED(result))
	{
		result = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(result))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		result = _pWICFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(result))
	{
		result = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, 
			WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeMedianCut);
	}
	if (SUCCEEDED(result))
	{
		result = D2DRenderTarget->CreateBitmapFromWicBitmap(pConverter, ppBitmap);
		int a = 0;
	}

	SAFE_RELEASE(pConverter);
	SAFE_RELEASE(pStream);
	SAFE_RELEASE(pSource);
	SAFE_RELEASE(pDecoder);

	return result;
}

//D2DImage *D2D::LoadBitmapFromFile(LPCTSTR fileName)
//{
//	D2DImage *image = nullptr;
//	image = new D2DImage();
//
//	IWICBitmapDecoder *decoder = nullptr;
//	HRESULT result = WICFactory->CreateDecoderFromFilename(fileName, nullptr,
//		GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
//	Assert(SUCCEEDED(result));
//
//	IWICBitmapFrameDecode *framePtr = nullptr;
//	result = decoder->GetFrame(0, &framePtr);
//	Assert(SUCCEEDED(result));
//
//	result = WICFactory->CreateFormatConverter(&WICFormatConverter);
//	Assert(SUCCEEDED(result));
//
//	result = WICFormatConverter->Initialize(framePtr, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
//	Assert(SUCCEEDED(result));
//
//	result = D2DRenderTarget.Get()->CreateBitmapFromWicBitmap(WICFormatConverter.Get(), nullptr, (ID2D1Bitmap **)(image->GetBitmap()) );
//	Assert(SUCCEEDED(result));
//
//	SAFE_RELEASE(framePtr);
//	SAFE_RELEASE(decoder);
//
////	D2D1_SIZE_F size = *image->GetBitmap()
////	image->SetWidth(image->GetBitmap())
//
//	image->Init();
//	return  image;
//}

ID2D1HwndRenderTarget * D2D::GetRenderTarget()
{
	return D2DRenderTarget;
}


