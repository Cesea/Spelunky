#ifndef STATE_H
#define STATE_H

#include "Command.h"

template <typename T>
class State
{
public:
	virtual void OnEnter(T *actor) = 0;
	virtual State *Update(T *actor, float deltaTime, Command command) = 0;
	virtual void OnExit(T *actor) = 0;
};

#endif