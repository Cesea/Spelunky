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
		_sourceImage->Render(renderTarget, destX + _anchor.x, destY + _anchor.y,
			_animation->GetFramePos().x, _animation->GetFramePos().y,
			_animation->GetFrameWidth(), _animation->GetFrameHeight(), alpha);
	}
	else
	{
		_sourceImage->RenderFlipX(renderTarget, destX + _anchor.x, destY + _anchor.y,
			_animation->GetFramePos().x, _animation->GetFramePos().y,
			_animation->GetFrameWidth(), _animation->GetFrameHeight(), alpha);
	}

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