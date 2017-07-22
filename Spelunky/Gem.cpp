#include "stdafx.h"
#include "Gem.h"

#include "CollisionComponent.h"

Gem::Gem(ObjectId id)
	:PassiveItem(id)
{
}

Gem::~Gem()
{
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
}

void Gem::Update(float deltaTime)
{
	if (!_isInTile)
	{
		//if (_velocity.DistanceSquare() > 1.0f)
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
	EVENTMANAGER->QueueEvent(new CollectMoneyEvent(_id, _onActorId, _value));
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

Gem & Gem::operator=(const GemProperty * other)
{
	position.tileX = other->position.x;
	position.tileY = other->position.y;
	_sourceIndex = other->sourceIndex;
	_value = other->value;
	return *this;
}

