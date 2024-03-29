#ifndef GAME_COMMON_MACRO_FUNCTIONS_H
#define GAME_COMMON_MACRO_FUNCTIONS_H

#include "GameCommonStructs.h"

const char *ConvertEntityStateEnumToString(EntityState state);

inline Rect operator+ (const Rect &rect, const TilePosition &tilePos)
{
	Vector2 untiledPosition = tilePos.UnTilelize();
	Rect result = rect + untiledPosition;
	return result;
}
inline Rect operator+ (const TilePosition &tilePos, const Rect &rect)
{
	Vector2 untiledPosition = tilePos.UnTilelize();
	Rect result = rect + untiledPosition;
	return result;
}

inline Rect operator- (const Rect &rect, const TilePosition &tilePos)
{
	Vector2 untiledPosition = tilePos.UnTilelize();
	Rect result = rect - untiledPosition;
	return result;
}

inline Rect operator- (const TilePosition &tilePos, const Rect &rect)
{
	Vector2 untiledPosition = tilePos.UnTilelize();
	Rect result = rect - untiledPosition;
	return result;
}

//inline Rect

void DrawBox(ID2D1HwndRenderTarget *renderTarget, float x, float y, float width, float height, const D2D1_COLOR_F &color, float strokeWidth = 1.0f);
void FillBox(ID2D1HwndRenderTarget *renderTarget, float x, float y, float width, float height, const D2D1_COLOR_F &color);
void DrawBoxFromCenter(ID2D1HwndRenderTarget *renderTarget, const Rect &rect, const D2D1_COLOR_F & color);

void DrawLine(ID2D1HwndRenderTarget *renderTarget, float startX, float startY, float endX, float endY, 
						const D2D1_COLOR_F &color, float strokeWidth = 1.0f);

void DrawCircleInterpolated(ID2D1HwndRenderTarget * renderTarget, 
	const ScreenVector2 & start, const ScreenVector2 & end, float t, ID2D1SolidColorBrush *brush);

//Vector2 UnTilelizePosition(const TilePosition &position);


#endif
