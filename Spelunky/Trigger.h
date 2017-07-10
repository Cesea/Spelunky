#ifndef TRIGGER_H
#define TRIGGER_H

#include "Delegate.h"

class Trigger
{
public:
	Trigger();
	virtual ~Trigger();
	void Init(float x, Delegate<void, float> activateFunction);

	void Activate();

	bool GetValid() { return _valid; }

	float xPos;
private:
	Delegate<void, float> _activateFunction;

	bool _valid{ true };

};


#endif