#ifndef TUNNEL_H
#define TUNNEL_H

#include "CollidingObject.h"	

class Tunnel : public CollidingObject
{
public :
	Tunnel(ObjectId id);
	virtual ~Tunnel();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	void HandlePlayerPositionEvent(const IEvent *event);
	void HandlePlayerInputEvent(const IEvent *event);

	void SetExit(bool32 b) { _exit = b; }

private :
	virtual GameObject *Copy(ObjectId id);

	Tunnel &operator= (TunnelProperty *other);
	Tunnel &operator= (const Tunnel &other);

	D2DSprite *_sprite{};
	IntVector2 _sourceIndex;
	bool32 _exit{false};

	bool32 _playerOn{ false };

};



#endif