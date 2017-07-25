#include "stdafx.h"
#include "Bat.h"

#include "CollisionComponent.h"

#include "BatOnTopState.h"
#include "BatFlyingState.h"

Bat::Bat(ObjectId id)
	:Enemy::Enemy(id)
{
}

Bat::~Bat()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Enemy, &Bat::HandleDamageEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<Enemy, &Bat::HandlePlayerAttackEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Bat, &Bat::HandlePlayerPositionEvent>(this));
	_graphics.Release();
}

HRESULT Bat::Init(BaseProperty * property)
{
	_hp = 1;
	EVENTMANAGER->RegisterDelegate(EVENT_DAMAGE, EventDelegate::FromFunction<Enemy, &Bat::HandleDamageEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<Enemy, &Bat::HandlePlayerAttackEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<Bat, &Bat::HandlePlayerPositionEvent>(this));
	_collisionComp = new CollisionComponent;
	_collisionComp->Init(RectMake(0, 0, 38, 38), Vector2(-19, -44));

	BuildAnimationSprite(L"on_top", IntVector2(-40, -72));
	BuildAnimationSprite(L"fly", IntVector2(-40, -72));

	SetGraphics(L"on_top");

	position.tileX = property->position.x;
	position.tileY = property->position.y;
	position.AddToTileRelX(32);
	position.AddToTileRelY(35);
	desiredPosition = position;

	_stateManager.Init(this, new BatOnTopState);

	_speed = Vector2(30, 30);
	_maxVelocity = Vector2(40, 40);

	_enemyType = EnemyType::ENEMY_Bat;

	return S_OK;
}

void Bat::Release(void)
{
}

void Bat::Update(float deltaTime)
{
	TilePosition centerPos = desiredPosition;
	centerPos.AddToTileRelY(-28.0f);

	_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));
	_stateManager.Update(deltaTime);
	_accel = Vector2();

	desiredPosition.AddToTileRel(_velocity * deltaTime);

	_collisionComp->Update(this, deltaTime, &_nearTiles);

}

void Bat::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position.UnTilelize() -camPos;
	_currentSprite->Render(renderTarget, drawingPos.x, drawingPos.y);


	//const Vector2 itemUntiledPosition = position.UnTilelize();
	//Rect itemAbsRect =
	//	RectMake(itemUntiledPosition.x, itemUntiledPosition.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	//itemAbsRect += _collisionComp->GetOffset();

	//DrawBox(gRenderTarget, itemAbsRect.x - camPos.x, itemAbsRect.y - camPos.y, itemAbsRect.width, itemAbsRect.height, D2D1::ColorF(1.0f, 1.0f, 0.0f));
}

GameObject * Bat::Copy(ObjectId id)
{
	return new Bat(id);
}

void Bat::HandlePlayerPositionEvent(const IEvent * event)
{
	PlayerPositionEvent *convertedEvent = (PlayerPositionEvent *)(event);
	const TilePosition &playerTilePosition =  convertedEvent->GetPosition();
	int tileXDiff = playerTilePosition.tileX - position.tileX;
	int tileYDiff = playerTilePosition.tileY - position.tileY;

	if (abs(tileXDiff) >= 6 || abs(tileYDiff) >= 3)
	{
		return;
	}

	const Vector2 &playerUntiled = playerTilePosition.UnTilelize();
	const Vector2 untiled = position.UnTilelize();

	if (playerUntiled.y > untiled.y && !_flying)
	{
		_stateManager.ChangeState(new BatFlyingState);
		_flying = true;
		return;
	}

	if (_flying)
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

		_accel += diff * 180;
	}


	const Rect &playerRect = convertedEvent->GetRect();

	Rect playerAbsRect = RectMake(playerUntiled.x, playerUntiled.y,
		playerRect.width, playerRect.height);
	playerAbsRect += convertedEvent->GetRectOffset();

	Rect enemyAbsRect =
		RectMake(untiled.x, untiled.y, _collisionComp->GetRect().width, _collisionComp->GetRect().height);
	enemyAbsRect += _collisionComp->GetOffset();

	float relXDiff = playerUntiled.x - untiled.x;
	float relYDiff = playerUntiled.y - untiled.y;

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

void Bat::BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 & anchor)
{
	std::wstring imageKey = L"bat_";
	Animation *animation = new Animation;
	animation->InitCopy(KEYANIMANAGER->FindAnimation(imageKey + aniKey));
	animation->SetOwner(this);
	D2DSprite *sprite = new D2DAnimationSprite;
	sprite->Init(IMAGEMANAGER->GetImage(L"monsters"), animation, anchor);

	_graphics.AddData(aniKey, sprite);
}
