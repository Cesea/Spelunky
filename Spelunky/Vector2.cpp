#include "stdafx.h"
#include "Vector2.h"

Vector2::Vector2()
	:x(0), y(0)
{
}

Vector2::Vector2(float x_, float y_)
	:x(x_), y(y_)
{
}

Vector2::~Vector2()
{
}

Vector2 & Vector2::operator=(const Vector2 & v)
{
	x = v.x;
	y = v.y;
	return *this;
}

bool & Vector2::operator==(const Vector2 & v)
{
	bool result = false;
	if ((x == v.x) && (y == v.y))
	{
		result = true;
	}
	return result;
}

Vector2 Vector2::operator-()
{
	Vector2 result;
	result.x = -x;
	result.y = -y;
	return result;
}

Vector2 & Vector2::operator+=(const Vector2 & v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2 & Vector2::operator-=(const Vector2 & v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2 & Vector2::operator*=(const float f)
{
	x *= f;
	y *= f;
	return *this;
}

Vector2 & Vector2::operator/=(const float f)
{
	x /= f;
	y /= f;
	return *this;
}

float Vector2::Distance()
{
	float result = 0.0f;
	result = sqrt((x * x) + (y * y));
	return result;
}

float Vector2::DistanceSquare()
{
	float result = 0.0f;
	result = (x * x) + (y * y);
	return result;
}

void Vector2::Normalize()
{
	float k = 1.0f / Distance();
	x *= k;
	y *= k;
}

void Vector2::Cross()
{
}
