#include "stdafx.h"
#include "CrawlState.h"

#include "Player.h"

#include "StandUpState.h"

void CrawlState::OnEnter(Player * object)
{
	object->SetGraphics(L"crawl");
}

State<Player>* CrawlState::Update(Player * object, float deltaTime)
{
	State<Player> *newState = nullptr;
	D2DSprite *currentSprite = object->GetCurrentGraphics();
	currentSprite->Update(deltaTime);

	return nullptr;
}

State<Player>* CrawlState::HandleCommand(Player * object, const ControlCommand & command)
{
	State<Player> *newState = nullptr;
	if (command.vertical == Command::None)
	{
		newState = new StandUpState;
	}
	return newState;
}

void CrawlState::OnExit(Player * object)
{
}
