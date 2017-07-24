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
	virtual void Update(float deltaTime);

	void SetTarget(GameObject *target);
	void SetPosition(const TilePosition& position);

	const TilePosition &GetPosition() { return _position; }

	void Move(const Vector2 &v);

	void Shake(float power, int shakeCount, float t);

	void ResetForMiddleStage();
	void ResetForNormalStage();
	
private :

	void KeepCameraInsideAnchorRect();

	TilePosition _position{};

	TilePosition _startPosition{};
	TilePosition _targetPosition{};

	TilePosition _anchorPosition{};

	Rect _seeingRect{};
	Rect _anchorRect{};
	Rect _worldRect{};

	GameObject *_target{};

	bool _following{ true };

	bool _shaking{ false };
	float _power{ 0 };
	float _powerSubDelta{ 0 };
	int _shakeCount{ 0 };
	Timer _shakeTimer;

};


#endif