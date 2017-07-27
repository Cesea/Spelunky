#ifndef ON_LEDGE_STATE_H
#define ON_LEDGE_STATE_H

#include "State.h"

class Player;

class OnLedgeState : public State<Player>
{
public :
	void OnEnter(Player *object);
	State<Player> *Update(Player *object, float deltaTime);
	State<Player> *HandleCommand(Player *object, const ControlCommand &command);
	State<Player> *HandleFrameEndEvent(Player *actor) override;
	void OnExit(Player *object);
private :
	bool _wasControlled{false};

	TilePosition _enteredPosition;
};

class LedgeGrabState : public State<Player>
{
public :
	void OnEnter(Player *object);
	State<Player> *Update(Player *object, float deltaTime);
	State<Player> *HandleCommand(Player *object, const ControlCommand &command);
	State<Player> *HandleFrameEndEvent(Player *actor) override { return nullptr; }
	void OnExit(Player *object);
private :
	TilePosition _startPosition;
	TilePosition _endPosition;

	Timer _timer;

	bool _stop{ false };
};


#endif