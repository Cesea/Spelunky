#include "RandomFunctions.h"

randomFunction::randomFunction()
{
	srand(time(NULL));
}

randomFunction::~randomFunction()
{
}

int randomFunction::GetInt(int num)
{
	int result = rand() % num;
	return result;
}

int randomFunction::GetFromIntTo(int from, int to)
{
	int result = (rand() % (to - from + 1)) + from;
	return result;
}

float randomFunction::GetFloat()
{
	float result = (float)rand() / (float)RAND_MAX;
	return result;
}

float randomFunction::GetFloat(float f)
{
	float result = GetFloat() * f;
	return result;
}

float randomFunction::GetFromFloatTo(float from, float to)
{
	float difference = to - from;
	float result = GetFloat(difference) + from;
	return result;
}