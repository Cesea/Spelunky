#ifndef RANDOM_FUNCTION_H
#define RANDOM_FUNCTION_H

#include "singletonBase.h"
#include <time.h>
#include <stdlib.h>

class randomFunction : public singletonBase<randomFunction>
{
public:
	randomFunction();
	~randomFunction();

	int GetInt(int num);
	int GetFromIntTo(int from, int to);

	float GetFloat();
	float GetFloat(float f);
	float GetFromFloatTo(float from, float to);

protected:

};

#endif