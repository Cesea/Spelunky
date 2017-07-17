#include "stdafx.h"
#include "EquipItem.h"

EquipItem::EquipItem(ObjectId id)
	:Item(id)
{
}

EquipItem::~EquipItem()
{
}

HRESULT EquipItem::Init(ArcheType type)
{
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<EquipItem, &EquipItem::HandlePlayerInputEvent>(this));
	_collisionComp = new CollisionComponent();
	_collisionComp->Init(RectMake(0, 0, 40, 40), Vector2(-20, -50));

	return S_OK;
}

void EquipItem::Release(void)
{
	SAFE_DELETE(_collisionComp);
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<EquipItem, &EquipItem::HandlePlayerInputEvent>(this));
}

void EquipItem::Update(float deltaTime)
{
	_velocity += _accel * deltaTime;

	position.AddToTileRel(_velocity * deltaTime);

	_accel = Vector2();
}

void EquipItem::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->Render(renderTarget, drawPos.x, drawPos.y);
}

void EquipItem::Use()
{
	_equiped = false;
}

GameObject * EquipItem::Copy(ObjectId id)
{
	return nullptr;
}

void EquipItem::HandlePlayerInputEvent(const IEvent * event)
{
	//나중에 플레이어가 앉아있는 상태일때 다른 메시지를 주는 방식으로 변경 요망,
	if (_actorOn && !_equiped)
	{
		PlayerInputEvent *convertedEvent = (PlayerInputEvent *)(event);
		const ControlCommand &commands = convertedEvent->GetControlCommand();
		if (commands.action == Command::Attack)
		{
			_equiped = true;
			//EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<EquipItem, &EquipItem::Move>(this));
		}
	}

	if (_equiped)
	{
		PlayerInputEvent *convertedEvent = (PlayerInputEvent *)(event);
		const ControlCommand &commands = convertedEvent->GetControlCommand();
		if (commands.horizontal == Command::MoveLeft)
		{
			_accel.x = -_speed.x;
		}
		else if (commands.horizontal == Command::MoveRight)
		{
			_accel.x = _speed.x;
		}

		if (commands.action == Command::Attack)
		{
			Use();
		}
	}
}

void EquipItem::Apply(ObjectId id)
{
}
