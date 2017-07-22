#ifndef TIME_TRIGGER_H
#define TIME_TRIGGER_H

#include "Delegate.h"
#include "Timer.h"

class TimeTrigger
{
public:
	TimeTrigger();
	virtual ~TimeTrigger();
	void Init(float time, const Delegate<void> &activateFunction);

	bool Update(float deltaTime);

	bool GetValid() { return _valid; }
	void SetValid(bool b) { _valid = b; }
private:
	Delegate<void> _activateFunction;

	Timer _timer;

	bool _valid{ false };

};

#endif