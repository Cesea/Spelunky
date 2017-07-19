#include "stdafx.h"
#include "Item.h"

Item::Item(ObjectId id)
	:MovingObject::MovingObject(id)
{
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Item, &Item::HandlePlayerPositionEvent>(this));
}

Item::~Item()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Item, &Item::HandlePlayerPositionEvent>(this));
}

HRESULT Item::Init(BaseProperty *property)
{
	return S_OK;
}

void Item::Release(void)
{
}

void Item::Update(float deltaTime)
{
}

void Item::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
}

GameObject * Item::Copy(ObjectId id)
{
	return nullptr;
}


void Item::HandlePlayerPositionEvent(const IEvent * event)
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
	itemAbsRect	+= _collisionComp->GetOffset();
	if (IsRectangleOverlap(playerAbsRect, itemAbsRect))
	{
		_onActorId = convertedEvent->GetId();
		_actorOn = true;
		//Console::Log("on\n");
	}
	else
	{
		_actorOn = false;
		_onActorId = UNVALID_OBJECT_ID;
	}
}

