#include "stdafx.h"
#include "StrongSnake.h"

#include "CollisionComponent.h"

#include "StrongSnakeWalkState.h"

StrongSnake::StrongSnake(ObjectId id)
	:Enemy::Enemy(id)
{
}

StrongSnake::~StrongSnake()
{
	//EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<Enemy, &StrongSnake::HandlePlayerAttackEvent>(this));
	//EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<StrongSnake, &StrongSnake::HandlePlayerPositionEvent>(this));
	//_graphics.Release();
}

HRESULT StrongSnake::Init(BaseProperty * property)
{
	//_hp = 1;
	//EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_ATTACK, EventDelegate::FromFunction<Enemy, &StrongSnake::HandlePlayerAttackEvent>(this));
	//EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_POSITION, EventDelegate::FromFunction<StrongSnake, &StrongSnake::HandlePlayerPositionEvent>(this));
	//_collisionComp = new CollisionComponent;
	//_collisionComp->Init(RectMake(0, 0, 38, 44), Vector2(-19, -44));

	//BuildAnimationSprite(L"walk", IntVector2(-40, -72));
	//BuildAnimationSprite(L"attack", IntVector2(-40, -72));
	//BuildAnimationSprite(L"spit", IntVector2(-40, -72));

	//SetGraphics(L"walk");

	//position.tileX = property->position.x;
	//position.tileY = property->position.y;
	//desiredPosition = position;

	//_stateManager.Init(this, new StrongSnakeWalkState);

	//_speed = Vector2(30, 300);
	//_maxVelocity = Vector2(40, 500);

	//return S_OK;
	return S_OK;
}

void StrongSnake::Release(void)
{
}

void StrongSnake::Update(float deltaTime)
{
	//_accel.y += GRAVITY;

	//TilePosition centerPos = desiredPosition;
	//centerPos.AddToTileRelY(-32.0f);

	//_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(centerPos.tileX, centerPos.tileY));
	//_stateManager.Update(deltaTime);
	//_accel = Vector2();

	//desiredPosition.AddToTileRel(_velocity * deltaTime);

	//_collisionComp->Update(this, deltaTime, &_nearTiles);
}

void StrongSnake::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position.UnTilelize() -camPos;
	_currentSprite->Render(renderTarget, drawingPos.x, drawingPos.y);
}

GameObject * StrongSnake::Copy(ObjectId id)
{
	return new StrongSnake(_id);
}

void StrongSnake::HandlePlayerPositionEvent(const IEvent * event)
{
}

void StrongSnake::BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 & anchor)
{
	std::wstring imageKey = L"strong_snake_";
	Animation *animation = new Animation;
	animation->InitCopy(KEYANIMANAGER->FindAnimation(imageKey + aniKey));
	animation->SetOwner(this);
	D2DSprite *sprite = new D2DAnimationSprite;
	sprite->Init(IMAGEMANAGER->GetImage(L"monsters"), animation, anchor);

	_graphics.AddData(aniKey, sprite);
}
