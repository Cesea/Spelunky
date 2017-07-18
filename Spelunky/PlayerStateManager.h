#ifndef PLAYER_STATE_MANAGER_H
#define PLAYER_STATE_MANAGER_H

#include "StateManager.h"

class Player;

class PlayerStateManager : public StateManager<Player>
{
public :
	void Update(float deltaTime) override;
	void HandleCommand(const ControlCommand &command) override;
private :
	State<Player> *_attackState{};
};

#endif