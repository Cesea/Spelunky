#ifndef CRAWL_STATE_H
#define CRAWL_STATE_H

#include "State.h"
class Player;

class CrawlState : public State<Player>
{
public :
	void OnEnter(Player *object);
	State<Player> *Update(Player *object, float deltaTime);
	State<Player> *HandleCommand(Player *object, const ControlCommand &command);
	State<Player> *HandleFrameEndEvent(Player *actor) override { return nullptr; }
	void OnExit(Player *object);
};



#endif
