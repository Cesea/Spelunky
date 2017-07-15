#ifndef STAND_UP_STATE_H
#define STAND_UP_STATE_H

#include "State.h"
class Player;

class StandUpState : public State<Player>
{
public :
	void OnEnter(Player *object);
	State<Player> *Update(Player *object, float deltaTime);
	State<Player> *HandleCommand(Player *object, const ControlCommand &command);
	State<Player> *HandleFrameEndEvent(Player *actor);
	void OnExit(Player *object);
};



#endif