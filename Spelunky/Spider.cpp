#include "stdafx.h"
#include "Spider.h"

#include "CollisionComponent.h"

#include "SpiderOnTopState.h"
#include "SpiderJumpState.h"
#include "SpiderOnGroundState.h"

Spider::Spider(ObjectId id)
	:Enemy::Enemy(id)
{
}

Spider::~Spider()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Enemy, &Spider::HandleDamageEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<Enemy, &Spider::HandlePlayerAttackEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Spider, &Spider::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_OBSTACLE_POSITION, EventDelegate::FromFunction<Enemy, &Spider::HandleObstaclePositionEvent>(this));
	_graphics.Release();
}

HRESULT Spider::Init(BaseProperty * property)
{
	_hp = 1;
	EVENTMANAGER->RegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Enemy, &Spider::HandleDamageEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<Enemy, &Spider::HandlePlayerAttackEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Spider, &Spider::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_OBSTACLE_POSITION, EventDelegate::FromFunction<Enemy, &Spider::HandleObstaclePositionEvent>(this));
	_collisionComp = new CollisionComponent;
	_collisionComp->Init(RectMake(0, 0, 38, 44), Vector2(-19, -44));

	BuildAnimationSprite(L"flip", IntVector2(-40, -60));
	BuildAnimationSprite(L"ground", IntVector2(-40, -60));
	BuildAnimationSprite(L"jump", IntVector2(-40, -60));

	SetGraphics(L"flip");
	_currentSprite->GetAnimation()->Stop();

	position.tileX = property->position.x;
	position.tileY = property->position.y;
	position.AddToTileRelX(32);
	position.AddToTileRelY(60);
	desiredPosition = position;


	_stateManager.Init(this, new SpiderOnTopState);

	_speed = Vector2(300, 600);
	_maxVelocity = Vector2(300, 500);

	_enemyType = EnemyType::ENEMY_Spider;

	return S_OK;
}

void Spider::PostInit()
{
	_holdingTile = STAGEMANAGER->GetCurrentStage()->GetValidTileAt(position.tileX, position.tileY - 1);
}

void Spider::Release(void)
{
}

void Spider::Update(float deltaTime)
{
	if (!_onTop)
	{
		_accel.y += GRAVITY;
	}
	else
	{
		if (_holdingTile->collisionType != TILE_COLLISION_BLOCK)
		{
			_onTop = false;
		}
	}

	TilePosition centerPos = desiredPosition;
	centerPos.AddToTileRelY(-16.0f);

	_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));
	_stateManager.Update(deltaTime);
	_accel = Vector2();

	desiredPosition.AddToTileRel(_velocity * deltaTime);

	_collisionComp->Update(this, deltaTime, &_nearTiles);
	TileCollisionType lowTileCollisionType = (_nearTiles.tiles[0]) ?
		_nearTiles.tiles[0]->collisionType : TileCollisionType::TILE_COLLISION_NONE;

	_onGround = false;
	if (lowTileCollisionType == TileCollisionType::TILE_COLLISION_BLOCK)
	{
		_onGround = true;
	}
	if (_eventDispatchTimer.Tick(deltaTime))
	{
		EVENTMANAGER->QueueEvent(new EnemyPositionEvent(_id, position, _collisionComp->GetRect(), _collisionComp->GetOffset()));
	}
}

void Spider::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position.UnTilelize() - camPos;
	_currentSprite->Render(renderTarget, drawingPos.x, drawingPos.y);
}

GameObject * Spider::Copy(ObjectId id)
{
	return new Spider(id);
}

void Spider::HandlePlayerPositionEvent(const IEvent * event)
{
	PlayerPositionEvent *convertedEvent = (PlayerPositionEvent *)(event);
	const TilePosition &playerTilePosition =  convertedEvent->GetPosition();
	int tileXDiff = playerTilePosition.tileX - position.tileX;
	int tileYDiff = playerTilePosition.tileY - position.tileY;

	if (abs(tileXDiff) >= 3 || abs(tileYDiff) >= 7)
	{
		return;
	}

	const Vector2 &playerUntiled = playerTilePosition.UnTilelize();
	const Vector2 untiled = position.UnTilelize();

	float relXDiff = playerUntiled.x - untiled.x;
	float relYDiff = playerUntiled.y - untiled.y;

	if ((abs(relXDiff)) < 15 && (playerUntiled.y > untiled.y) && _onTop)
	{
		_currentSprite->GetAnimation()->Start();
		//_stateManager.ChangeState(new Spider);
		_onTop = false;
		return;
	}

	if (!_onTop)
	{
		Vector2 diff = (playerUntiled - untiled);
		diff.Normalize();
		if (diff.x > 0 && _seeingDirection == Direction::Left)
		{
			_currentSprite->SyncFlip(Direction::Right);
			_seeingDirection = Direction::Right;
		}
		else if(diff.x < 0 && _seeingDirection == Direction::Right)
		{
			_currentSprite->SyncFlip(Direction::Left);
			_seeingDirection = Direction::Left;
		}
	}


	const Rect &playerRect = convertedEvent->GetRect();

	Rect playerAbsRect = RectMake(playerUntiled.x, playerUntiled.y,
		playerRect.width, playerRect.height);
	playerAbsRect += convertedEvent->GetRectOffset();

	Rect enemyAbsRect =
		RectMake(untiled.x, untiled.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	enemyAbsRect += _collisionComp->GetOffset();


	Rect overlapRect;
	if (IsRectangleOverlap(playerAbsRect, enemyAbsRect, overlapRect))
	{
		if (overlapRect.width > overlapRect.height && convertedEvent->GetFalling())
		{
			Damaged(1, (relXDiff < 0) ? Direction::Right : Direction::Left);
			EVENTMANAGER->QueueEvent(new PlayerUpperJumpEvent(_velocity));
			return;
		}
		else
		{
			EVENTMANAGER->QueueEvent(new PlayerDamagedEvent(_id, 1, Vector2(relXDiff, relYDiff)));
		}
	}
}

void Spider::BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 & anchor)
{
	std::wstring imageKey = L"spider_";
	Animation *animation = new Animation;
	animation->InitCopy(KEYANIMANAGER->FindAnimation(imageKey + aniKey));
	animation->SetOwner(this);
	D2DSprite *sprite = new D2DAnimationSprite;
	sprite->Init(IMAGEMANAGER->GetImage(L"monsters"), animation, anchor);

	_graphics.AddData(aniKey, sprite);
}
