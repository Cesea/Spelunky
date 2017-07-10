#ifndef INT_VECTOR_H
#define INT_VECTOR_H

#include <math.h>

class IntVector2
{
public:
	int x;
	int y;

	IntVector2();
	IntVector2(int x_, int y_);
	~IntVector2();

	bool operator== (const IntVector2 &v);

	IntVector2& operator= (const IntVector2& v);
	IntVector2& operator- ();

	IntVector2& operator+= (const IntVector2& v);
	IntVector2& operator-= (const IntVector2& v);
	IntVector2& operator*= (const int i);

	float Distance();
	float DistanceSquare();

};

inline IntVector2 operator+ (const IntVector2& v1, const IntVector2& v2)
{
	IntVector2 result = { v1.x + v2.x, v1.y + v2.y };
	return result;
}
inline IntVector2 operator- (const IntVector2& v1, const IntVector2& v2)
{
	IntVector2 result = { v1.x - v2.x, v1.y - v2.y };
	return result;
}

inline IntVector2 operator* (const IntVector2& v, const int i)
{
	IntVector2 result = { v.x * i, v.y * i };
	return result;
}
inline IntVector2 operator* (const int i, const IntVector2& v)
{
	IntVector2 result = { v.x * i, v.y * i };
	return result;
}

#endif
