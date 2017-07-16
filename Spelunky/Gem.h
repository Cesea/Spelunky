#ifndef GEM_H
#define GEM_H

#include "PassiveItem.h"

class Gem : public PassiveItem
{
public :
	Gem(ObjectId id);
	virtual ~Gem();

	virtual HRESULT Init(ArcheType type);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual void Use();


	virtual GameObject *Copy(ObjectId id);
	virtual void HandleMessage(const IEvent *event);

	virtual void Apply(GameObject *object);

private :
};


#endif