#include "stdafx.h"
#include "Snake.h"

#include "CollisionComponent.h"

#include "SnakeWalkState.h"
#include "SnakeAttackState.h"

Snake::Snake(ObjectId id)
	:Enemy::Enemy(id)
{
}

Snake::~Snake()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Enemy, &Snake::HandleDamageEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<Enemy, &Enemy::HandlePlayerAttackEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Snake, &Snake::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_OBSTACLE_POSITION, EventDelegate::FromFunction<Enemy, &Snake::HandleObstaclePositionEvent>(this));
	_graphics.Release();

}

HRESULT Snake::Init(BaseProperty * property)
{
	_hp = 1;
	EVENTMANAGER->RegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Enemy, &Snake::HandleDamageEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<Enemy, &Snake::HandlePlayerAttackEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Snake, &Snake::HandlePlayerPositionEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_OBSTACLE_POSITION, EventDelegate::FromFunction<Enemy, &Snake::HandleObstaclePositionEvent>(this));
	_collisionComp = new CollisionComponent;
	_collisionComp->Init(RectMake(0, 0, 44, 48), Vector2(-22, -48));

	BuildAnimationSprite(L"walk", IntVector2(-40, -66));
	BuildAnimationSprite(L"attack", IntVector2(-40, -66));

	SetGraphics(L"walk");

	position.tileX = property->position.x;
	position.tileY = property->position.y;
	desiredPosition = position;

	_stateManager.Init(this, new SnakeWalkState);

	_speed = Vector2(30, 300);
	_maxVelocity = Vector2(40, 500);

	_enemyType = EnemyType::ENEMY_Snake;

	return S_OK;
}

void Snake::Release(void)
{
}

void Snake::Update(float deltaTime)
{
	_accel.y += GRAVITY;

	TilePosition centerPos = desiredPosition;
	centerPos.AddToTileRelY(-32.0f);

	_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));
	_stateManager.Update(deltaTime);
	_accel = Vector2();

	desiredPosition.AddToTileRel(_velocity * deltaTime);

	_collisionComp->Update(this, deltaTime, &_nearTiles);

	if (_eventDispatchTimer.Tick(deltaTime))
	{
		EVENTMANAGER->QueueEvent(new EnemyPositionEvent(_id, position, _collisionComp->GetRect(), _collisionComp->GetOffset()));
	}

}

void Snake::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position.UnTilelize() -camPos;
	_currentSprite->Render(renderTarget, drawingPos.x, drawingPos.y);

	//const Vector2 itemUntiledPosition = position.UnTilelize();
	//Rect itemAbsRect =
	//	RectMake(itemUntiledPosition.x, itemUntiledPosition.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	//itemAbsRect += _collisionComp->GetOffset();

	//DrawBox(gRenderTarget, itemAbsRect.x - camPos.x, itemAbsRect.y - camPos.y, itemAbsRect.width, itemAbsRect.height, D2D1::ColorF(1.0f, 1.0f, 0.0f));
}

GameObject * Snake::Copy(ObjectId id)
{
	return new Snake(id);
}

void Snake::HandlePlayerPositionEvent(const IEvent * event)
{
	PlayerPositionEvent *convertedEvent = (PlayerPositionEvent *)(event);
	const TilePosition &playerTilePosition =  convertedEvent->GetPosition();
	int tileXDiff = playerTilePosition.tileX - position.tileX;
	int tileYDiff = playerTilePosition.tileY - position.tileY;

	if (abs(tileXDiff) >= 3 || abs(tileYDiff) >= 3)
	{
		return;
	}

	const Vector2 &playerUntiled = playerTilePosition.UnTilelize();
	const Vector2 untiled = position.UnTilelize();
	const Rect &playerRect = convertedEvent->GetRect();

	Rect playerAbsRect = RectMake(playerUntiled.x, playerUntiled.y,
		playerRect.width, playerRect.height);
	playerAbsRect += convertedEvent->GetRectOffset();

	Rect itemAbsRect =
		RectMake(untiled.x, untiled.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	itemAbsRect += _collisionComp->GetOffset();

	float relXDiff = playerUntiled.x - untiled.x;
	float relYDiff = playerUntiled.y - untiled.y;

	Rect overlapRect;
	if (IsRectangleOverlap(playerAbsRect, itemAbsRect, overlapRect))
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
	if (!_attacking)
	{
		if (abs(relXDiff) <= 40 && abs(relYDiff) < 25 && !_attacking)
		{
			_stateManager.ChangeState(new SnakeAttackState());
		}
	}
}

void Snake::BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 & anchor)
{
	std::wstring imageKey = L"snake_";
	Animation *animation = new Animation;
	animation->InitCopy(KEYANIMANAGER->FindAnimation(imageKey + aniKey));
	animation->SetOwner(this);
	D2DSprite *sprite = new D2DAnimationSprite;
	sprite->Init(IMAGEMANAGER->GetImage(L"monsters"), animation, anchor);

	_graphics.AddData(aniKey, sprite);
}
