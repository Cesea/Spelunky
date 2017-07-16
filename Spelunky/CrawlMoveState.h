#ifndef CRAWL_MOVE_STATE_H
#define CRAWL_MOVE_STATE_H

#include "State.h"
class Player;

class CrawlMoveState : public State<Player>
{
public:
	void OnEnter(Player *object);
	State<Player> *Update(Player *object, float deltaTime);
	State<Player> *HandleCommand(Player *object, const ControlCommand &command);
	State<Player> *HandleFrameEndEvent(Player *actor) override { return nullptr; }
	void OnExit(Player *object);

private :
	bool _wasControlled{false};
};



#endif