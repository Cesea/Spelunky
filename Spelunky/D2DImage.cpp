#include "stdafx.h"
#include "D2DImage.h"

D2DImage::D2DImage()
{
}

D2DImage::~D2DImage()
{
}

HRESULT D2DImage::Init()
{
	D2D1_SIZE_F size = _bitmap->GetSize();
	_width = size.width;
	_height = size.height;
	return S_OK;
}

#pragma region render 
void D2DImage::Render(ID2D1HwndRenderTarget * renderTarget, float alpha)
{
	Assert(_bitmap);

	D2D1::Matrix3x2F matTranslation;
	matTranslation = D2D1::Matrix3x2F::Translation(_x, _y);
	renderTarget->SetTransform(matTranslation);

	renderTarget->DrawBitmap(_bitmap);

	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::Render(ID2D1HwndRenderTarget * renderTarget, float destX, float destY, float alpha)
{
	Assert(_bitmap);
	D2D1::Matrix3x2F matTranslation;
	matTranslation = D2D1::Matrix3x2F::Translation(destX, destY);
	renderTarget->SetTransform(matTranslation);

	D2D1_SIZE_F size = _bitmap->GetSize();
	D2D1_RECT_F destRect = D2D1::RectF(0, 0, _width, _height);
	D2D1_RECT_F sourceRect = D2D1::RectF(0, 0, size.width, size.height);
	renderTarget->DrawBitmap(_bitmap, destRect, alpha, INTERPOLATIONMODE, sourceRect);
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::Render(ID2D1HwndRenderTarget * renderTarget, float sourX, float sourY, float sourWidth, float sourHeight, float alpha)
{
	Assert(_bitmap);
	D2D1::Matrix3x2F matTranslation;
	matTranslation = D2D1::Matrix3x2F::Translation(_x, _y);
	renderTarget->SetTransform(matTranslation);

	//	D2D1_SIZE_F size = _bitmap->GetSize();
	D2D1_RECT_F destRect = D2D1::RectF(0, 0, sourWidth, sourHeight);
	D2D1_RECT_F sourceRect = D2D1::RectF(sourX, sourY, sourX + sourWidth, sourY + sourHeight);
	renderTarget->DrawBitmap(_bitmap, destRect, alpha, INTERPOLATIONMODE, sourceRect);

	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}


void D2DImage::Render(ID2D1HwndRenderTarget * renderTarget, float destX, float destY, float sourX, float sourY, float sourWidth, float sourHeight, float alpha)
{
	Assert(_bitmap);
	D2D1::Matrix3x2F matTranslation;
	matTranslation = D2D1::Matrix3x2F::Translation(destX, destY);
	renderTarget->SetTransform(matTranslation);

	//	D2D1_SIZE_F size = _bitmap->GetSize();
	D2D1_RECT_F destRect = D2D1::RectF(0, 0, sourWidth, sourHeight);
	D2D1_RECT_F sourceRect = D2D1::RectF(sourX, sourY, sourX + sourWidth, sourY + sourHeight);
	renderTarget->DrawBitmap(_bitmap, destRect, alpha, INTERPOLATIONMODE, sourceRect);

	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::Render(ID2D1HwndRenderTarget * renderTarget, float destX, float destY, float destWidth, float destHeight, float sourX, float sourY, float sourWidth, float sourHeight, float alpha)
{
	Assert(_bitmap);

	D2D1::Matrix3x2F matTranslation;
	matTranslation = D2D1::Matrix3x2F::Translation(destX, destY);
	renderTarget->SetTransform(matTranslation);

	//	D2D1_SIZE_F size = _bitmap->GetSize();
	D2D1_RECT_F destRect = D2D1::RectF(0, 0, destWidth, destHeight);
	D2D1_RECT_F sourceRect = D2D1::RectF(sourX, sourY, sourX + sourWidth, sourY + sourHeight);
	renderTarget->DrawBitmap(_bitmap, destRect, alpha, INTERPOLATIONMODE, sourceRect);

	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}
#pragma endregion


//FlipRender

#pragma region flipx
void D2DImage::RenderFlipX(ID2D1HwndRenderTarget * renderTarget, float alpha)
{
	Assert(_bitmap);

	D2D1::Matrix3x2F matTranslation;
	matTranslation = D2D1::Matrix3x2F::Translation(_x, _y);

	D2D1::Matrix3x2F matSize;
	matSize = D2D1::Matrix3x2F::Scale(-1, 1);

	renderTarget->SetTransform(matSize * matTranslation);

	renderTarget->SetTransform(matSize);

	renderTarget->DrawBitmap(_bitmap);

	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::RenderFlipX(ID2D1HwndRenderTarget * renderTarget, float destX, float destY, float alpha)
{
	Assert(_bitmap);
	D2D1::Matrix3x2F matTranslation;
	matTranslation = D2D1::Matrix3x2F::Translation(destX, destY);

	D2D1::Matrix3x2F matSize;
	matSize = D2D1::Matrix3x2F::Scale(-1, 1);

	renderTarget->SetTransform(matSize * matTranslation);

	renderTarget->SetTransform(matSize);

	D2D1_SIZE_F size = _bitmap->GetSize();
	D2D1_RECT_F destRect = D2D1::RectF(0, 0, _width, _height);
	D2D1_RECT_F sourceRect = D2D1::RectF(0, 0, size.width, size.height);
	renderTarget->DrawBitmap(_bitmap, destRect, alpha, INTERPOLATIONMODE, sourceRect);
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::RenderFlipX(ID2D1HwndRenderTarget * renderTarget, float sourX, float sourY, float sourWidth, float sourHeight, float alpha)
{
	Assert(_bitmap);

	D2D1::Matrix3x2F matTranslation;
	matTranslation = D2D1::Matrix3x2F::Translation(sourWidth + _x, _y);

	D2D1::Matrix3x2F matSize;
	matSize = D2D1::Matrix3x2F::Scale(-1, 1);

	renderTarget->SetTransform(matSize * matTranslation);

	D2D1_RECT_F destRect = D2D1::RectF(0, 0, sourWidth, sourHeight);
	D2D1_RECT_F sourceRect = D2D1::RectF(sourX, sourY, sourX + sourWidth, sourY + sourHeight);
	renderTarget->DrawBitmap(_bitmap, destRect, alpha, INTERPOLATIONMODE, sourceRect);

	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::RenderFlipX(ID2D1HwndRenderTarget * renderTarget, float destX, float destY, float sourX, float sourY, float sourWidth, float sourHeight, float alpha)
{
	Assert(_bitmap);
	D2D1::Matrix3x2F matTranslation;
	matTranslation = D2D1::Matrix3x2F::Translation(sourWidth + destX, destY);

	D2D1::Matrix3x2F matSize;
	matSize = D2D1::Matrix3x2F::Scale(-1, 1);
	renderTarget->SetTransform(matSize * matTranslation);

	D2D1_RECT_F destRect = D2D1::RectF(0, 0, sourWidth, sourHeight);
	D2D1_RECT_F sourceRect = D2D1::RectF(sourX, sourY, sourX + sourWidth, sourY + sourHeight);
	renderTarget->DrawBitmap(_bitmap, destRect, alpha, INTERPOLATIONMODE, sourceRect);

	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}
#pragma endregion

void D2DImage::Release()
{
	SAFE_RELEASE(_bitmap);
}
