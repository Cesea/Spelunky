#include "stdafx.h"
#include "D2DAnimationSprite.h"


D2DAnimationSprite::D2DAnimationSprite()
{
	D2DSprite::D2DSprite();
}

D2DAnimationSprite::~D2DAnimationSprite()
{
}

HRESULT D2DAnimationSprite::Init(D2DImage * sourceImage, Animation *animation, const IntVector2 &anchor)
{
	_sourceImage = sourceImage;
	_animation = animation;

	_anchor = anchor;

	_widthPerFrame = _animation->GetFrameWidth();
	_heightPerFrame = _animation->GetFrameHeight();
	return S_OK;
}

void D2DAnimationSprite::Update(float deltaTime)
{
	_animation->FrameUpdate(deltaTime);
}

void D2DAnimationSprite::Render(ID2D1HwndRenderTarget * renderTarget, float destX, float destY, float alpha)
{
	if (!_flipedX)
	{
		_sourceImage->Render(renderTarget, ConvertFloatToInt(destX + _anchor.x), ConvertFloatToInt(destY + _anchor.y),
			ConvertFloatToInt(_animation->GetFramePos().x), ConvertFloatToInt(_animation->GetFramePos().y),
			ConvertFloatToInt(_animation->GetFrameWidth()), ConvertFloatToInt(_animation->GetFrameHeight()), alpha);
	}
	else
	{
		_sourceImage->RenderFlipX(renderTarget, ConvertFloatToInt(destX + _anchor.x), ConvertFloatToInt(destY + _anchor.y),
			ConvertFloatToInt(_animation->GetFramePos().x), ConvertFloatToInt(_animation->GetFramePos().y),
			ConvertFloatToInt(_animation->GetFrameWidth()), ConvertFloatToInt(_animation->GetFrameHeight()), alpha);
	}

}

void D2DAnimationSprite::RenderMatrix(ID2D1HwndRenderTarget * renderTarget, float destX, float destY, 
	const D2D1::Matrix3x2F & mat, float alpha)
{
	_sourceImage->RenderMatrix(renderTarget, ConvertFloatToInt(destX + _anchor.x), ConvertFloatToInt(destY + _anchor.y),
			ConvertFloatToInt(_animation->GetFramePos().x), ConvertFloatToInt(_animation->GetFramePos().y),
			ConvertFloatToInt(_animation->GetFrameWidth()), ConvertFloatToInt(_animation->GetFrameHeight()), mat, alpha);
}

void D2DAnimationSprite::Release()
{
	_sourceImage = nullptr;
	if (_animation->GetCopyed())
	{
		SAFE_RELEASE_AND_DELETE(_animation);
	}
	//SAFE_RELEASE_AND_DELETE(_animation);
}