#ifndef FAINT_STATE_H
#define FAINT_STATE_H

#include "State.h"
class Player;

class FaintState : public State<Player>
{
public :
	void OnEnter(Player *object);
	State<Player> *Update(Player *object, float deltaTime);
	State<Player> *HandleCommand(Player *object, const ControlCommand &command);
	State<Player> *HandleFrameEndEvent(Player *actor) override { return nullptr; }
	void OnExit(Player *object);

private :
	Timer _recoverTimer;
	bool _recoverTimerOn{ false };

	Timer _onGroundTimer;
};


#endif