#include "stdafx.h"
#include "BatOnTopState.h"

#include "Bat.h"

void BatOnTopState::OnEnter(Bat * object)
{
	object->SetGraphics(L"on_top");
}

State<Bat>* BatOnTopState::Update(Bat * object, float deltaTime)
{
	State<Bat> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	return nullptr;
}

State<Bat>* BatOnTopState::HandleCommand(Bat * object, const ControlCommand & command)
{
	return nullptr;
}

State<Bat>* BatOnTopState::HandleFrameEndEvent(Bat * actor)
{
	return nullptr;
}

void BatOnTopState::OnExit(Bat * object)
{
}
