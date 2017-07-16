#include "stdafx.h"
#include "Player.h"

#include "IdleState.h"

Player::Player(ObjectId id)
	:MovingObject::MovingObject(id)
{
	_seeingDirection = Direction::Left;

	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<Player, &Player::HandlePlayerInputEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_FRAME_ENDED, EventDelegate::FromFunction<Player, &Player::HandleFrameEndEvent>(this));

	_rect = RectMake(0, 0, 56, 64);
	_rectOffset = Vector2(-28, -64);
}

Player::~Player()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<Player, &Player::HandlePlayerInputEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_FRAME_ENDED, EventDelegate::FromFunction<Player, &Player::HandleFrameEndEvent>(this));
}

HRESULT Player::Init(ArcheType type)
{
	BuildAnimationSprite(L"idle", IntVector2(-40, -72));
	BuildAnimationSprite(L"walk", IntVector2(-40, -72));
	BuildAnimationSprite(L"crawl", IntVector2(-40, -72));
	BuildAnimationSprite(L"crawlIdle", IntVector2(-40, -72));
	BuildAnimationSprite(L"crawlMove", IntVector2(-40, -72));
	BuildAnimationSprite(L"standUp", IntVector2(-40, -72));
	BuildAnimationSprite(L"lookUp", IntVector2(-40, -72));
	BuildAnimationSprite(L"lookRevert", IntVector2(-40, -72));
	BuildAnimationSprite(L"jump", IntVector2(-40, -72));
	BuildAnimationSprite(L"falling", IntVector2(-40, -72));
	BuildAnimationSprite(L"ladderIdle", IntVector2(-40, -72));
	BuildAnimationSprite(L"ladderClimb", IntVector2(-40, -72));

	SetGraphics(L"idle");

	_stateManager.Init(this, new IdleState);

	return S_OK;
}

void Player::Release(void)
{
	_graphics.Release();
}

void Player::Update(float deltaTime)
{
	_accel.y += GRAVITY;
	_stateManager.Update(deltaTime);
	_accel = Vector2();

	_onGround = false;
	_headHit = false;
	_onWall = false;
	_canGrab = false;
	_onLedge = false;
	_canClimb = false;
	_canClimbUp = false;

	CollisionCheck();
	CheckCurrentTile();
}

void Player::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position.UnTilelize() -camPos;
	_currentSprite->Render(renderTarget, drawingPos.x, drawingPos.y);
}

void Player::Serialize(FileUtils::File & file)
{
}

void Player::DeSerialize(FileUtils::File & file)
{
}

GameObject * Player::Copy(ObjectId id)
{
	return nullptr;
}

void Player::HandlePlayerInputEvent(const IEvent * event)
{
	PlayerInputEvent *convertedEvent = (PlayerInputEvent *)(event);
	const ControlCommand &controlCommand = convertedEvent->GetControlCommand();
	_stateManager.HandleCommand(controlCommand);
}

void Player::HandleFrameEndEvent(const IEvent * event)
{
	//지금 생각으로는 이 이벤트를 오브젝트 메니져에서 관리를 해야 할 것 같다
	//각각의 클래스에서 처리하면 어쨌던 찾아야 한다...
	FrameEndedEvent *convertedEvent = (FrameEndedEvent *)(event);
	_stateManager.HandleFrameEndEvent();
}

void Player::HandleMessage(const IEvent * event)
{
}

void Player::SetGraphics(const std::wstring & key)
{
	D2DSprite *found = _graphics.FindData(key);
	if (found != nullptr)
	{
		if (_currentSprite)
		{
			_currentSprite->GetAnimation()->Stop();
		}
		_currentSprite = found;
		_currentSprite->GetAnimation()->Start();
		_currentSprite->SyncFlip(_seeingDirection);
	}
}

void Player::BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 & anchor)
{
	std::wstring imageKey = L"char_orange";
	Animation *animation = new Animation;
	animation->InitCopy(KEYANIMANAGER->FindAnimation(imageKey + L"_" + aniKey));
	animation->SetOwner(this);
	D2DSprite *sprite = new D2DAnimationSprite;
	sprite->Init(IMAGEMANAGER->GetImage(imageKey), animation, anchor);

	_graphics.AddData(aniKey, sprite);
}

void Player::CollisionCheck()
{

	TilePosition centerTilePos = desiredPosition;
	centerTilePos.AddToTileRelY(-32.0f);

	_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacentTiles9(IntVector2(centerTilePos.tileX, centerTilePos.tileY));

	for (int i = 0; i < 8; ++i)
	{
		if (_nearTiles.tiles[i] == nullptr || 
			_nearTiles.tiles[i]->sourceIndex.x == -1)
			continue;
		Rect absRect = desiredPosition.UnTilelize() + _rect + _rectOffset;
		Rect tileRect = RectMake(_nearTiles.tiles[i]->position.x * TILE_SIZE, _nearTiles.tiles[i]->position.y * TILE_SIZE, TILE_SIZE, TILE_SIZE);

		if (i == 0)
		{

		}
		else
		{
			if (_nearTiles.tiles[i]->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
			{
				Rect overlapRect;
				if (IsRectangleOverlap(absRect, tileRect, overlapRect))
				{
					//아래 타일
					if (i == 0)
					{
						desiredPosition.AddToTileRel(0, -overlapRect.height);
						_velocity.y = 0.0f;
						_onGround = true;
					}
					//위 타일
					else if (i == 1)
					{
						desiredPosition.AddToTileRel(0, overlapRect.height);
						_velocity.y = 0.0f;
						_headHit = true;
					}
					//왼 타일
					else if (i == 2)
					{
						desiredPosition.AddToTileRel(overlapRect.width, 0);
						_velocity.x = 0.0f;
						_onWall = true;
					}
					//오른 타일
					else if (i == 3)
					{
						desiredPosition.AddToTileRel(-overlapRect.width, 0);
						_velocity.x = 0.0f;
						_onWall = true;
					}
					//대각선
					else
					{
						//수직으로 충돌이 일어남
						if (overlapRect.width > overlapRect.height)
						{
							_velocity.y = 0.0f;
							float pushingHeight;
							if (i == 4 || i == 6)
							{
								pushingHeight = -overlapRect.height;
								_onGround = true;
							}
							else
							{
								pushingHeight = overlapRect.height;
								_headHit = true;
							}
							desiredPosition.AddToTileRel(0, pushingHeight);
						}
						//수평으로 충돌이 일어남
						else
						{
							_velocity.x = 0.0f;
							_onWall = true;
							float pushingWidth;
							if (i == 5 || i == 6)
							{
								pushingWidth = overlapRect.width;
							}
							else
							{
								pushingWidth = -overlapRect.width;
							}
							desiredPosition.AddToTileRel(pushingWidth, 0);
						}
					}
				}
			}

		}

	}
	position = desiredPosition;
}

void Player::CheckCurrentTile()
{
	TileCollisionType centerTileCollisionType = _nearTiles.tiles[8]->collisionType;
	TileCollisionType upperTileCollisionType = _nearTiles.tiles[1]->collisionType;
	TileCollisionType lowerTileCollisionType = _nearTiles.tiles[0]->collisionType;
	if (centerTileCollisionType == TILE_COLLISION_LADDER || centerTileCollisionType == TILE_COLLISION_EOF_LADDER)
	{
		if (position.tileRel.x > 20 && position.tileRel.x < 44)
		{
			_canClimb = true;
			_canClimbUp = true;
		}
		if ((lowerTileCollisionType == TILE_COLLISION_EOF_LADDER) &&
			(position.tileRel.y < 4.0f))
		{
			_canClimbUp = false;
		}
	}
}
