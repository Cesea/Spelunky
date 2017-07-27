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
	EVENTMANAGER->RegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Arrow, &Arrow::HandleDamageEvent>(this));
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


	return S_OK;
}

void Arrow::PostInit()
{
	if (_throwDirection == Direction::Left)
	{
		position.AddToTileRelX(-50);
		position.AddToTileRelY(-10);
		desiredPosition = position;
		_velocity.x -= 1000;
		_velocity.y -= 160;
	}
	else if (_throwDirection == Direction::Right)
	{
		position.AddToTileRelX(50);
		position.AddToTileRelY(-10);
		desiredPosition = position;
		_velocity.x += 1000;
		_velocity.y -= 160;
	}
}

void Arrow::Release(void)
{
	EquipItem::Release();
	EVENTMANAGER->UnRegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Arrow, &Arrow::HandleDamageEvent>(this));
	SAFE_RELEASE_AND_DELETE(_sprite);
}

void Arrow::Update(float deltaTime)
{
	if (_equiped)
	{
		desiredPosition = _pOwner->desiredPosition;
		desiredPosition.AddToTileRelY(-16);
		position = desiredPosition;
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
			if (velLength > 300.0f)
			{
				EVENTMANAGER->QueueEvent(new DamageEvent(_id, 1, position, _collisionComp->GetRect(), _collisionComp->GetOffset()));

				if (collisionResult && _sourceIndex.x == 0)
				{
					SOUNDMANAGER->Play(L"bounce");
				}
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

void Arrow::Use(const ControlCommand & commands)
{
}

GameObject * Arrow::Copy(ObjectId id)
{
	return nullptr;
}

void Arrow::Apply(ObjectId id)
{
}

void Arrow::HandlePlayerAttackEvent(const IEvent * event)
{
}

void Arrow::HandleDamageEvent(const IEvent * event)
{
	if (!_equiped)
	{
		PlayerAttackEvent *convertedEvent = (PlayerAttackEvent *)event;
		Direction seeingDirection = convertedEvent->GetDirection();

		Vector2 positionUntiled = position.UnTilelize();
		const TilePosition & playerPosition = convertedEvent->GetTilePosition();
		Vector2 playerPositionUntiled = playerPosition.UnTilelize();

		bool hitted = false;
		if (seeingDirection == Direction::Left)
		{
			if (positionUntiled.x <= playerPositionUntiled.x + 10 &&
				positionUntiled.x >= playerPositionUntiled.x - 74 &&
				position.tileY == playerPosition.tileY)
			{
				hitted = true;
			}
		}
		else if (seeingDirection == Direction::Right)
		{
			if (positionUntiled.x >= playerPositionUntiled.x - 10 &&
				positionUntiled.x <= playerPositionUntiled.x + 74 &&
				position.tileY == playerPosition.tileY)
			{

				hitted = true;
			}
		}
		if (hitted)
		{
			if (seeingDirection == Direction::Left)
			{
				_velocity.x = -140;
			}
			else if (seeingDirection == Direction::Right)
			{
				_velocity.x = 140;
			}
			_velocity.y -= 80;
		}
	}
}

void Arrow::operator=(const ThrowProperty * property)
{
}
