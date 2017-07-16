#ifndef LOOK_REVERT_STATE_H
#define LOOK_REVERT_STATE_H

#include "State.h"

class Player;

class LookRevertState : public State<Player>
{
public:
	void OnEnter(Player *object);
	State<Player> *Update(Player *object, float deltaTime);
	State<Player> *HandleCommand(Player *object, const ControlCommand &command);
	State<Player> *HandleFrameEndEvent(Player *actor) override;
	void OnExit(Player *object);
};


#endif
