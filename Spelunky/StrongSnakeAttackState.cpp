#include "stdafx.h"
#include "StrongSnakeAttackState.h"

#include "StrongSnake.h"

#include "StrongSnakeWalkState.h"


void StrongSnakeAttackState::OnEnter(StrongSnake * object)
{
	object->SetGraphics(L"attack");
	object->_attacking = true;
}

State<StrongSnake>* StrongSnakeAttackState::Update(StrongSnake * object, float deltaTime)
{
	State<StrongSnake> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	if (!currentSprite->GetAnimation()->IsPlay())
	{
		newState = new StrongSnakeWalkState();
	}
	return newState;

}

State<StrongSnake>* StrongSnakeAttackState::HandleCommand(StrongSnake * object, const ControlCommand & command)
{
	return nullptr;
}

State<StrongSnake>* StrongSnakeAttackState::HandleFrameEndEvent(StrongSnake * actor)
{
	return nullptr;
}

void StrongSnakeAttackState::OnExit(StrongSnake * object)
{
}
