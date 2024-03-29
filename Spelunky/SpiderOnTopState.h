#ifndef SPIDER_ON_TOP_STATE_H
#define SPIDER_ON_TOP_STATE_H

#include "State.h"

class Spider;

class SpiderOnTopState : public State<Spider>
{
public :
	void OnEnter(Spider *object);
	State<Spider> *Update(Spider *object, float deltaTime);
	State<Spider> *HandleCommand(Spider *object, const ControlCommand &command);
	State<Spider> *HandleFrameEndEvent(Spider *actor) override;
	void OnExit(Spider *object);

private :

};


#endif