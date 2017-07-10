#include "stdafx.h"
#include "Trigger.h"

Trigger::Trigger()
{
}

Trigger::~Trigger()
{
}

void Trigger::Init(float x, Delegate<void, float> activateFunction)
{
	xPos = x;
	_activateFunction = activateFunction;
}

void Trigger::Activate()
{
	if (_valid)
	{
		_activateFunction(xPos);
		_valid = false;
	}
}
