#ifndef SNAKE_ATTACK_STATE_H
#define SNAKE_ATTACK_STATE_H

#include "State.h"

class Snake;

class SnakeAttackState : public State<Snake>
{
public :
	void OnEnter(Snake *object);
	State<Snake> *Update(Snake *object, float deltaTime);
	State<Snake> *HandleCommand(Snake *object, const ControlCommand &command);
	State<Snake> *HandleFrameEndEvent(Snake *actor) override;
	void OnExit(Snake *object);

};


#endif
