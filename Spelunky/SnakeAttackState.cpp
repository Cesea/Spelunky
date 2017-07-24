#include "stdafx.h"
#include "SnakeAttackState.h"

#include "Snake.h"
#include "SnakeWalkState.h"

void SnakeAttackState::OnEnter(Snake * object)
{
	object->SetGraphics(L"attack");
	object->_attacking = true;
}

State<Snake>* SnakeAttackState::Update(Snake * object, float deltaTime)
{
	State<Snake> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	if (!currentSprite->GetAnimation()->IsPlay())
	{
		newState = new SnakeWalkState();
	}
	return newState;
}

State<Snake>* SnakeAttackState::HandleCommand(Snake * object, const ControlCommand & command)
{
	return nullptr;
}

State<Snake>* SnakeAttackState::HandleFrameEndEvent(Snake * actor)
{
	return nullptr;
}

void SnakeAttackState::OnExit(Snake * object)
{
	object->_attacking = false;
}
