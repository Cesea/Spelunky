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
	return S_OK;
}

void Camera::Release(void)
{
}

void Camera::Update(void)
{
	//_position = _pPlayerCar->GetPosition();

}

void Camera::Render(ID2D1HwndRenderTarget * renderTarget)
{
}
