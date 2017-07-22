#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

HRESULT Camera::Init()
{
	_worldRect = RectMake(0, 0, TILE_SIZE * STAGE_TOTAL_COUNTX, TILE_SIZE * STAGE_TOTAL_COUNTY);
	_anchorRect = RectMake(0, 0, _worldRect.width - WINSIZEX, _worldRect.height - WINSIZEY);
	return S_OK;
}

void Camera::Release(void)
{
}

void Camera::Update(float deltaTime)
{
	if (_target && _following)
	{
		_position = _target->position;
		_position.AddToTile(-10, -6);
	}
	if (_shaking)
	{
		if (_shakeTimer.Tick(deltaTime))
		{
			_shakeCount--;
			if (_shakeCount == -1)
			{
				_shaking = false;
				_position = _targetPosition;
			}
			else if (_shakeCount == 0)
			{
				_startPosition = _position;
				_targetPosition = _anchorPosition;
			}
			else
			{
				_power -= _powerSubDelta;
				_startPosition = _position;

				_targetPosition = _anchorPosition;
				float randRadian = RND->GetFloat(0, M_PI2);
				_targetPosition.AddToTileRel(cosf(randRadian) * _power, -sinf(randRadian) * _power);
			}
		}
		else
		{
			float t = _shakeTimer.GetCurrentSecond() / _shakeTimer.GetTargetSecond();
			_position = InterpolateTilePosition(_startPosition, _targetPosition, t);
		}
	}

	KeepCameraInsideAnchorRect();
	//_position = _pPlayerCar->GetPosition();
}
void Camera::SetTarget(GameObject * target)
{
	_target = target;
}

void Camera::SetPosition(const TilePosition & position)
{
	_position = position;
}

void Camera::Move(const Vector2 & v)
{
	_position.AddToTileRel(v.x, v.y);
}

void Camera::Shake(float power, int shakeCount, float t)
{
	_shakeTimer.ResetAndChangeTargetSecond(t / float(shakeCount));
	_power = power;
	_shaking = true;
	_shakeCount = shakeCount;
	_powerSubDelta = (_power / (float)shakeCount);

	_anchorPosition = _position;
}

void Camera::KeepCameraInsideAnchorRect()
{
	if (_position.tileX < 0)
	{
		_position.tileX = 0;
		_position.tileRel.x = 0;
	}

	if (_position.tileY < 0)
	{
		_position.tileY = 0;
		_position.tileRel.y = 0;
	}

	if (_position.tileX > STAGE_TOTAL_COUNTX - 2 - 20)
	{
		_position.tileX = STAGE_TOTAL_COUNTX - 2- 20;
		_position.tileRel.x = 64;
	}
		//if (_position.tileRel.x > TILE_SIZE)
		//{
		//	_position.tileRel.x = TILE_SIZE;
		//}

	if (_position.tileY > STAGE_TOTAL_COUNTY - 13)
	{
		_position.tileY = STAGE_TOTAL_COUNTY - 13;
		_position.tileRel.y = 64;
		//if (_position.tileRel.y > TILE_SIZE)
		//{
		//	_position.tileRel.y = TILE_SIZE;
		//}
	}
}
