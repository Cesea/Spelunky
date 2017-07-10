#ifndef D2DSPRITE_H
#define D2DSPRITE_H

#include "D2DImage.h"

class Animation;

class D2DSprite
{
public:
	D2DSprite();
	virtual ~D2DSprite();

	virtual HRESULT Init(D2DImage *sourceImage, float sourceX, float sourceY, float widthPerFrame, float heightPerFrame, const IntVector2 &anchor);
	//virtual HRESULT Init(D2DImage * sourceImage, float time, float sourceX, float sourceY,
	//	int numFrameX, float widthPerFrame, float heightPerFrame, AnimationType type);
	virtual HRESULT Init(D2DImage * sourceImage, Animation *animation, const IntVector2 &anchor);

	//virtual HRESULT InitCopy(D2DSprite *sourceSprite);

	virtual void Update(float deltaTime);

	virtual void Render(ID2D1HwndRenderTarget *renderTarget, float alpha = 1.0f);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float alpha = 1.0f);

	//virtual void RenderFromCenter(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float alpha = 1.0f);
	//virtual void RenderFromCenter(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float destWidth, float destHeight, float alpha = 1.0f);

	virtual void Release();

	inline void SetWidthPerFrame(float width) { _widthPerFrame = width; }
	inline void setHeightPerFrame(float height) { _heightPerFrame = height; }

	inline float GetWidthPerFrame() { return _widthPerFrame; }
	inline float GetHeightPerFrame() { return _heightPerFrame; }

	inline void SetSourceX(int sourceX) { _sourceX = sourceX; }
	inline void SetSourceY(int sourceY) { _sourceY = sourceY; }

	inline float GetSourceX() { return _sourceX; }
	inline float GetSourceY() { return _sourceY; }

	virtual inline bool IsEnd() { return false; }

	virtual IntVector2 GetCurrentAnchor() { return _anchor; }

	virtual Animation *GetAnimation() { return nullptr; }

	virtual inline D2DImage *GetSourceImage()
	{
		return _sourceImage;
	}

	inline void SetFlipX(bool b)
	{
		_flipedX = b;
	}
	inline bool GetFlipX()
	{
		return _flipedX;
	}

	virtual inline Rect GetCurrentRect()
	{
		return RectMake(0, 0, _widthPerFrame, _heightPerFrame);
	}

protected:
	float _sourceX{};
	float _sourceY{};

	float _widthPerFrame{};
	float _heightPerFrame{};

	bool _flipedX{ false };

	D2DImage *_sourceImage{ nullptr };
	IntVector2 _anchor;

private:
};



#endif
