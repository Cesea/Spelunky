#ifndef SPRITE_OBJECT_H
#define SPRITE_OBJECT_H

#include "gameNode.h"

#include "D2DSprite.h"
#include "D2DAnimationSprite.h"


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
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos, float alpha = 1.0f);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, int sourceX, int sourceY, const Vector2 &camPos, float alpha = 1.0f);

	Vector2 position;

	void MoveTo(const Vector2 &targetPos, float time);

protected :
	bool _interpolating{false};
	D2DSprite *_sprite;

	Timer _timer;

	Vector2 _startPosition;
	Vector2 _targetPosition;

	int _zOrder{};
};

#endif
