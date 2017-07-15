#ifndef STATE_H
#define STATE_H

#include "Command.h"

template <typename T>
class State
{
public:
	virtual void OnEnter(T *actor) = 0;
	virtual State *Update(T *actor, float deltaTime) = 0;
	virtual State *HandleCommand(T *actor, const ControlCommand &command)  = 0;
	virtual State *HandleFrameEndEvent(T *actor) = 0;
	virtual void OnExit(T *actor) = 0;
};

#endif