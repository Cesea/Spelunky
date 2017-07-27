#ifndef ARROW_H
#define ARROW_H

#include "Throws.h"

class Arrow : public Throws
{
public:
	Arrow(ObjectId id);
	virtual ~Arrow();

	virtual HRESULT Init(BaseProperty *property);
	void PostInit();
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual void HandlePlayerPositionEvent(const IEvent *event);

	void SetFireDirection(Direction direction) { _throwDirection = direction; }
	virtual void SetThrowDir(Direction direction) { _throwDirection = direction; }

private:

	void operator= (const ThrowProperty *property);

	bool32 _arrowFirstHit{ false };
	Direction _throwDirection{};
};

#endif