#include "stdafx.h"
#include "Bomb.h"

#include "CollisionComponent.h"

Bomb::Bomb(ObjectId id)
	:MovingObject(id)
{
}

Bomb::~Bomb()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_OBSTACLE_POSITION, EventDelegate::FromFunction<Bomb, &Bomb::HandleObstaclePositionEvent>(this));
	SAFE_RELEASE_AND_DELETE(_sprite);
	SAFE_DELETE(_collisionComp);
}

HRESULT Bomb::Init(BaseProperty * property)
{
	BombProperty *convertedProperty = (BombProperty *)property;

	position.tileX = property->position.x;
	position.tileY = property->position.y;

	_isSticky = convertedProperty->sticky;
	_velocity = convertedProperty->initialVelocity;

	_sprite = new D2DAnimationSprite();
	Animation *animation = new Animation(); 
	if (_isSticky)
	{
		animation->InitCopy(KEYANIMANAGER->FindAnimation(L"sticky_bomb"));
	}
	else
	{
		animation->InitCopy(KEYANIMANAGER->FindAnimation(L"normal_bomb"));
	}
	_sprite->Init(IMAGEMANAGER->GetImage(L"bomb"), animation, IntVector2(-40, -40));

	_bombTimer.Init(0.8f);

	_scaleTimer.Init(0.2f);
	mat = D2D1::Matrix3x2F::Identity();

	_velocity = convertedProperty->initialVelocity;
	_maxVelocity = Vector2(300, 550);

	_collisionComp = new CollisionComponent;
	_collisionComp->Init(RectMake(0, 0, 40, 40), Vector2(-20, -20));

	_collisionComp->SetRepulse(true);

	EVENTMANAGER->RegisterDelegate(EVENT_OBSTACLE_POSITION, EventDelegate::FromFunction<Bomb, &Bomb::HandleObstaclePositionEvent>(this));
	return S_OK;
}

void Bomb::Release(void)
{
	//_sprite->Release();
	//delete _sprite;
}

void Bomb::Update(float deltaTime)
{
	_accel.y += GRAVITY;
	_velocity += _accel * deltaTime;

	_accel = Vector2();
	desiredPosition.AddToTileRel(_velocity * deltaTime);

	TilePosition centerPos = desiredPosition;
	centerPos.AddToTileRelY(-8.0f);
	_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));
	_collisionComp->Update(this, deltaTime, &_nearTiles);

	if (_bombTimer.Tick(deltaTime))
	{
		_bombStage++;
		if (_bombStage == 1)
		{
			_sprite->GetAnimation()->Start();
			_yScaling = true;

			_startScale = _scale;
			_targetScale = (_scale == 1.0f) ? 0.4f : 1.0f;
			SOUNDMANAGER->Play(L"bomb_timer");
		}
		else if(_bombStage == 2)
		{
			_scaleTimer.ResetAndChangeTargetSecond(0.1f);
			_sprite->GetAnimation()->SetFPS(30);

		}
		else if (_bombStage == 3)
		{
			STAGEMANAGER->DestroyTile(position.tileX - 2, position.tileY - 2, 4, 3);
			EVENTMANAGER->QueueEvent(new ItemBreakEvent(_id, BreakType::BREAK_Bomb));
			EFFECTMANAGER->PlayExplosionEffect(position.UnTilelize());
			EFFECTMANAGER->PlaySmokeEffect(position.UnTilelize());
			SOUNDMANAGER->Play(L"bomb_explosion");
		}
	}
	_sprite->Update(deltaTime);
	if (_yScaling)
	{
		if (_scaleTimer.Tick(deltaTime))
		{
			_scale = _targetScale;
			_startScale = _scale;
			_targetScale = (_scale == 1.0f) ? 0.4f : 1.0f;
		}

		float t = _scaleTimer.GetCurrentSecond() / _scaleTimer.GetTargetSecond();
		_scale = InterpolateFloat(_startScale, _targetScale, t);
			
		mat = D2D1::Matrix3x2F::Scale(_scale, _scale, D2D1::Point2F(40, 40));
	}
}

void Bomb::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->RenderMatrix(renderTarget, drawPos.x, drawPos.y, mat);
}

GameObject * Bomb::Copy(ObjectId id)
{
	return nullptr;
}

void Bomb::HandleObstaclePositionEvent(const IEvent * event)
{
	ObstaclePositionEvent *convertedEvent = (ObstaclePositionEvent *)(event);

	const TilePosition &obstacleTilePos = convertedEvent->GetPosition();
	int tileXDiff = obstacleTilePos.tileX - position.tileX;
	int tileYDiff = obstacleTilePos.tileY - position.tileY;

	if (abs(tileXDiff) >= 3 || abs(tileYDiff) >= 3)
	{
		return;
	}

	const Rect &obstacleRect = convertedEvent->GetRect();
	const Vector2 &obstacleUntiledPosition = convertedEvent->GetPosition().UnTilelize();

	const Vector2 itemUntiledPosition = position.UnTilelize();

	Rect obstacleAbsRect = RectMake(obstacleUntiledPosition.x, obstacleUntiledPosition.y,
		obstacleRect.width, obstacleRect.height);
	obstacleAbsRect += convertedEvent->GetRectOffset();

	Rect itemAbsRect =
		RectMake(itemUntiledPosition.x, itemUntiledPosition.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	itemAbsRect += _collisionComp->GetOffset();

	float relXDiff = itemUntiledPosition.x - obstacleUntiledPosition.x;
	float relYDiff = itemUntiledPosition.y - obstacleUntiledPosition.y;

	Rect overlapRect;
	if (IsRectangleOverlap(obstacleAbsRect, itemAbsRect, overlapRect))
	{
		if (overlapRect.width > overlapRect.height)
		{
			if (relYDiff > 0)
			{
				position.AddToTileRelY(overlapRect.height);
				_velocity.y = 0;
				_velocity.x *= 0.5;
			}
			else
			{
				position.AddToTileRelY(-overlapRect.height);
				_velocity.y = 0;
				_velocity.x *= 0.5;
			}
		}
		else
		{
			if (relXDiff > 0)
			{
				position.AddToTileRelX(overlapRect.width);
				_velocity.x *= -0.5;
			}
			else
			{
				position.AddToTileRelX(-overlapRect.width);
				_velocity.x *= -0.5;
			}
		}
		desiredPosition = position;
	}
}