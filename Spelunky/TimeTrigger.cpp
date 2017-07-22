#include "stdafx.h"
#include "TimeTrigger.h"

TimeTrigger::TimeTrigger()
{
}

TimeTrigger::~TimeTrigger()
{
}

void TimeTrigger::Init(float time, const Delegate<void> &activateFunction)
{
	_timer.Init(time);
	_activateFunction = activateFunction;
}

bool TimeTrigger::Update(float deltaTime)
{
	bool result = false;
	if (_valid)
	{
		if (_timer.Tick(deltaTime))
		{
			_activateFunction();
			result = true;
			_valid = false;
		}
	}
	return result;
}

