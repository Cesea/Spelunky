#ifndef SPIDER_JUMP_STATE_H
#define SPIDER_JUMP_STATE_H

#include "State.h"

class Spider;

class SpiderJumpState : public State<Spider>
{
public:
	void OnEnter(Spider *object);
	State<Spider> *Update(Spider *object, float deltaTime);
	State<Spider> *HandleCommand(Spider *object, const ControlCommand &command);
	State<Spider> *HandleFrameEndEvent(Spider *actor) override;
	void OnExit(Spider *object);

private:
};


#endif