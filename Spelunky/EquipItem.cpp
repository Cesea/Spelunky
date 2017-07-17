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
	_collisionComp->Init(RectMake(0, 0, 40, 40), Vector2(-20, -40));

	return S_OK;
}

void EquipItem::Release(void)
{
	SAFE_DELETE(_collisionComp);
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<EquipItem, &EquipItem::HandlePlayerInputEvent>(this));
}

void EquipItem::Update(float deltaTime)
{
	if (_equiped)
	{
		desiredPosition = _pOwner->desiredPosition;
		desiredPosition.AddToTileRelY(-4);
		position = desiredPosition;
	}
	else
	{
		if (_velocity.DistanceSquare() > 1.0f)
		{

			_accel.y += GRAVITY;
			_velocity += _accel * deltaTime;

			_accel = Vector2();
			desiredPosition.AddToTileRel(_velocity * deltaTime);

			TilePosition centerPos = desiredPosition;
			centerPos.AddToTileRelY(-32.0f);
			_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacentTiles9(IntVector2(centerPos.tileX, centerPos.tileY));
			_collisionComp->Update(this, deltaTime, &_nearTiles);
		}
	}
}

void EquipItem::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->Render(renderTarget, drawPos.x, drawPos.y);
}

void EquipItem::Use()
{
	_equiped = false;
	_pOwner = nullptr;
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
			_pOwner = (MovingObject *)OBJECTMANAGER->FindObjectId(convertedEvent->GetId());
			_equiped = true;
			return;
			//EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<EquipItem, &EquipItem::Move>(this));
		}
	}

	if (_equiped)
	{
		PlayerInputEvent *convertedEvent = (PlayerInputEvent *)(event);
		const ControlCommand &commands = convertedEvent->GetControlCommand();

		if (commands.action == Command::Attack)
		{
			_collisionComp->SetRepulse(true);
			Use();
		}
	}
}

void EquipItem::Apply(ObjectId id)
{
}
