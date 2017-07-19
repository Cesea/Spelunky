#ifndef GEM_H
#define GEM_H

#include "PassiveItem.h"

class Gem : public PassiveItem
{
public :
	Gem(ObjectId id);
	virtual ~Gem();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);


	virtual GameObject *Copy(ObjectId id);

	virtual void Apply(ObjectId id);

private :

	int _value{};
};


#endif