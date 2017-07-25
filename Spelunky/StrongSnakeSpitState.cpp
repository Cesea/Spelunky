#include "stdafx.h"
#include "StrongSnakeSpitState.h"

#include "StrongSnake.h"

void StrongSnakeSpitState::OnEnter(StrongSnake * object)
{
}

State<StrongSnake>* StrongSnakeSpitState::Update(StrongSnake * object, float deltaTime)
{
	return nullptr;
}

State<StrongSnake>* StrongSnakeSpitState::HandleCommand(StrongSnake * object, const ControlCommand & command)
{
	return nullptr;
}

State<StrongSnake>* StrongSnakeSpitState::HandleFrameEndEvent(StrongSnake * actor)
{
	return nullptr;
}

void StrongSnakeSpitState::OnExit(StrongSnake * object)
{
}
