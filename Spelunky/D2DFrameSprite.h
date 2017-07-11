#ifndef D2D_FRAME_SPRITE_H
#define D2D_FRAME_SPRITE_H

#include "D2DSprite.h"

class D2DFrameSprite : public D2DSprite
{
public:
	D2DFrameSprite();
	virtual ~D2DFrameSprite();

	virtual HRESULT Init(D2DImage *sourceImage, float widthPerFrame, float heightPerFrame, const IntVector2 &anchor);

	virtual void Update(float deltaTime);

	virtual void FrameRender(ID2D1HwndRenderTarget *renderTarget, int xIndex, int yIndex, float alpha = 1.0f);
	virtual void FrameRender(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, int xIndex, int yIndex, float alpha = 1.0f);

	//virtual void RenderFromCenter(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float alpha = 1.0f);
	//virtual void RenderFromCenter(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float destWidth, float destHeight, float alpha = 1.0f);

	virtual void Release();

	inline void SetWidthPerFrame(float width) { _widthPerFrame = width; }
	inline void setHeightPerFrame(float height) { _heightPerFrame = height; }

	inline float GetWidthPerFrame() { return _widthPerFrame; }
	inline float GetHeightPerFrame() { return _heightPerFrame; }

	virtual IntVector2 GetCurrentAnchor() { return _anchor; }

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

protected:
	int _imageWidth;
	int _imageHeight;

	int _xCount;
	int _yCount;

	bool _flipedX{ false };


private:
};



#endif