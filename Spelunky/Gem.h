#ifndef GEM_H
#define GEM_H

#include "PassiveItem.h"

class Gem : public PassiveItem
{
public:

	friend class Stage;

	Gem(ObjectId id);
	virtual ~Gem();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual GameObject *Copy(ObjectId id);

	virtual void Apply(ObjectId id);

	void SetIsInTile(bool b) { _isInTile = b; }
	void Digged(); 

private :
	Gem &operator= (const GemProperty *other);

	int _value{};

	bool _isInTile{ false };
};


#endif