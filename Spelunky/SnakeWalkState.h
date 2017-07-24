#ifndef SNAKE_WALK_STATE_H
#define SNAKE_WALK_STATE_H

#include "State.h"

class Snake;

class SnakeWalkState : public State<Snake>
{
public :
	void OnEnter(Snake *object);
	State<Snake> *Update(Snake *object, float deltaTime);
	State<Snake> *HandleCommand(Snake *object, const ControlCommand &command);
	State<Snake> *HandleFrameEndEvent(Snake *actor) override;
	void OnExit(Snake *object);

private :
	bool _wasControlled{false};

};


#endif
