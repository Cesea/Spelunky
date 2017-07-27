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
	virtual HRESULT Init(D2DImage *sourceImage, float widthPerFrame, float heightPerFrame, const IntVector2 &anchor) { return S_OK; }
	virtual HRESULT Init(D2DImage * sourceImage, Animation *animation, const IntVector2 &anchor) { return S_OK; }

	//virtual HRESULT InitCopy(D2DSprite *sourceSprite);

	virtual void Update(float deltaTime);

	virtual void Render(ID2D1HwndRenderTarget *renderTarget, float alpha = 1.0f);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float alpha = 1.0f);
	virtual void FrameRender(ID2D1HwndRenderTarget *renderTarget, int xIndex, int yIndex, float alpha = 1.0f) {}
	virtual void FrameRender(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, int xIndex, int yIndex, float alpha = 1.0f) {}

	virtual void FrameRenderFlip(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, int xIndex, int yIndex, float alpha = 1.0f) {}

	virtual void RenderMatrix(ID2D1HwndRenderTarget *renderTarget, float destX, float destY,
		const D2D1::Matrix3x2F& mat, float alpha = 1.0f);
	virtual void RenderRotate(ID2D1HwndRenderTarget *renderTarget, float destX, float destY,
		const D2D1::Matrix3x2F& mat, float alpha = 1.0f);
	virtual void FrameRenderMatrix(ID2D1HwndRenderTarget *renderTarget, float destX, float destY,
		int xIndex, int yIndex, const D2D1::Matrix3x2F &mat, float alpha = 1.0f);

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

	void SyncFlip(Direction direction);

	virtual inline bool IsEnd() { return false; }

	virtual IntVector2 GetCurrentAnchor() { return _anchor; }
	void SetCurrentAnchor(const IntVector2 &v) { _anchor = v; }

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
