#include "stdafx.h"
#include "Throws.h"

#include "CollisionComponent.h"

Throws::Throws(ObjectId id)
	:EquipItem(id)
{
}

Throws::~Throws()
{
}

HRESULT Throws::Init(BaseProperty *property)
{
	EquipItem::Init(property);
	EVENTMANAGER->RegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Throws, &Throws::HandleDamageEvent>(this));
	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"throws"), 80, 80, IntVector2(-40, -50));

	_speed = Vector2(400, 300);
	_maxVelocity = Vector2(300, 550);

	ThrowProperty *convertedProperty = (ThrowProperty *)(property);
	*this = convertedProperty;

	_equipSlot = EquipSlot::Weapon;

	if (_sourceIndex.x == 0) { _breakType = BreakType::BREAK_Rock; }
	else if(_sourceIndex.x == 1) { _breakType = BreakType::BREAK_Jar; }
	else if(_sourceIndex.x == 2) { _breakType = BreakType::BREAK_BackBone; }
	else if(_sourceIndex.x == 3) { _breakType = BreakType::BREAK_Bone; }

	return S_OK;
}

void Throws::Release(void)
{
	EquipItem::Release();
	EVENTMANAGER->UnRegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Throws, &Throws::HandleDamageEvent>(this));
	SAFE_RELEASE_AND_DELETE(_sprite);
}

void Throws::Update(float deltaTime)
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

void Throws::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->FrameRender(renderTarget, drawPos.x, drawPos.y, _sourceIndex.x, _sourceIndex.y);

	const Vector2 itemUntiledPosition = position.UnTilelize();
	Rect itemAbsRect =
			RectMake(itemUntiledPosition.x, itemUntiledPosition.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	itemAbsRect += _collisionComp->GetOffset();

	DrawBox(renderTarget, itemAbsRect.x - camPos.x, itemAbsRect.y - camPos.y, itemAbsRect.width, itemAbsRect.height, D2D1::ColorF(1.0f, 0.0, 0.0, 1.0f));

	Vector2 pos = position.UnTilelize();

	DrawBox(renderTarget, pos.x - camPos.x, pos.y - camPos.y, 5, 5, D2D1::ColorF(1.0f, 1.0, 0.0, 1.0f));
}

void Throws::Use(const ControlCommand &commands)
{
	_equiped = false;

	if (commands.horizontal == Command::None)
	{
		if (_pOwner->GetDirection() == Direction::Left)
		{
			_velocity.x -= 900;
		}
		else if (_pOwner->GetDirection() == Direction::Right)
		{
			_velocity.x += 900;
		}
	}
	else
	{
		if (commands.horizontal == Command::MoveLeft)
		{
			_velocity.x -= 900;
		}
		else if (commands.horizontal == Command::MoveRight)
		{
			_velocity.x += 900;
		}
	}

	if (commands.vertical == Command::MoveUp)
	{
		_velocity.y -= 600;
	}
	else if(commands.vertical == Command::MoveDown)
	{
		_velocity.y = 100;
	}
	else if (commands.vertical == Command::None)
	{
		_velocity.y -= 500;
	}

	_velocity.Normalize();
	_velocity *= 1000.0f;

	_throwed = true;

	_pOwner = nullptr;
}

GameObject * Throws::Copy(ObjectId id)
{
	return new Throws(id);
}

void Throws::Apply(ObjectId id)
{
}

void Throws::HandlePlayerAttackEvent(const IEvent * event)
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
			if (positionUntiled.x <= playerPositionUntiled.x  + 10&&
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
			if (_breakable)
			{
				if (_sourceIndex.x == 1)
				{
					EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_Jar));
				}
				else if (_sourceIndex.x == 2)
				{
					EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_BackBone));
				}

				else if (_sourceIndex.x == 3)
				{
					EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_Bone));
				}
			}
			else
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
}

void Throws::HandleDamageEvent(const IEvent * event)
{
	DamageEvent *convertedEvent = (DamageEvent *)(event);
	if (_id == convertedEvent->GetAttackerId())
	{
		return;
	}
	else
	{
		const TilePosition &attackerPosition = convertedEvent->GetTilePosition();
		int tileXDiff = attackerPosition.tileX - position.tileX;
		int tileYDiff = attackerPosition.tileY - position.tileY;

		if (abs(tileXDiff) >= 2 || abs(tileYDiff) > 2)
		{
			return;
		}

		const Rect &attackerRect = convertedEvent->GetRect();
		const Vector2 &attackerRectOffset = convertedEvent->GetRectOffset();

		Rect attackerAbsRect = attackerRect + attackerRectOffset + attackerPosition.UnTilelize();
		Rect thisAbsRect = _collisionComp->GetRect() + _collisionComp->GetOffset() + position.UnTilelize();

		Rect overlapRect;
		if (IsRectangleOverlap(attackerAbsRect, thisAbsRect, overlapRect))
		{
			if (_sourceIndex.x == 1)
			{
				EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_Jar));
			}
			else if (_sourceIndex.x == 2)
			{
				EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_BackBone));
			}
			else if (_sourceIndex.x == 3)
			{
				EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_Bone));
			}
		}
	}
}

void Throws::operator=(const ThrowProperty * property)
{
	position.tileX = property->position.x;
	position.tileY = property->position.y;
	_sourceIndex = property->sourceIndex;

	_breakable = property->breakable;
	if (!_breakable)
	{
		_collisionComp->SetRepulse(true);
	}
}
