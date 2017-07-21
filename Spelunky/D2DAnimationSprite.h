#ifndef D2D_ANIMATION_SPRITE_H
#define D2D_ANIMATION_SPRITE_H

#include "Timer.h"
#include "D2DSprite.h"


#include "Animation.h"
#include "Delegate.h"

class D2DAnimationSprite : public D2DSprite
{
public:
	D2DAnimationSprite();
	virtual ~D2DAnimationSprite();

	virtual HRESULT Init(D2DImage * sourceImage, Animation *animation, const IntVector2 &anchor);
	//virtual HRESULT InitCopy(D2DAnimationSprite *sourceSprite);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float alpha = 1.0f);
	virtual void RenderMatrix(ID2D1HwndRenderTarget *renderTarget, float destX, float destY,
		const D2D1::Matrix3x2F& mat, float alpha = 1.0f);

	virtual Animation *GetAnimation() { return _animation; }

	//void RenderFromCenter(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float alpha = 1.0f);
	//void RenderFromCenter(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float destWidth, float destHeight, float alpha = 1.0f);

	virtual void Release();

	virtual IntVector2 GetCurrentAnchor() { return _anchor; }

protected:
	Animation *_animation{};
};



#endif
