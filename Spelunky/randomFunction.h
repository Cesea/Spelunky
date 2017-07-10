#pragma once

#include "singletonBase.h"
#include <time.h>

class randomFunction : public singletonBase <randomFunction>
{
public:
	//생성자
	randomFunction(void)
	{
		srand((unsigned int)time(NULL));
	}
	~randomFunction(void)
	{
	}

	//getInt
	int GetInt(int num) { return rand() % num; }
	int GetFromIntTo(int fromNum, int toNum)
	{
		return (rand() % (toNum - fromNum + 1)) + fromNum;
	}

	//getFloat
	float GetFloat() { return (float)rand() / (float)RAND_MAX; } //32767 0.0 ~ 1.0 사이값
	float GetFloat(float num) { return (float)rand() / (float)RAND_MAX * num; }
	float GetFloat(float fromFloat, float toFloat)
	{
		float rnd = ((float)rand() / (float)RAND_MAX);
		return (rnd * (toFloat - fromFloat) + fromFloat);
	}
};