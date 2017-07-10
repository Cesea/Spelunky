#include "stdafx.h"
#include "D2DSprite.h"

D2DSprite::D2DSprite()
{
}

D2DSprite::~D2DSprite()
{
}

HRESULT D2DSprite::Init(D2DImage *sourceImage, float sourceX, float sourceY, float widthPerFrame, float heightPerFrame, const IntVector2 &anchor)
{
	_sourceImage = sourceImage;
	_sourceX = sourceX;
	_sourceY = sourceY;
	_widthPerFrame = widthPerFrame;
	_heightPerFrame = heightPerFrame;

	_anchor = anchor;
	return S_OK;
}

HRESULT D2DSprite::Init(D2DImage * sourceImage, Animation *animation, const IntVector2 &anchor)
{
	return S_OK;
}

//HRESULT D2DSprite::InitCopy(D2DSprite * sourceSprite)
//{
//	_sourceImage = sourceSprite->GetSourceImage();
//	_sourceX = sourceSprite->GetSourceX();
//	_sourceY = sourceSprite->GetSourceY();
//	_widthPerFrame = sourceSprite->GetWidthPerFrame();
//	_heightPerFrame = sourceSprite->GetHeightPerFrame();
//	return S_OK;
//}

void D2DSprite::Update(float deltaTime)
{
}

void D2DSprite::Render(ID2D1HwndRenderTarget *renderTarget, float alpha)
{
	if (!_flipedX)
	{
		_sourceImage->Render(renderTarget, _sourceX, _sourceY, _widthPerFrame, _heightPerFrame, alpha);
	}
	else
	{
		_sourceImage->RenderFlipX(renderTarget, alpha);
	}
}

void D2DSprite::Render(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float alpha)
{
	if (!_flipedX)
	{
		_sourceImage->Render(renderTarget, destX + _anchor.x, destY + _anchor.y, _sourceX, _sourceY, _widthPerFrame, _heightPerFrame, alpha);
	}
	else
	{
		_sourceImage->RenderFlipX(renderTarget, destX + _anchor.x, destY + _anchor.y, _sourceX, _sourceY, _widthPerFrame, _heightPerFrame, alpha);
	}
}

//void D2DSprite::Render(ID2D1HwndRenderTarget * renderTarget, float destX, float destY, float destWidth, float destHeight, float alpha)
//{
//	if (!_flipedX)
//	{
//		_sourceImage->Render(renderTarget, destX, destY, destWidth, destHeight, _sourceX, _sourceY, _widthPerFrame, _heightPerFrame, alpha);
//	}
//	else
//	{
//		_sourceImage->RenderFlipX(renderTarget, destX, destY, _sourceX, _sourceY, _widthPerFrame, _heightPerFrame, alpha);
//	}
//}
//
//void D2DSprite::RenderFromCenter(ID2D1HwndRenderTarget * renderTarget, float destX, float destY, float alpha)
//{
//	if (!_flipedX)
//	{
//		_sourceImage->Render(renderTarget, destX - _widthPerFrame / 2,
//			destY - _heightPerFrame / 2,
//			_sourceX, _sourceY, _widthPerFrame, _heightPerFrame, alpha);
//	}
//	//x 축만 반전
//	else if (_flipedX)
//	{
//		_sourceImage->RenderFlipX(renderTarget, destX - _widthPerFrame / 2,
//			destY - _heightPerFrame / 2,
//			_sourceX, _sourceY, _widthPerFrame, _heightPerFrame, alpha);
//	}
//}
//
//void D2DSprite::RenderFromCenter(ID2D1HwndRenderTarget * renderTarget, float destX, float destY, float destWidth, float destHeight, float alpha)
//{
//	if (!_flipedX)
//	{
//		_sourceImage->Render(renderTarget, destX - destWidth / 2,
//			destY - destHeight / 2,
//			destWidth, destHeight,
//			_sourceX, _sourceY, _widthPerFrame, _heightPerFrame, alpha);
//	}
//	//x 축만 반전
//	else if (_flipedX)
//	{
//		//_sourceImage->RenderFlipX(renderTarget, destX - _widthPerFrame / 2,
//		//	destY - _heightPerFrame / 2,
//		//	destWidth, destHeight,
//		//	_sourceX, _sourceY, _widthPerFrame, _heightPerFrame, alpha);
//	}
//}

void D2DSprite::Release()
{
	_sourceImage = nullptr;
}
