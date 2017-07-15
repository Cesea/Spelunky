#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "Command.h"
#include "State.h"

template <typename T>
class StateManager
{
public:
	StateManager() {}
	virtual ~StateManager() {}

	HRESULT Init(T *actor, State<T> *firstState)
	{
		_pActor = actor;

		_currentState = firstState;
		return S_OK;
	}
	void Release()
	{
		_pActor = nullptr;
		SAFE_DELETE(_currentState);
		SAFE_DELETE(_prevState)
	}
	void Update(float deltaTime)
	{
		State<T> *newState = _currentState->Update(_pActor, deltaTime);
		if (newState != nullptr)
		{
			ChangeState(newState);
		}
	}

	void HandleCommand(const ControlCommand &command)
	{
		State<T> *newState = _currentState->HandleCommand(_pActor, command);
		if (newState != nullptr)
		{
			ChangeState(newState);
		}
	}

	void HandleFrameEndEvent(void)
	{
		State<T> *newState = _currentState->HandleFrameEndEvent(_pActor);
		if (newState != nullptr)
		{
			ChangeState(newState);
		}
	}

	void ChangeState(State<T> *state)
	{
		if (_currentState)
		{
			_currentState->OnExit(_pActor);
			_currentState = nullptr;
		}
		_currentState = state;
		_currentState->OnEnter(_pActor);
	}

private:

	State<T> *_currentState{};
	State<T> *_prevState{};

	T *_pActor{};
};

#endif