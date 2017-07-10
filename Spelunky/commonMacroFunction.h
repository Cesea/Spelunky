#pragma once

#include <vector>

struct Rect
{
	float x{};
	float y{};
	float width{};
	float height{};
};

inline Rect RectMake(float x, float y, float width, float height)
{
	Rect result{};
	result.x = x;
	result.y = y;
	result.width = width;
	result.height = height;

	return result;
}

inline float MinFloat(float f1, float f2)
{
	float result{};
	if (f1 < f2)
	{
		result = f1;
	}
	else
	{
		result = f2;
	}
	return result;
}
inline float MaxFloat(float f1, float f2)
{
	float result{};
	if (f1 > f2)
	{
		result = f1;
	}
	else
	{
		result = f2;
	}
	return result;
}

inline bool IsRectangleOverlap(Rect rect1, Rect rect2)
{
	bool result = false;
	if ((rect1.x < rect2.x + rect2.width) && (rect1.x + rect1.width > rect2.x) && 
		(rect1.y < rect2.y + rect2.height) && (rect1.y + rect1.height > rect2.y))
	{
		result = true;
	}
	return result;
}

inline bool IsRectangleOverlap(const Rect& rect1, const Rect &rect2, Rect& overlap)
{
	bool result = false;
	if (IsRectangleOverlap(rect1, rect2))
	{
		result = true;
	}

	if (result == true)
	{
		overlap.x = MaxFloat(rect1.x, rect2.x);
		overlap.y = MaxFloat(rect1.y, rect2.y);

		float overlapRight = MinFloat(rect1.x + rect1.width, rect2.x + rect2.width);
		float overlapBottom = MinFloat(rect1.y + rect1.height, rect2.y + rect2.height);

		overlap.width = overlapRight - overlap.x;
		overlap.height = overlapBottom - overlap.y;
	}
	return result;
}

inline bool IsPointInRect(Vector2 p, Rect rect)
{
	bool result = false;
	if ((p.x > rect.x) && (p.x < rect.x + rect.width) && (p.y > rect.y) && (p.y < rect.y + rect.height))
	{
		result = true;
	}
	return result;
}

inline void ClampFloat(float *f, float min, float max)
{
	if (*f < min) { *f = min; }
	if (*f > max) { *f = max; }
}

inline void ClampInt(int *i, int min, int max)
{
	if (*i < min) { *i = min; }
	if (*i > max) { *i = max; }
}

inline float InterpolateFloat(float start, float end, float t)
{
	float result = start;
	float diff = end - start;
	result += diff * t;
	return result;
}

inline ScreenVector2 InterpolateVector(const ScreenVector2 &start, const ScreenVector2 end, float t)
{
	ScreenVector2 result = {};
	result.x = InterpolateFloat(start.x, end.x, t);
	result.y = InterpolateFloat(start.y, end.y, t);
	return result;
}

inline Rect operator+ (const Rect &rect, const Vector2 &v)
{
	return RectMake(rect.x + v.x, rect.y + v.y, rect.width, rect.height);
}
inline Rect operator+ (const Vector2 &v, const Rect &rect)
{
	return RectMake(rect.x + v.x, rect.y + v.y, rect.width, rect.height);
}
inline Rect operator- (const Rect &rect, const Vector2 &v)
{
	return RectMake(rect.x - v.x, rect.y - v.y, rect.width, rect.height);
}
