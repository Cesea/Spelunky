#include "stdafx.h"
#include "SpriteObject.h"

SpriteObject::SpriteObject()
{
}

SpriteObject::~SpriteObject()
{
}

HRESULT SpriteObject::Init(const std::wstring &key, int sourceX, int sourceY, int frameWidth, int frameHeight, const IntVector2 &anchor)
{
	_sprite = new D2DSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(key), sourceX, sourceY, frameWidth, frameHeight, anchor);
	return S_OK;
}

HRESULT SpriteObject::InitAnimation(const std::wstring &key,Animation *animation, const IntVector2 &anchor)
{
	_sprite = new D2DAnimationSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(key), animation, anchor);

	_sprite->GetAnimation()->Start();
	return S_OK;
}

HRESULT SpriteObject::InitFrame(const std::wstring &key,int frameWidth, int frameHeight, const IntVector2 &anchor)
{
	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(key), frameWidth, frameHeight, anchor);
	return S_OK;
}


void SpriteObject::Release(void)
{
	SAFE_RELEASE_AND_DELETE(_sprite);
	if (_hasAlpha)
	{
		SAFE_RELEASE_AND_DELETE(_alphaSprite);
	}
}

void SpriteObject::Update(float deltaTime)
{
	if (_interpolating)
	{
		if (_timer.Tick(deltaTime))
		{
			_timer.Reset();
			_interpolating = false;
			position = _targetPosition;
		}
		float t = _timer.GetCurrentSecond() / _timer.GetTargetSecond();

		position = InterpolateVector(_startPosition, _targetPosition, t);
	}

	_sprite->Update(deltaTime);

}

void SpriteObject::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos, float alpha)
{
	Vector2 drawingPos = position - camPos;
	_sprite->Render(renderTarget, drawingPos.x, drawingPos.y, alpha);
	if (_hasAlpha)
	{
		_alphaSprite->Render(renderTarget, drawingPos.x, drawingPos.y, _scale);
	}
}

void SpriteObject::Render(ID2D1HwndRenderTarget * renderTarget, int sourceX, int sourceY, const Vector2 & camPos, float alpha)
{
	Vector2 drawingPos = position - camPos;
	_sprite->FrameRender(renderTarget, drawingPos.x, drawingPos.y, sourceX, sourceY, alpha);
	if (_hasAlpha)
	{
		_alphaSprite->FrameRender(renderTarget, drawingPos.x, drawingPos.y, sourceX, sourceY, _scale);
	}
}

void SpriteObject::RenderScale(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos, float alpha)
{
	Vector2 drawingPos = position - camPos;
	_sprite->RenderMatrix(renderTarget, drawingPos.x, drawingPos.y, D2D1::Matrix3x2F::Scale(_scale, _scale), alpha);
	if (_hasAlpha)
	{
		_alphaSprite->RenderMatrix(renderTarget, drawingPos.x, drawingPos.y, D2D1::Matrix3x2F::Scale(_scale, _scale), 1.0f - _scale);
	}
}

HRESULT SpriteObject::AlphaInit(const std::wstring & key, int sourceX, int sourceY, int frameWidth, int frameHeight, const IntVector2 & anchor)
{
	_alphaSprite = new D2DSprite;
	_alphaSprite->Init(IMAGEMANAGER->GetImage(key), sourceX, sourceY, frameWidth, frameHeight, anchor);
	_hasAlpha = true;
	return S_OK;
}

HRESULT SpriteObject::AlphaInitAnimation(const std::wstring & key, Animation * animation, const IntVector2 & anchor)
{
	_alphaSprite = new D2DAnimationSprite;
	_alphaSprite->Init(IMAGEMANAGER->GetImage(key), animation, anchor);
	_alphaSprite->GetAnimation()->Start();

	_hasAlpha = true;
	return S_OK;
}

HRESULT SpriteObject::AlphaInitFrame(const std::wstring & key, int frameWidth, int frameHeight, const IntVector2 & anchor)
{
	_alphaSprite = new D2DFrameSprite;
	_alphaSprite->Init(IMAGEMANAGER->GetImage(key), frameWidth, frameHeight, anchor);
	_hasAlpha = true;
	return S_OK;
}

void SpriteObject::MoveTo(const Vector2 & targetPos, float time)
{
	_startPosition = position;
	_targetPosition = targetPos;
	_timer.ResetAndChangeTargetSecond(time);

	_interpolating = true;
}
