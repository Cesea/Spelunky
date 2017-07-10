#include "Vector3.h"
#include "stdafx.h"


Vector3 & Vector3::operator=(const Vector3 & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

bool Vector3::operator==(const Vector3 & v)
{
	bool result = false;
	if (x == v.x && y == v.y && z == v.z)
	{
		result = true;
	}
	return result;
}

Vector3 Vector3::operator-()
{
	Vector3 result = *this;
	result.x = -result.x;
	result.y = -result.y;
	result.z = -result.z;
	return result;
}

Vector3& Vector3::operator += (const Vector3& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}
Vector3& Vector3::operator -= (const Vector3& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

Vector3& Vector3::operator *= (const float d)
{
	x *= d; y *= d; z *= d;
	return *this;
}
Vector3& Vector3::operator /= (const float d)
{
	x /= d; y /= d; z *= d;
	return *this;
}

//Length function
float Vector3::Length()
{
	return sqrt(x*x + y*y + z*z);
}
//Length squared function
float Vector3::LengthSquared()
{
	return x*x + y*y + z*z;
}

void Vector3::Normalize()
{
	float k = 1.0f / sqrt(x*x + y*y + z*z);
	x *= k; y *= k; z *= k;
}