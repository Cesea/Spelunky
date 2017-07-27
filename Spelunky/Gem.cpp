#include "stdafx.h"
#include "Gem.h"

#include "CollisionComponent.h"

Gem::Gem(ObjectId id)
	:PassiveItem(id)
{
}

Gem::~Gem()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Item, &Gem::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<Gem, &Gem::HandlePlayerAttackEvent>(this));
}

HRESULT Gem::Init(BaseProperty *property)
{
	PassiveItem::Init(property);
	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"gems"), 80, 80, IntVector2(-40, -40));
	GemProperty *convertedProperty = (GemProperty *)(property);
	*this = convertedProperty;

	position.tileRel = Vector2(32, 32);
	desiredPosition = position;

	_collisionComp = new CollisionComponent;
	_collisionComp->Init(RectMake(0, 0, 40, 40), Vector2(-20, -20));

	return S_OK;
}

void Gem::Release(void)
{
	SAFE_RELEASE_AND_DELETE(_sprite);
	SAFE_DELETE(_collisionComp);
}

void Gem::Update(float deltaTime)
{
	if (!_isInTile)
	{
		{
			_accel.y += GRAVITY;
			_velocity += _accel * deltaTime;

			_accel = Vector2();
			desiredPosition.AddToTileRel(_velocity * deltaTime);

			TilePosition centerPos = desiredPosition;
			centerPos.AddToTileRelY(-32.0f);
			_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));
			_collisionComp->Update(this, deltaTime, &_nearTiles);
		}
	}

	if (_actorOn)
	{
		Apply(_onActorId);
	}
	if (!_valid)
	{
		EVENTMANAGER->QueueEvent(new DestroyObjectEvent(_id));
	}
}

void Gem::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position.UnTilelize() - camPos;
	_sprite->FrameRender(renderTarget, drawingPos.x, drawingPos.y, _sourceIndex.x, _sourceIndex.y);

}


GameObject * Gem::Copy(ObjectId id)
{
	return nullptr;
}


void Gem::Apply(ObjectId id)
{
	SOUNDMANAGER->Play(L"gem" + std::to_wstring(_sourceIndex.x % 5));
	EVENTMANAGER->QueueEvent(new CollectMoneyEvent(_id, _onActorId, _value, _gemType));
	_valid = false;
}

void Gem::Digged()
{
	if (_sourceIndex.x == 0)
	{
		_sourceIndex.x = 6;
		_value = 500;
	}
	else if (_sourceIndex.x == 1)
	{
		_sourceIndex.x = 7;
		_value = 1500;
	}
	_isInTile = false;
}

void Gem::HandlePlayerAttackEvent(const IEvent * event)
{
	if (!_isInTile)
	{
		PlayerAttackEvent *convertedEvent = (PlayerAttackEvent *)event;
		TilePosition playerPos = convertedEvent->GetTilePosition();

		int tileXDiff = playerPos.tileX - position.tileX;
		int tileYDiff = playerPos.tileY - position.tileY;

		if (abs(tileXDiff) >= 3 || abs(tileYDiff) >= 3)
		{
			return;
		}

		Direction seeingDirection = convertedEvent->GetDirection();

		Vector2 positionUntiled = position.UnTilelize();
		const TilePosition & playerPosition = convertedEvent->GetTilePosition();
		Vector2 playerPositionUntiled = playerPosition.UnTilelize();

		bool hitted = false;
		if (seeingDirection == Direction::Left)
		{
			if (positionUntiled.x <= playerPositionUntiled.x + 10 &&
				positionUntiled.x >= playerPositionUntiled.x - 70 &&
				position.tileY == playerPosition.tileY)
			{
				hitted = true;
			}
		}
		else if (seeingDirection == Direction::Right)
		{
			if (positionUntiled.x >= playerPositionUntiled.x - 10 &&
				positionUntiled.x <= playerPositionUntiled.x + 84 &&
				position.tileY == playerPosition.tileY)
			{

				hitted = true;
			}
		}
		if (hitted)
		{
			Console::Log("Gem hitted\n");
			_collisionComp->SetRepulse(true);
			if (seeingDirection == Direction::Left)
			{
				_velocity.x = -140;
			}
			else if (seeingDirection == Direction::Right)
			{
				_velocity.x = 140;
			}
			_velocity.y -= 84;
		}
	}
}

Gem & Gem::operator=(const GemProperty * other)
{
	position.tileX = other->position.x;
	position.tileY = other->position.y;
	_sourceIndex = other->sourceIndex;
	_value = other->value;
	_gemType = other->type;
	return *this;
}

