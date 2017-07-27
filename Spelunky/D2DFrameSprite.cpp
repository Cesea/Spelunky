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
	
	_anchor = anchor;

	return S_OK;
}

void D2DFrameSprite::Update(float deltaTime)
{
}

void D2DFrameSprite::FrameRender(ID2D1HwndRenderTarget * renderTarget, int xIndex, int yIndex, float alpha)
{
	_sourceImage->Render(renderTarget, 0, 0, ConvertFloatToInt(xIndex * _widthPerFrame), 
		ConvertFloatToInt(yIndex * _heightPerFrame), alpha);
}

void D2DFrameSprite::FrameRender(ID2D1HwndRenderTarget * renderTarget, float destX, float destY,
	int xIndex, int yIndex,  float alpha)
{
	_sourceImage->Render(renderTarget, ConvertFloatToInt(destX + _anchor.x), ConvertFloatToInt(destY + _anchor.y), 
		ConvertFloatToInt(xIndex * _widthPerFrame), ConvertFloatToInt(yIndex * _heightPerFrame),
		ConvertFloatToInt(_widthPerFrame), ConvertFloatToInt(_heightPerFrame), alpha);
}

void D2DFrameSprite::FrameRenderFlip(ID2D1HwndRenderTarget * renderTarget, float destX, float destY, int xIndex, int yIndex, float alpha)
{
	_sourceImage->RenderFlipX(renderTarget, ConvertFloatToInt(destX + _anchor.x), ConvertFloatToInt(destY + _anchor.y), 
		ConvertFloatToInt(xIndex * _widthPerFrame), ConvertFloatToInt(yIndex * _heightPerFrame),
		ConvertFloatToInt(_widthPerFrame), ConvertFloatToInt(_heightPerFrame), alpha);
}

void D2DFrameSprite::FrameRenderMatrix(ID2D1HwndRenderTarget * renderTarget, float destX, float destY,
	int xIndex, int yIndex, const D2D1::Matrix3x2F & mat, float alpha)
{
	_sourceImage->RenderMatrix(renderTarget, ConvertFloatToInt(destX ), ConvertFloatToInt(destY ), 
		ConvertFloatToInt(xIndex * _widthPerFrame), ConvertFloatToInt(yIndex * _heightPerFrame), 
		ConvertFloatToInt(_widthPerFrame), ConvertFloatToInt(_heightPerFrame), mat * D2D1::Matrix3x2F::Translation(_anchor.x, _anchor.y), alpha);
}

void D2DFrameSprite::Release()
{
	_sourceImage = nullptr;
}
