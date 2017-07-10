#ifndef VECTORH
#define VECTORH

#include <math.h>

/*
	Basic Vector3 class
*/
class Vector3
{
public :
	float x, y, z;

// Constructors
	Vector3()
		:x(), y(), z()
	{}

	Vector3(float a)
		: x(a), y(a), z(a)
	{}

	Vector3(float x_, float y_, float z_)
		:x(x_), y(y_), z(z_)
	{}

	Vector3 &operator = (const Vector3 &v);
	bool operator== (const Vector3 &v);


	Vector3 operator-();

	Vector3& operator += (const Vector3& v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator *= (const float d);
	Vector3& operator /= (const float d);

	

	float Length();
	float LengthSquared();
	void Normalize();

}; //Vector3

inline Vector3 operator + (const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
inline Vector3 operator - (const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
inline Vector3 operator * (const Vector3& v1, const float d)
{
	return Vector3(v1.x * d, v1.y * d, v1.z * d);
}
inline Vector3 operator * (const float d, const Vector3& v1)
{
	return Vector3(v1.x * d, v1.y * d, v1.z * d);
}
inline Vector3 operator / (const Vector3& v1, const float d)
{
return Vector3(v1.x / d, v1.y / d, v1.z / d);
}

inline Vector3 Normalize(Vector3 v)
{
	return v / v.Length();
}

//Dot product
inline float Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//Cross product
inline Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.y*v2.z - v1.z*v2.y,
					v1.z*v2.x - v1.x*v2.z,
					v1.x*v2.y - v1.y*v2.x);
}

inline bool operator == (const Vector3& v1, const Vector3& v2)
{
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
		return true;
	else
		return false;
}

//static Vector3 getReflect(const Vector3& incident, const Vector3& normal)
//{
//	Vector3 retVec;
//	retVec = 2 * dot(-incident, normal) * normal + incident;
//	return retVec;
//}
//
//static bool getRefract(const Vector3& incident, const Vector3& normal, float index, Vector3& refracted)
//{
//	Vector3 ui = normalize(incident);
//	float IDotN = dot(-ui, normal);
//	float n1_over_n2 = index;
//	float discriminant = 1.0 - n1_over_n2 * n1_over_n2 * (1 - IDotN * IDotN);
//	if (discriminant > 0)
//	{
//		refracted = n1_over_n2 * (incident + normal * IDotN) - normal * sqrt(discriminant);
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//static void hughes_moeller(const Vector3& normal, Vector3& b1, Vector3& b2)
//{
//	if (fabs(normal.x) > fabs(normal.z))
//	{
//		b2 = Vector3(-normal.y, normal.x, 0.0);
//	}
//	else
//	{
//		b2 = Vector3(0.0, -normal.z, normal.y);
//	}
//	normalize(b2);
//	b1 = cross(b2, normal);
//}
//
#endif