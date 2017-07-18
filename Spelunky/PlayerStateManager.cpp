#include "stdafx.h"
#include "PlayerStateManager.h"

#include "Player.h"

#include "AttackState.h"

void PlayerStateManager::Update(float deltaTime)
{
	State<Player> *newState = _currentState->Update(_pActor, deltaTime);
	if (newState != nullptr)
	{
		ChangeState(newState);
	}
}

void PlayerStateManager::HandleCommand(const ControlCommand & command)
{
	if (command.action != Command::Attack)
	{
		State<Player> *newState = _currentState->HandleCommand(_pActor, command);
		if (newState != nullptr)
		{
			ChangeState(newState);
		}
	}
	else
	{
		if (_pActor->_holding)
		{
			_attackState = new AttackState;
		}
		else
		{
			_attackState = new ThrowState;
		}
		ChangeState(_attackState);
	}
}
