#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"


class Camera
{
public :
	Camera();
	~Camera();

	virtual HRESULT Init();
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget);

	inline ScreenVector2 GetPosition()
	{
		return _position;
	}

	inline ScreenVector2 GetSeeingRange()
	{
		return _seeingRange;
	}

	
private :

	ScreenVector2 _position{};
	ScreenVector2 _seeingRange{WINSIZEX, WINSIZEY};
};


#endif