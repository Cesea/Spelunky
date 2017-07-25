#ifndef STRONG_SNAKE_SPIT_STATE_H
#define STRONG_SNAKE_ATTACK_SPIT_STATE_H

#include "State.h"

class StrongSnake;

class StrongSnakeSpitState : public State<StrongSnake>
{
public:
	void OnEnter(StrongSnake *object);
	State<StrongSnake> *Update(StrongSnake *object, float deltaTime);
	State<StrongSnake> *HandleCommand(StrongSnake *object, const ControlCommand &command);
	State<StrongSnake> *HandleFrameEndEvent(StrongSnake *actor) override;
	void OnExit(StrongSnake *object);

private:

};
#endif
