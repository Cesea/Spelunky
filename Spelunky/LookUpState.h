#ifndef LOOK_UP_STATE_H
#define LOOK_UP_STATE_H

#include "State.h"
class Player;

class LookUpState : public State<Player>
{
public :
	void OnEnter(Player *object);
	State<Player> *Update(Player *object, float deltaTime);
	State<Player> *HandleCommand(Player *object, const ControlCommand &command);
	State<Player> *HandleFrameEndEvent(Player *actor) override { return nullptr; }
	void OnExit(Player *object);
};


#endif
