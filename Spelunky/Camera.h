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

	void SetTarget(GameObject *target);
	void SetPosition(const TilePosition& position);

	const TilePosition &GetPosition() { return _position; }

	void Move(const Vector2 &v);

	
private :

	void KeepCameraInsideAnchorRect();

	TilePosition _position{};
	Rect _seeingRect{};
	Rect _anchorRect{};
	Rect _worldRect{};

	GameObject *_target{};

	bool _following{ false };

};


#endif