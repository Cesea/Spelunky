#ifndef FALLING_STATE_H
#define FALLING_STATE_H

#include "State.h"
class Player;

class FallingState : public State<Player>
{
public:
	void OnEnter(Player *object);
	State<Player> *Update(Player *object, float deltaTime);
	State<Player> *HandleCommand(Player *object, const ControlCommand &command);
	State<Player> *HandleFrameEndEvent(Player *actor) override { return nullptr; }
	void OnExit(Player *object);

private :
	bool _wasControlled{ true };
};



#endif