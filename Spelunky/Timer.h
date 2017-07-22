#ifndef TIMER_H
#define TIMER_H

#include "stdafx.h"

class Timer
{
public :

	Timer();
	~Timer();

	HRESULT Init(float targetSecond);

	bool Tick(float deltaSecond);
	void Reset();
	void ResetAndChangeTargetSecond(float targetSecond);
	void MakeAboutToTick();

	inline float GetCurrentSecond()
	{
		return _currentSecond;
	}
	inline float GetTargetSecond()
	{
		return _targetSecond;
	}
private :

	float _currentSecond;
	float _targetSecond;
};
#endif