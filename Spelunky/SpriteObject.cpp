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
}

void SpriteObject::Render(ID2D1HwndRenderTarget * renderTarget, int sourceX, int sourceY, const Vector2 & camPos, float alpha)
{
	Vector2 drawingPos = position - camPos;
	_sprite->FrameRender(renderTarget, drawingPos.x, drawingPos.y, sourceX, sourceY, alpha);
}

void SpriteObject::MoveTo(const Vector2 & targetPos, float time)
{
	_startPosition = position;
	_targetPosition = targetPos;
	_timer.ResetAndChangeTargetSecond(time);

	_interpolating = true;
}
