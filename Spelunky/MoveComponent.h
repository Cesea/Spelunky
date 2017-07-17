#ifndef MOVE_COMPONENT_H
#define MOVE_COMPONENT_H

#include "MovingObject.h"

class MoveComponent
{
public :
	MoveComponent() {}
	virtual ~MoveComponent() {}

	virtual void Move(MovingObject * object, const ControlCommand &commands) = 0;
	virtual void Move(const Vector2 &force) = 0;
private :
};

class SimpleMoveComponent : public MoveComponent
{
	SimpleMoveComponent() {}
	virtual ~SimpleMoveComponent() {}

	void Move(MovingObject * object, const ControlCommand &commands) override;
	void Move(const Vector2 &force) override;
};




#endif