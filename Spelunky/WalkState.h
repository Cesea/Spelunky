#ifndef WALK_STATE_H
#define WALK_STATE_H

#include "State.h"
class Player;

class WalkState : public State<Player>
{
public:
	void OnEnter(Player *object) override;
	State<Player> *Update(Player *object, float deltaTime) override;
	State<Player> *HandleCommand(Player *object, const ControlCommand &command) override;
	State<Player> *HandleFrameEndEvent(Player *actor) override { return nullptr; }
	void OnExit(Player *object);

private :

	bool _wasControlled{ false };
};



#endif