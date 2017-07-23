#include "stdafx.h"
#include "Tunnel.h"

#include "CollisionComponent.h"

Tunnel::Tunnel(ObjectId id)
	:CollidingObject(id)
{
}

Tunnel::~Tunnel()
{
}

HRESULT Tunnel::Init(BaseProperty * property)
{
	_collisionComp = new CollisionComponent;
	_collisionComp->Init(RectMake(0, 0, 64, 64), Vector2(-32, 0));

	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"exitdoors"), 256, 256, IntVector2(-128, -96));
	TunnelProperty *convertedProperty = (TunnelProperty *)(property);
	*this = convertedProperty;

	position.AddToTileRelX(32);

	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Tunnel, &Tunnel::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<Tunnel, &Tunnel::HandlePlayerInputEvent>(this));

	return S_OK;
}

void Tunnel::Release(void)
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Tunnel, &Tunnel::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<Tunnel, &Tunnel::HandlePlayerInputEvent>(this));
	SAFE_RELEASE_AND_DELETE(_sprite);
}

void Tunnel::Update(float deltaTime)
{
}

void Tunnel::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->FrameRender(gRenderTarget,  drawPos.x, drawPos.y, _sourceIndex.x, _sourceIndex.y);


	const Vector2 itemUntiledPosition = position.UnTilelize();
	Rect itemAbsRect =
		RectMake(itemUntiledPosition.x, itemUntiledPosition.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	itemAbsRect += _collisionComp->GetOffset();

	//DrawBox(gRenderTarget, itemAbsRect.x - camPos.x, itemAbsRect.y - camPos.y, itemAbsRect.width, itemAbsRect.height, D2D1::ColorF(1.0f, 1.0f, 0.0f));

}

void Tunnel::HandlePlayerPositionEvent(const IEvent * event)
{
	if (_exit)
	{

		PlayerPositionEvent *convertedEvent = (PlayerPositionEvent *)(event);
		const Rect &playerRect = convertedEvent->GetRect();
		const Vector2 &playerUntiledPosition = convertedEvent->GetPosition().UnTilelize();

		const Vector2 itemUntiledPosition = position.UnTilelize();

		Rect playerAbsRect = RectMake(playerUntiledPosition.x, playerUntiledPosition.y,
			playerRect.width, playerRect.height);
		playerAbsRect += convertedEvent->GetRectOffset();

		Rect itemAbsRect =
			RectMake(itemUntiledPosition.x, itemUntiledPosition.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
		itemAbsRect += _collisionComp->GetOffset();
		if (IsRectangleOverlap(playerAbsRect, itemAbsRect))
		{
			_playerOn = true;
			EVENTMANAGER->QueueEvent(new OnTunnelEvent(IntVector2(position.tileX, position.tileY)));
		}
		else
		{
			_playerOn = false;
		}
	}
}

void Tunnel::HandlePlayerInputEvent(const IEvent * event)
{
	if (_playerOn)
	{
		PlayerInputEvent *convertedEvent = (PlayerInputEvent *)event;
		if (convertedEvent->GetControlCommand().action == Command::GoExit)
		{
			//EVENTMANAGER->QueueEvent(new PlayerGoExitEvent());
		}
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
