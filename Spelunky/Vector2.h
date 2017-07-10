#ifndef VECTOR2_H
#define VECTOR2_H

#include <math.h>

class Vector2
{
public :
	float x;
	float y;

	Vector2();
	Vector2(float x_, float y_);

	~Vector2();

	Vector2 &operator= (const Vector2 &v);
	bool &operator == (const Vector2 &v);


	Vector2 operator- ();

	Vector2& operator+= (const Vector2& v);
	Vector2& operator-= (const Vector2& v);
	Vector2& operator*= (const float f);
	Vector2& operator/= (const float f);

	float Distance();
	float DistanceSquare();

	void Normalize();

	void Cross();
};

inline Vector2 operator+ (const Vector2& v1, const Vector2& v2)
{
	Vector2 result = { v1.x + v2.x, v1.y + v2.y };
	return result;
}
inline Vector2 operator- (const Vector2& v1, const Vector2& v2)
{
	Vector2 result = { v1.x - v2.x, v1.y - v2.y };
	return result;
}

inline Vector2 operator* (const Vector2& v, const float f)
{
	Vector2 result = { v.x * f, v.y * f };
	return result;
}
inline Vector2 operator* (const float f, const Vector2& v)
{
	Vector2 result = { v.x * f, v.y * f };
	return result;
}

inline Vector2 operator/ (const Vector2& v, const float f)
{
	Vector2 result = { v.x / f, v.y / f };
	return result;
}
inline Vector2 operator/ (const float f, const Vector2& v)
{
	Vector2 result = { v.x / f, v.y / f };
	return result;
}

inline Vector2 Normalize(const Vector2	&v)
{
	Vector2 result;
	result.x = v.x;
	result.y = v.y;

	result.Normalize();
	return result;
}

inline Vector2 Cross(const Vector2 & v)
{
	Vector2 result;
	result.x = v.x;
	result.y = v.y;

	float temp = result.y;
	result.y = result.x;
	result.x = -temp;
	return result;
}

inline float Dot(const Vector2& v1, const Vector2& v2)
{
	float result = v1.x * v2.x + v1.y * v2.y;
	return result;
}


#endif
