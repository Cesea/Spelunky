#include "stdafx.h"
#include "IntVector2.h"

IntVector2::IntVector2()
	:x(0), y(0)
{
}

IntVector2::IntVector2(int x_, int y_)
	:x(x_), y(y_)
{
}

IntVector2::~IntVector2()
{
}

bool IntVector2::operator==(const IntVector2 & v)
{
	bool result = false;
	if ((x == v.x) && (y == v.y))
	{
		result = true;
	}
	return true;
}

IntVector2 &IntVector2::operator=(const IntVector2 & v)
{
	x = v.x;
	y = v.y;
	return *this;
}

IntVector2 & IntVector2::operator-()
{
	x = -x;
	y = -y;
	return *this;
}

IntVector2 & IntVector2::operator+=(const IntVector2 & v)
{
	x += v.x;
	y += v.y;
	return *this;
}

IntVector2 & IntVector2::operator-=(const IntVector2 & v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

IntVector2 & IntVector2::operator*=(const int i)
{
	x *= i;
	y *= i;
	return *this;
}



float IntVector2::Distance()
{
	return sqrt((float)x * (float)x + (float)y * (float)y);
}

float IntVector2::DistanceSquare()
{
	return ((float)x *(float) x + (float)y * (float)y);
}
