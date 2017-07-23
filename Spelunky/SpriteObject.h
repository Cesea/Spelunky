#ifndef SPRITE_OBJECT_H
#define SPRITE_OBJECT_H

#include "gameNode.h"

#include "D2DSprite.h"
#include "D2DAnimationSprite.h"

#include "Delegate.h"


class SpriteObject : public gameNode
{
public :
	SpriteObject();
	virtual ~SpriteObject();

	virtual HRESULT Init(const std::wstring &key, int sourceX, int sourceY, int frameWidth, int frameHeight, const IntVector2 &anchor);
	virtual HRESULT InitAnimation(const std::wstring &key,Animation *animation, const IntVector2 &anchor);
	virtual HRESULT InitFrame(const std::wstring &key,int frameWidth, int frameHeight, const IntVector2 &anchor);
	virtual void Release(void);
	virtual void Update(float deltaTime);

	void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);
	void Render(ID2D1HwndRenderTarget *renderTarget, int sourceX, int sourceY, const Vector2 &camPos);

	void RenderScale(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);
	void RenderRotate(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	HRESULT AlphaInit(const std::wstring &key, int sourceX, int sourceY, int frameWidth, int frameHeight, const IntVector2 &anchor);
	HRESULT AlphaInitAnimation(const std::wstring &key,Animation *animation, const IntVector2 &anchor);
	HRESULT AlphaInitFrame(const std::wstring &key,int frameWidth, int frameHeight, const IntVector2 &anchor);

	void SetPositionEndFunction(const Delegate<void> &function);
	void SetRotationEndFunction(const Delegate<void> &function);
	void SetScaleEndFunction(const Delegate<void> &function);
	void SetDisappearEndFunction(const Delegate<void> &function);

	D2DSprite *GetSprite() { return _sprite; }

	Vector2 position;
	Vector2 scale{1, 1};
	float degree{};
	float alpha{ 1.0f };

	void MoveTo(const Vector2 &targetPos, float time);
	void MoveBy(const Vector2 &deltaPos, float time);
	void RotateTo(const float targetAngle, float time);
	void ScaleTo(const float x, const float y, float time);
	void DisappearTo(const float a, float time);

	void SetFlip(bool b) { _sprite->SetFlipX(b); }

	bool32 GetPositionInterpolating() { return _positionInterpolating; }

protected :
	
	D2DSprite *_sprite{};

	bool _hasAlpha{ false };
	D2DSprite *_alphaSprite{};

	Timer _positionTimer;
	Timer _rotationTimer;
	Timer _scaleTimer;
	Timer _disappearTimer;

	Vector2 _startPosition;
	Vector2 _targetPosition;

	float _startDegree;
	float _targetDegree;

	Vector2 _startScale;
	Vector2 _targetScale;

	float _startAlpha;
	float _targetAlpha;


	int _frameWidth{};
	int _frameHeight{};

	bool32 _positionInterpolating{false};
	bool32 _rotationInterpolating{false};
	bool32 _scaleInterpolating{false};
	bool32 _disappearInterpolating{ false };

	bool32 _hasPositionEndFunction{ false };
	bool32 _hasRotationEndFunction{ false };
	bool32 _hasScaleEndFunction{ false };
	bool32 _hasDisappearEndFunction{ false };
	Delegate<void> _positionEndFunction;
	Delegate<void> _rotationEndFunction;
	Delegate<void> _scaleEndFunction;
	Delegate<void> _disappearEndFunction;

	bool _flip{ false };

};

#endif
