#ifndef LADDER_CLIMB_STATE_H
#define LADDER_CLIMB_STATE_H

#include "State.h"
class Player;

class LadderIdleState : public State<Player>
{
public:
	void OnEnter(Player *object);
	State<Player> *Update(Player *object, float deltaTime);
	State<Player> *HandleCommand(Player *object, const ControlCommand &command);
	State<Player> *HandleFrameEndEvent(Player *actor) override { return nullptr; }
	void OnExit(Player *object);

private:
};


class LadderClimbState : public State<Player>
{
public:
	void OnEnter(Player *object);
	State<Player> *Update(Player *object, float deltaTime);
	State<Player> *HandleCommand(Player *object, const ControlCommand &command);
	State<Player> *HandleFrameEndEvent(Player *actor) override { return nullptr; }
	void OnExit(Player *object);

private:
	bool _wasControlled{ true };
};



#endif