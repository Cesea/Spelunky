#ifndef BAT_ON_FLYING_STATE_H
#define BAT_ON_FLYING_STATE_H

#include "State.h"

class Bat;

class BatFlyingState : public State<Bat>
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