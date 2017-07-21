#ifndef D2D_BITMAP_H
#define D2D_BITMAP_H

#include "stdafx.h"

class D2DImage
{
public:
	D2DImage();
	~D2DImage();

	HRESULT Init();

	void Render(ID2D1HwndRenderTarget *renderTarget, float alpha = 1.0f);
	void Render(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float alpha = 1.0f);
	void Render(ID2D1HwndRenderTarget *renderTarget, float sourX, float sourY, float sourWidth, float sourHeight, float alpha = 1.0f);
	void Render(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float sourX, float sourY, float sourWidth, float sourHeight, float alpha = 1.0f);

	//for squashed render
	void Render(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float destWidth, float destHeight,
					float sourX, float sourY, float sourWidth, float sourHeight, float alpha = 1.0f);

	//여기서의 Matrix인자는 Rotation, Scale Matrix이다
	void RenderMatrix(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float sourX, float sourY, 
		float sourWidth, float sourHeight, const D2D1::Matrix3x2F &mat, float alpha = 1.0f);

	void RenderFlipX(ID2D1HwndRenderTarget *renderTarget, float alpha = 1.0f);
	void RenderFlipX(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float alpha = 1.0f);
	void RenderFlipX(ID2D1HwndRenderTarget *renderTarget, float sourX, float sourY, float sourWidth, float sourHeight, float alpha = 1.0f);
	void RenderFlipX(ID2D1HwndRenderTarget *renderTarget, float destX, float destY, float sourX, float sourY, float sourWidth, float sourHeight, float alpha = 1.0f);

	void Release();

	inline ID2D1Bitmap **GetBitmap() { return &_bitmap; }

	//좌표 x
	inline void SetX(float x) { _x = x; }
	inline float GetX(void) { return _x; }

	//좌표 y
	inline void SetY(float y) { _y = y; }
	inline float GetY(void) { return _y; }

	inline void SetWidth(float w) { _width = w; }
	inline void SetHeight(float h) { _height = h; }

	inline float GetWidth() { return _width; }
	inline float GetHeight() { return _height; }

	const std::wstring &GetName() { return _name; }
	void SetName(const std::wstring &name) { _name = name; }

private:
	ID2D1Bitmap *_bitmap{ nullptr };

	std::wstring _name;
	float _x{ 0 };
	float _y{ 0 };
	float _width{ 0 };
	float _height{ 0 };

};


#endif