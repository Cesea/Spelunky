#include "stdafx.h"
#include "Tunnel.h"

Tunnel::Tunnel(ObjectId id)
	:CollidingObject(id)
{
}

Tunnel::~Tunnel()
{
}

HRESULT Tunnel::Init(BaseProperty * property)
{
	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"exitdoors"), 256, 256, IntVector2(-96, -96));
	TunnelProperty *convertedProperty = (TunnelProperty *)(property);
	*this = convertedProperty;

	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Tunnel, &Tunnel::HandlePlayerPositionEvent>(this));

	return S_OK;
}

void Tunnel::Release(void)
{
	SAFE_RELEASE_AND_DELETE(_sprite);
}

void Tunnel::Update(float deltaTime)
{
}

void Tunnel::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->FrameRender(gRenderTarget,  drawPos.x, drawPos.y, _sourceIndex.x, _sourceIndex.y);
}

void Tunnel::HandlePlayerPositionEvent(const IEvent * event)
{
	PlayerPositionEvent *convertedEvent = (PlayerPositionEvent *)(event);
	if (_exit)
	{
	}
}

GameObject * Tunnel::Copy(ObjectId id)
{
	Tunnel *newTunnel = new Tunnel(id);
	newTunnel = this;
	return newTunnel;
}

Tunnel & Tunnel::operator=(TunnelProperty * other)
{
	position.tileX = other->position.x;
	position.tileY = other->position.y;
	_sourceIndex = other->sourceIndex;
	_exit = other->exit;
	return *this;
}

Tunnel & Tunnel::operator=(const Tunnel & other)
{
	_sprite = other._sprite;
	position = other.position;
	_sourceIndex = other._sourceIndex;
	_exit = other._exit;
	return *this;
}
