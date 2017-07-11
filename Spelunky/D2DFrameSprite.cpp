#include "stdafx.h"
#include "D2DFrameSprite.h"

D2DFrameSprite::D2DFrameSprite()
{
}

D2DFrameSprite::~D2DFrameSprite()
{
}

HRESULT D2DFrameSprite::Init(D2DImage * sourceImage, float widthPerFrame, float heightPerFrame, const IntVector2 & anchor)
{
	_sourceImage = sourceImage;
	_imageWidth = sourceImage->GetWidth();
	_imageHeight = sourceImage->GetHeight();

	_widthPerFrame = widthPerFrame;
	_heightPerFrame = heightPerFrame;

	_xCount = _imageWidth / widthPerFrame;
	_yCount = _imageHeight / heightPerFrame;
	

	return S_OK;
}

void D2DFrameSprite::Update(float deltaTime)
{
}

void D2DFrameSprite::Render(ID2D1HwndRenderTarget * renderTarget, int xIndex, int yIndex, float alpha)
{
	_sourceImage->Render(renderTarget, 0, 0, xIndex * _widthPerFrame, yIndex * _heightPerFrame, alpha);
}

void D2DFrameSprite::Render(ID2D1HwndRenderTarget * renderTarget, float destX, float destY,
	int xIndex, int yIndex,  float alpha)
{
	_sourceImage->Render(renderTarget, destX, destY, xIndex * _widthPerFrame, yIndex * _heightPerFrame, alpha);
}

void D2DFrameSprite::Release()
{
	_sourceImage = nullptr;
}
