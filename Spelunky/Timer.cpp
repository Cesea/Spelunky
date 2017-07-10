#include "stdafx.h"
#include "Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

HRESULT Timer::Init(float targetSecond)
{
	_targetSecond = targetSecond;
	_currentSecond = 0.0f;

	return S_OK;
}

bool Timer::Tick(float deltaSecond)
{
	bool result = false;
	_currentSecond += deltaSecond;
	if (_currentSecond >= _targetSecond)
	{
		result = true;
		_currentSecond = _currentSecond - _targetSecond;
	}
	return result;
}

void Timer::Reset()
{
	_currentSecond = 0.0f;
}

void Timer::ResetAndChangeTargetSecond(float targetSecond)
{
	Reset();
	_targetSecond = targetSecond;
}

void Timer::MakeAboutToTick()
{
	_currentSecond = _targetSecond - 0.001f;
}
