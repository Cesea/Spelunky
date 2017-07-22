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

	_frameWidth = _sprite->GetWidthPerFrame();
	_frameHeight = _sprite->GetHeightPerFrame();
	return S_OK;
}

HRESULT SpriteObject::InitAnimation(const std::wstring &key,Animation *animation, const IntVector2 &anchor)
{
	_sprite = new D2DAnimationSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(key), animation, anchor);

	_frameWidth = _sprite->GetWidthPerFrame();
	_frameHeight = _sprite->GetHeightPerFrame();

	_sprite->GetAnimation()->Start();
	return S_OK;
}

HRESULT SpriteObject::InitFrame(const std::wstring &key,int frameWidth, int frameHeight, const IntVector2 &anchor)
{
	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(key), frameWidth, frameHeight, anchor);

	_frameWidth = _sprite->GetWidthPerFrame();
	_frameHeight = _sprite->GetHeightPerFrame();
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
	if (_positionInterpolating)
	{
		if (_positionTimer.Tick(deltaTime))
		{
			_positionTimer.Reset();
			_positionInterpolating = false;
			position = _targetPosition;
			if (_hasPositionEndFunction)
			{
				_positionEndFunction();
			}
		}
		else
		{
			float t = _positionTimer.GetCurrentSecond() / _positionTimer.GetTargetSecond();
			position = InterpolateVector(_startPosition, _targetPosition, t);
		}
	}
	if (_rotationInterpolating)
	{
		if (_rotationTimer.Tick(deltaTime))
		{
			_rotationTimer.Reset();
			_rotationInterpolating = false;
			degree = _targetDegree;
			if (_hasRotationEndFunction)
			{
				_rotationEndFunction();
			}
		}
		else
		{
			float t = _rotationTimer.GetCurrentSecond() / _rotationTimer.GetTargetSecond();
			degree = InterpolateFloat(_startDegree, _targetDegree, t);
		}
	}
	if (_scaleInterpolating)
	{
		if (_rotationTimer.Tick(deltaTime))
		{
			_scaleTimer.Reset();
			_scaleInterpolating = false;
			scale = _targetScale;
			if (_hasScaleEndFunction)
			{
				_scaleEndFunction();
			}
		}
		else
		{
			float t = _scaleTimer.GetCurrentSecond() / _scaleTimer.GetTargetSecond();
			scale = InterpolateVector(_startScale, _targetScale, t);
		}
	}

	if (_disappearInterpolating)
	{
		if (_disappearTimer.Tick(deltaTime))
		{
			_disappearTimer.Reset();
			_disappearInterpolating = false;
			alpha = _targetAlpha;
			if (_hasDisappearEndFunction)
			{
				_disappearEndFunction();
			}
		}
		else
		{
			float t = _disappearTimer.GetCurrentSecond() / _disappearTimer.GetTargetSecond();
			alpha = InterpolateFloat(_startAlpha, _targetAlpha, t);
		}
	}

	_sprite->Update(deltaTime);

}

void SpriteObject::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position - camPos;
	_sprite->Render(renderTarget, drawingPos.x, drawingPos.y, alpha);
	if (_hasAlpha)
	{
		_alphaSprite->Render(renderTarget, drawingPos.x, drawingPos.y, scale.x);
	}
}

void SpriteObject::Render(ID2D1HwndRenderTarget * renderTarget, int sourceX, int sourceY, const Vector2 & camPos)
{
	Vector2 drawingPos = position - camPos;
	_sprite->FrameRender(renderTarget, drawingPos.x, drawingPos.y, sourceX, sourceY, alpha);
	if (_hasAlpha)
	{
		_alphaSprite->FrameRender(renderTarget, drawingPos.x, drawingPos.y, sourceX, sourceY, scale.x);
	}
}

void SpriteObject::RenderScale(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position - camPos;
	_sprite->RenderMatrix(renderTarget, drawingPos.x, drawingPos.y,
		D2D1::Matrix3x2F::Scale(scale.x, scale.y), alpha);
	if (_hasAlpha)
	{
		_alphaSprite->RenderMatrix(renderTarget, drawingPos.x, drawingPos.y,
			D2D1::Matrix3x2F::Scale(scale.x, scale.y), 1.0f - scale.x);
	}
}

void SpriteObject::RenderRotate(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position - camPos;
	D2D1::Matrix3x2F rotMatrix = D2D1::Matrix3x2F::Rotation(degree, D2D1::Point2F(_frameWidth / 2, _frameHeight / 2));
	_sprite->RenderRotate(renderTarget, drawingPos.x, drawingPos.y, rotMatrix, alpha);
	if (_hasAlpha)
	{
		_alphaSprite->RenderMatrix(renderTarget, drawingPos.x, drawingPos.y, rotMatrix, 1.0f - scale.x);
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

void SpriteObject::SetPositionEndFunction(const Delegate<void> &function)
{
	_hasPositionEndFunction = true;
	_positionEndFunction = function;
}

void SpriteObject::SetRotationEndFunction(const Delegate<void> &function)
{
	_hasRotationEndFunction = true;
	_rotationEndFunction = function;
}

void SpriteObject::SetScaleEndFunction(const Delegate<void> &function)
{
	_hasScaleEndFunction = true;
	_scaleEndFunction = function;
}

void SpriteObject::SetDisappearEndFunction(const Delegate<void>& function)
{
	_hasDisappearEndFunction = true;
	_disappearEndFunction = function;
}

void SpriteObject::MoveTo(const Vector2 & targetPos, float time)
{
	_startPosition = position;
	_targetPosition = targetPos;
	_positionTimer.ResetAndChangeTargetSecond(time);

	_positionInterpolating = true;
}

void SpriteObject::MoveBy(const Vector2 & deltaPos, float time)
{
	_startPosition = position;
	_targetPosition = position + deltaPos;
	_positionTimer.ResetAndChangeTargetSecond(time);

	_positionInterpolating = true;
}

void SpriteObject::RotateTo(const float targetAngle, float time)
{
	_startDegree = degree;
	_targetDegree = targetAngle;
	_rotationTimer.ResetAndChangeTargetSecond(time);

	_rotationInterpolating = true;
}

void SpriteObject::ScaleTo(const float x, const float y, float time)
{
	_startScale = scale;
	_targetScale = Vector2(x, y);
	_scaleTimer.ResetAndChangeTargetSecond(time);

	_scaleInterpolating = true;
}

void SpriteObject::DisappearTo(const float a, float time)
{
	_startAlpha = alpha;
	_targetAlpha = a;
	_disappearTimer.ResetAndChangeTargetSecond(time);

	_disappearInterpolating = true;
}
