#ifndef BAT_ON_TOP_STATE_H
#define BAT_ON_TOP_STATE_H

#include "State.h"

class Bat;

class BatOnTopState : public State<Bat>
{
public:
	void OnEnter(Bat *object);
	State<Bat> *Update(Bat *object, float deltaTime);
	State<Bat> *HandleCommand(Bat *object, const ControlCommand &command);
	State<Bat> *HandleFrameEndEvent(Bat *actor) override;
	void OnExit(Bat *object);

private:
	bool _wasControlled{ false };

};


#endif