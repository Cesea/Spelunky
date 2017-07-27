#include "stdafx.h"
#include "Arrow.h"

#include "CollisionComponent.h"

Arrow::Arrow(ObjectId id)
	:Throws(id)
{
}

Arrow::~Arrow()
{
}

HRESULT Arrow::Init(BaseProperty * property)
{
	EquipItem::Init(property);
	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"throws"), 80, 80, IntVector2(-40, -50));

	ThrowProperty *convertedProperty = (ThrowProperty *)(property);

	position.tileX = property->position.x;
	position.tileY = property->position.y;

	_breakable = false;
	if (!_breakable)
	{
		_collisionComp->SetRepulse(true);
	}

	_equipSlot = EquipSlot::Weapon;
	_sourceIndex.x = 4;
	_sourceIndex.y = 0;
	_breakType = BreakType::BREAK_Arrow;


	_speed = Vector2(400, 300);
	_maxVelocity = Vector2(1200, 900);

	EVENTMANAGER->RegisterDelegate(EVENT_OBSTACLE_POSITION, EventDelegate::FromFunction<Throws, &Throws::HandleObstaclePositionEvent>(this));

	return S_OK;
}

void Arrow::PostInit()
{
	if (_throwDirection == Direction::Left)
	{
		position.AddToTileRelX(-70);
		position.AddToTileRelY(-30);
		desiredPosition = position;
		_velocity.x -= 900;
		_velocity.y -= 130;
	}
	else if (_throwDirection == Direction::Right)
	{
		position.AddToTileRelX(70);
		position.AddToTileRelY(-30);
		desiredPosition = position;
		_velocity.x += 900;
		_velocity.y -= 130;
	}
}

void Arrow::Release(void)
{
	Throws::Release();
}

void Arrow::Update(float deltaTime)
{
	if (_equiped)
	{
		desiredPosition = _pOwner->desiredPosition;
		desiredPosition.AddToTileRelY(-16);
		position = desiredPosition;
		_throwDirection = _pOwner->GetDirection();
	}
	else
	{
		_accel.y += GRAVITY;

		_velocity += _accel * deltaTime;

		_accel = Vector2();
		ClampFloat(&_velocity.x, -_maxVelocity.x, _maxVelocity.x);
		ClampFloat(&_velocity.y, -_maxVelocity.y, _maxVelocity.y);
		desiredPosition.AddToTileRel(_velocity * deltaTime);


		TilePosition centerPos = desiredPosition;
		centerPos.AddToTileRelY(-16.0f);
		_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));

		//Vector2 posUn = centerPos.UnTilelize();

		bool collisionResult = _collisionComp->Update(this, deltaTime, &_nearTiles);

		if (_throwed)
		{
			float velLength = _velocity.Distance();
			if (velLength > 800.0f)
			{
				EVENTMANAGER->QueueEvent(new DamageEvent(_id, 1, position, _collisionComp->GetRect(), _collisionComp->GetOffset()));

				if (collisionResult && _sourceIndex.x == 0)
				{
					SOUNDMANAGER->Play(L"arrow_hit_wall");
				}
			}
			else
			{
				SOUNDMANAGER->Play(L"bounce");
			}

			if (_breakable && collisionResult)
			{
				if (_sourceIndex.x == 1)
				{
					EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_Jar));
				}
				else if (_sourceIndex.x == 2 || _sourceIndex.x == 3)
				{
					EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_Bone));
				}
			}

			if (_velocity.DistanceSquare() < 36.0f)
			{
				_throwed = false;
				_velocity = Vector2();
			}
		}
	}

}

void Arrow::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	if (_throwDirection == Direction::Left)
	{
		_sprite->FrameRenderFlip(renderTarget, drawPos.x, drawPos.y, _sourceIndex.x, _sourceIndex.y);
	}
	else if(_throwDirection == Direction::Right)
	{
		_sprite->FrameRender(renderTarget, drawPos.x, drawPos.y, _sourceIndex.x, _sourceIndex.y);
	}
}

void Arrow::HandlePlayerPositionEvent(const IEvent * event)
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

	float relX = itemUntiledPosition.x - playerUntiledPosition.x;
	float relY = itemUntiledPosition.y - playerUntiledPosition.y;

	if (IsRectangleOverlap(playerAbsRect, itemAbsRect))
	{
		if (_velocity.Distance() > 1000)
		{
			Vector2 diff(relX, relY);
			diff.Normalize();
			EVENTMANAGER->QueueEvent(new PlayerDamagedEvent(_id, 1,  diff));
		}
	}
}


void Arrow::operator=(const ThrowProperty * property)
{
}
