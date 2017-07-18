#include "stdafx.h"
#include "Player.h"

#include "IdleState.h"

Player::Player(ObjectId id)
	:MovingObject::MovingObject(id)
{
	_seeingDirection = Direction::Left;

	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<Player, &Player::HandlePlayerInputEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_FRAME_ENDED, EventDelegate::FromFunction<Player, &Player::HandleFrameEndEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_COLLECT_MONEY, EventDelegate::FromFunction<Player, &Player::HandleCollectedMoneyEvent>(this));

	_rect = RectMake(0, 0, 56, 64);
	_rectOffset = Vector2(-28, -64);

	_speed = Vector2(460, 300);
	_maxVelocity = Vector2(340, 630);
}

Player::~Player()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_INPUT, EventDelegate::FromFunction<Player, &Player::HandlePlayerInputEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_FRAME_ENDED, EventDelegate::FromFunction<Player, &Player::HandleFrameEndEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_COLLECT_MONEY, EventDelegate::FromFunction<Player, &Player::HandleCollectedMoneyEvent>(this));
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
	BuildAnimationSprite(L"onLedge", IntVector2(-40, -72));
	BuildAnimationSprite(L"ledgeGrab", IntVector2(-40, -72));
	BuildAnimationSprite(L"grab", IntVector2(-40, -72));
	BuildAnimationSprite(L"upperDeath", IntVector2(-40, -72));

	BuildAnimationSprite(L"attack", IntVector2(-40, -72));
	//BuildAnimationSprite(L"throw", IntVector2(-40, -72));

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
	_upperDeath = false;

	if (!_interpolating)
	{
		CollisionCheck();
		CheckCurrentTile();
	}
	EVENTMANAGER->QueueEvent(new PlayerPositionEvent(_id, position, _rect, _rectOffset));
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

	if (controlCommand.dash == Command::DashOn)
	{
		_maxVelocity.x += 150;
		_speed.x += 50;
	}
	else if (controlCommand.dash == Command::DashOff)
	{
		_maxVelocity.x -= 150;
		_speed.x -= 50;
	}
	_stateManager.HandleCommand(controlCommand);
}

void Player::HandleFrameEndEvent(const IEvent * event)
{
	//지금 생각으로는 이 이벤트를 오브젝트 메니져에서 관리를 해야 할 것 같다
	//각각의 클래스에서 처리하면 어쨌던 찾아야 한다...
	FrameEndedEvent *convertedEvent = (FrameEndedEvent *)(event);
	_stateManager.HandleFrameEndEvent();
}

void Player::HandleCollectedMoneyEvent(const IEvent * event)
{
	CollectMoneyEvent *convertedEvent = (CollectMoneyEvent *)event;
	_money += convertedEvent->GetValue();
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

		PlayScene::Tile *currentTile = _nearTiles.tiles[i];

		Rect absRect = desiredPosition.UnTilelize() + _rect + _rectOffset;
		Rect tileRect = RectMake(currentTile->position.x * TILE_SIZE, currentTile->position.y * TILE_SIZE, TILE_SIZE, TILE_SIZE);

		Rect overlapRect;
		//아래 타일의 경우 체크를 해 주어야 할 경우가 있다. 사다리, 위에서 떨어지면 죽는 타일 등....
		if (i == 0)
		{
			if (IsRectangleOverlap(absRect, tileRect, overlapRect))
			{
				if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
				{
					desiredPosition.AddToTileRel(0, -overlapRect.height);
					_velocity.y = 0.0f;
					_onGround = true;
				}
				else if ((currentTile->collisionType == TileCollisionType::TILE_COLLISION_EOF_LADDER) && 
							(_velocity.y >= 0.0f) && (desiredPosition.tileRel.y < 10) &&
							(!_stateClimbing))
				{
					desiredPosition.AddToTileRel(0, -overlapRect.height);
					_velocity.y = 0.0f;
					_canClimb = true;
					_onGround = true;
				}
			}
		}
		else
		{
			if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
			{
				if (IsRectangleOverlap(absRect, tileRect, overlapRect))
				{
					//위 타일
					if (i == 1)
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

	TileCollisionType lowerLeftTileCollisionType = _nearTiles.tiles[6]->collisionType;
	TileCollisionType lowerRightTileCollisionType = _nearTiles.tiles[4]->collisionType;

	TileCollisionType leftTileCollisionType = _nearTiles.tiles[2]->collisionType;
	TileCollisionType rightTileCollisionType = _nearTiles.tiles[3]->collisionType;

	TileCollisionType upperLeftTileCollisionType = _nearTiles.tiles[5]->collisionType;
	TileCollisionType upperRightTileCollisionType = _nearTiles.tiles[7]->collisionType;

	if (centerTileCollisionType == TILE_COLLISION_UPPER_DEAD && position.tileRel.y < 40)
	{
		_upperDeath = true;
		return;
	}

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

	if ((lowerTileCollisionType == TILE_COLLISION_BLOCK || lowerTileCollisionType == TILE_COLLISION_EOF_LADDER) &&
		(lowerLeftTileCollisionType != TILE_COLLISION_BLOCK))
	{
		if (position.tileRel.x < 8)
		{
			_onLedge = true;
		}
	}
	if ((lowerTileCollisionType == TILE_COLLISION_BLOCK || lowerTileCollisionType == TILE_COLLISION_EOF_LADDER) &&
		(lowerRightTileCollisionType != TILE_COLLISION_BLOCK))
	{
		if (position.tileRel.x > 56)
		{
			_onLedge = true;
		}
	}

	if ((!_onGround) &&
		(leftTileCollisionType == TILE_COLLISION_BLOCK) &&
		(upperLeftTileCollisionType == TILE_COLLISION_NONE) &&
		(_seeingDirection == Direction::Left) &&
		(position.tileRel.x < 36.0f) &&
		desiredPosition.tileRel.y > 40.0f && desiredPosition.tileRel.y < 46.0f)
	{
		_canGrab = true;
	}
	else if ((!_onGround) &&
		(rightTileCollisionType == TILE_COLLISION_BLOCK) &&
		(upperRightTileCollisionType == TILE_COLLISION_NONE) &&
		(_seeingDirection == Direction::Right) &&
		(position.tileRel.x > 28.0f) &&
		desiredPosition.tileRel.y > 40.0f && desiredPosition.tileRel.y < 46.0f)
	{
		_canGrab = true;
	}
}
