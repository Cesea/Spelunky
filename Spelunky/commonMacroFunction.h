#pragma once

#include <vector>


inline float MinFloat(float f1, float f2)
{
	float result{};
	if (f1 < f2) { result = f1; }
	else { result = f2; }
	return result;
}
inline float MaxFloat(float f1, float f2)
{
	float result{};
	if (f1 > f2) { result = f1; }
	else { result = f2; }
	return result;
}
inline int MinInt(int i1, int i2)
{
	int result{};
	if (i1 < i2) { result = i1; }
	else { result = i2; }
	return result;
}
inline int MaxInt(int i1, int i2)
{
	int result{};
	if (i1 > i2) { result = i1; }
	else { result = i2; }
	return result;
}

struct IntRect
{
	int x{};
	int y{};
	int width{};
	int height{};
};

inline IntRect IntRectMake(int x, int y, int width, int height)
{
	IntRect result;
	result.x = x;
	result.y = y;
	result.width = width;
	result.height = height;
	return result;
}

inline IntRect IntRectMakeWidthCorners(const IntVector2 &p1, const IntVector2 &p2)
{
	IntRect result;
	result.x = MinInt(p1.x, p2.x);
	result.y = MinInt(p1.y, p2.y);

	int maxX = MaxInt(p1.x, p2.x);
	int maxY = MaxInt(p1.y, p2.y);
	result.width = maxX - result.x;
	result.height = maxY - result.y;
	return result;
}

struct Rect
{
	float x{};
	float y{};
	float width{};
	float height{};

	void operator+= (const Vector2 &v)
	{
		x += v.x;
		y += v.y;
	}
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

inline bool IsRectangleOverlap(const Rect &rect1, const Rect &rect2)
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

inline bool ClampFloat(float *f, float min, float max)
{
	bool result = false;
	if (*f < min) { *f = min; }
	if (*f > max) { *f = max; }
	return result;
}

inline bool ClampInt(int *i, int min, int max)
{
	bool result = false;
	if (*i < min) { *i = min; }
	if (*i > max) { *i = max; }
	return result;
}

inline float InterpolateFloat(float start, float end, float t)
{
	float result = start;
	float diff = end - start;
	result += diff * t;
	return result;
}

inline Vector2 InterpolateVector(const Vector2 &start, const Vector2 &end, float t)
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

inline int ConvertFloatToInt(float f)
{
	int result{};
	result = (int)(f + 0.5f);
	return result;
}

inline int GetIndexFromXY(int x, int y, int xCount)
{
	int result = x + (y * xCount);
	return result;
}

inline float ToRadians(float degrees)
{
	float result = 0;
	result = degrees * (M_PI / 180.0f);
	return result;
}

inline float ToDegrees(float radians)
{
	float result = 0;
	result = radians * (180 / M_PI);

	return result;
}

inline float GetAngle(float startX, float startY, float endX, float endY)
{
	float x = endX - startX;
	float y = endY - startY;

	float d = sqrt(x * x + y * y);

	float angle = acos(x / d);
	if (y > 0) angle = M_PI2 - angle;
	return angle;
}
