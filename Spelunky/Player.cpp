#include "stdafx.h"
#include "Player.h"

#include "IdleState.h"
#include "JumpState.h"
#include "FaintState.h"
#include "FallingState.h"
#include "PushingState.h"

Player::Player(ObjectId id)
	:MovingObject::MovingObject(id)
{
	_seeingDirection = Direction::Left;

	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_INPUT, 
		EventDelegate::FromFunction<Player, &Player::HandlePlayerInputEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_FRAME_ENDED, 
		EventDelegate::FromFunction<Player, &Player::HandleFrameEndEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_COLLECT_MONEY, 
		EventDelegate::FromFunction<Player, &Player::HandleCollectedMoneyEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_HOLDING, 
		EventDelegate::FromFunction<Player, &Player::HandleHoldingEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_ON_TUNNEL, 
		EventDelegate::FromFunction<Player, &Player::HandleOnTunnelEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_GO_EXIT, 
		EventDelegate::FromFunction<Player, &Player::HandlePlayerGoExitEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_UPPER_JUMP, 
		EventDelegate::FromFunction<Player, &Player::HandlePlayerUpperJumpEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PLAYER_DAMAGED, 
		EventDelegate::FromFunction<Player, &Player::HandlePlayerDamagedEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_PUSHING_OBJECT, 
		EventDelegate::FromFunction<Player, &Player::HandlePushingObjectEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_ITEM_BREAK, 
		EventDelegate::FromFunction<Player, &Player::HandleItemBreakEvent>(this));

	_rect = RectMake(0, 0, 38, 44);
	_rectOffset = Vector2(-19, -44);

	_speed = Vector2(460, 300);
	_maxVelocity = Vector2(340, 630);
}

Player::~Player()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_INPUT, 
		EventDelegate::FromFunction<Player, &Player::HandlePlayerInputEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_FRAME_ENDED, 
		EventDelegate::FromFunction<Player, &Player::HandleFrameEndEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_COLLECT_MONEY, 
		EventDelegate::FromFunction<Player, &Player::HandleCollectedMoneyEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_HOLDING, 
		EventDelegate::FromFunction<Player, &Player::HandleHoldingEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_ON_TUNNEL, 
		EventDelegate::FromFunction<Player, &Player::HandleOnTunnelEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_GO_EXIT, 
		EventDelegate::FromFunction<Player, &Player::HandlePlayerGoExitEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_UPPER_JUMP, 
		EventDelegate::FromFunction<Player, &Player::HandlePlayerUpperJumpEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PLAYER_DAMAGED, 
		EventDelegate::FromFunction<Player, &Player::HandlePlayerDamagedEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_PUSHING_OBJECT, 
		EventDelegate::FromFunction<Player, &Player::HandlePushingObjectEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_ITEM_BREAK, 
		EventDelegate::FromFunction<Player, &Player::HandleItemBreakEvent>(this));
}

HRESULT Player::Init(BaseProperty *property)
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
	BuildAnimationSprite(L"pushing", IntVector2(-40, -72));

	BuildAnimationSprite(L"attack", IntVector2(-40, -72));
	BuildAnimationSprite(L"throw", IntVector2(-40, -72));

	BuildAnimationSprite(L"exit", IntVector2(-40, -72));
	BuildAnimationSprite(L"faint", IntVector2(-40, -72));

	BuildWeaponAnimationSprite(L"whip", IntVector2(-40, -90));
	BuildWeaponAnimationSprite(L"mattock", IntVector2(-40, -90));
	BuildWeaponAnimationSprite(L"machete", IntVector2(-40, -90));

	SetGraphics(L"idle");

	_stateManager.Init(this, new IdleState);

	_exitTimer.Init(1.3f);
	_vulnerableTimer.Init(1.0f);

	playerBombEatFunction = Delegate<void, int>::FromFunction<Player, &Player::EatBomb>(this);
	playerRopeEatFunction = Delegate<void, int>::FromFunction<Player, &Player::EatRope>(this);

	return S_OK;
}

void Player::Release(void)
{
	_graphics.Release();
}

void Player::Update(float deltaTime)
{
	if (_canControl)
	{
		_accel.y += GRAVITY;
		_stateManager.Update(deltaTime);
		_accel = Vector2();

		if (_onGround)
		{
			_isFalling = false;
		}

		_onGround = false;
		_headHit = false;
		_onWall = false;
		_canGrab = false;
		_onLedge = false;
		_canClimb = false;
		_canClimbUp = false;
		_upperDeath = false;
		_endOfLadder = false;
		_onTunnel = false;
		_onObject = false;

		if (!_vulnerable && !_isFaint)
		{
			if (_vulnerableTimer.Tick(deltaTime))
			{
				_vulnerable = true;
			}
		}

		if (!_interpolating)
		{
			CollisionCheck();
		}

		//Console::Log("fallig : %d\n", _isFalling);
		EVENTMANAGER->QueueEvent(new PlayerPositionEvent(_id, position, _rect, _rectOffset, _isFalling));
	}
	else
	{
		if (_moveToExitInterpolating)
		{
			if (_exitTimer.Tick(deltaTime))
			{
				_moveToExitInterpolating = false;
				position = _exitPosition;
				_exitTimer.ResetAndChangeTargetSecond(1.3f);
				SOUNDMANAGER->Play(L"into_door");
				SetGraphics(L"exit");
			}
			else
			{
				float t = _exitTimer.GetCurrentSecond() / _exitTimer.GetTargetSecond();
				position = InterpolateTilePosition(_exitStartPosition, _exitPosition, t);
			}
			_currentSprite->Update(deltaTime);
		}
		else
		{
			if (_exitOnMiddleStage)
			{
				if (KEYMANAGER->IsOnceKeyDown('Q'))
				{
					EVENTMANAGER->DiscardAllEvents();
					EVENTMANAGER->FireEvent(new LayerOnEvent(false, true, _dead, position));
					return;
				}
				_currentSprite->Update(deltaTime);
			}
			else
			{
				if (_exitTimer.Tick(deltaTime))
				{
					EVENTMANAGER->DiscardAllEvents();
					EVENTMANAGER->FireEvent(new LayerOnEvent(false, true, _dead, position));
					return;
				}
				_currentSprite->Update(deltaTime);
			}
		}
	}
}

void Player::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawingPos = position.UnTilelize() -camPos;

	if (_currentWeaponSprite)
	{
		_currentWeaponSprite->Render(renderTarget, drawingPos.x + _weaponOffset.x, drawingPos.y + _weaponOffset.y);
	}
	if (!_vulnerable)
	{
		static int vulnerRender = 0;
		if (((vulnerRender++) % 6) == 0)
		{
			_currentSprite->Render(renderTarget, drawingPos.x, drawingPos.y);
		}
	}
	else
	{
		_currentSprite->Render(renderTarget, drawingPos.x, drawingPos.y);
	}

	for (int i = 0; i < 2; ++i)
	{
		if (_holdingObject[i])
		{
			_holdingObject[i]->Render(renderTarget, camPos);
		}
	}
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
		_maxVelocity.x = 340;
		_speed.x -= 50;
	}
	if (controlCommand.action == Command::GoExit && _onTunnel)
	{
		EVENTMANAGER->QueueEvent(new PlayerGoExitEvent(false));
	}

	if (controlCommand.action == Command::Dig)
	{
		_digging = true;
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

void Player::HandleHoldingEvent(const IEvent * event)
{
	HoldingEvent *convertedEvent = (HoldingEvent *)(event);
	_holding = true;
	EquipSlot slotType = convertedEvent->GetSlot();
	if (slotType == EquipSlot::Weapon)
	{
		_holdingObject[0] = (EquipItem *)OBJECTMANAGER->FindObjectId(convertedEvent->GetId());
		SOUNDMANAGER->Play(L"pick_up");
	}
	else if (slotType == EquipSlot::Jump)
	{
		_holdingObject[1] = (EquipItem *)OBJECTMANAGER->FindObjectId(convertedEvent->GetId());
	}
}

void Player::HandleOnTunnelEvent(const IEvent * event)
{
	_onTunnel = true;
}

void Player::HandlePlayerGoExitEvent(const IEvent * event)
{
	PlayerGoExitEvent *convertedEvent = (PlayerGoExitEvent *)event;
	if (convertedEvent->GetIsMiddle())
	{
		_exitTimer.ResetAndChangeTargetSecond(4.0f);
	}
	else
	{
		_exitTimer.ResetAndChangeTargetSecond(1.3f);
	}

	_exitOnMiddleStage = convertedEvent->GetIsMiddle();

	_canControl = false;
	_exitStartPosition = position;
	_moveToExitInterpolating = true;

	if (_exitPosition.UnTilelize().x > position.UnTilelize().x)
	{
		_seeingDirection = Direction::Right;
	}
	else
	{
		_seeingDirection = Direction::Left;
	}
	SetGraphics(L"walk");
}

void Player::HandlePlayerUpperJumpEvent(const IEvent * event)
{
	PlayerUpperJumpEvent *convertedEvent = (PlayerUpperJumpEvent *)(event);
	_jumpPower -= 100;
	_stateManager.ChangeState(new JumpState);
	_jumpPower += 100;
}

void Player::HandlePlayerDamagedEvent(const IEvent * event)
{
	PlayerDamagedEvent *convertedEvent = (PlayerDamagedEvent *)(event);
	if (_vulnerable)
	{
		Enemy *convertedObject = (Enemy *)OBJECTMANAGER->FindObjectId(convertedEvent->GetAttackerId());
		if (convertedObject)
		{
			SOUNDMANAGER->Play(L"hit");
			_lastEnemyHittedType = convertedObject->GetEnemyType();
			_hp -= convertedEvent->GetDamage();
			_vulnerable = false;
			Vector2 enemyPosDiff = convertedEvent->GetPosDiff();
			enemyPosDiff.Normalize();
			enemyPosDiff *= 500;
			enemyPosDiff.y -= 300;
			_velocity = enemyPosDiff;
			_stateManager.ChangeState(new FaintState());

			if (_hp <= 0)
			{
				_hp = 0;
			}
		}
	}
}

void Player::HandlePushingObjectEvent(const IEvent * event)
{
	if (!_pushingObject && _onGround)
	{
		_stateManager.ChangeState(new PushingState);
	}
}

void Player::HandleItemBreakEvent(const IEvent * event)
{
	ItemBreakEvent *convertedEvent = (ItemBreakEvent *)(event);
	if (_holding)
	{
		if (_holdingObject[0])
		{
			if (_holdingObject[0]->GetId() == convertedEvent->GetId())
			{
				_holdingObject[0] = nullptr;
			}
		}
	}
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

void Player::SetWeaponGraphics(const std::wstring & key)
{
	D2DSprite *found = _weaponGraphics.FindData(key);
	if (found != nullptr)
	{
		if (_currentWeaponSprite)
		{
			_currentWeaponSprite->GetAnimation()->Stop();
		}
		_currentWeaponSprite = found;
		_currentWeaponSprite->GetAnimation()->Start();
		_currentWeaponSprite->SyncFlip(_seeingDirection);
	}
}

void Player::EndWeaponGraphics()
{
	_currentWeaponSprite = nullptr;
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

void Player::BuildWeaponAnimationSprite(const std::wstring & aniKey, const IntVector2 & anchor)
{
	std::wstring imageKey = L"weapon_";
	Animation *animation = new Animation;
	animation->InitCopy(KEYANIMANAGER->FindAnimation(imageKey + aniKey));
	animation->SetOwner(this);
	animation->SetEndFunction(Delegate<void>::FromFunction<Player, &Player::EndWeaponGraphics>(this));
	D2DSprite *sprite = new D2DAnimationSprite;
	sprite->Init(IMAGEMANAGER->GetImage(L"weaponanimation"), animation, anchor);

	_weaponGraphics.AddData(aniKey, sprite);
}

void Player::CollisionCheck()
{
	TilePosition centerTilePos = desiredPosition;
	centerTilePos.AddToTileRelY(-32.0f);

	Stage *currentStage = STAGEMANAGER->GetCurrentStage();
	if (currentStage)
	{
		_nearTiles = currentStage->GetAdjacent9(IntVector2(centerTilePos.tileX, centerTilePos.tileY));

		for (int i = 0; i < 8; ++i)
		{
			if (_nearTiles.tiles[i] == nullptr)
				continue;

			Tile *currentTile = _nearTiles.tiles[i];

			Rect absRect = desiredPosition.UnTilelize() + _rect + _rectOffset;
			Rect tileRect = RectMake(currentTile->position.tileX * TILE_SIZE, currentTile->position.tileY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

			Rect overlapRect;
			//아래 타일의 경우 체크를 해 주어야 할 경우가 있다. 사다리, 위에서 떨어지면 죽는 타일 등....
			if (i == 0)
			{
				if (IsRectangleOverlap(absRect, tileRect, overlapRect))
				{
					if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
					{
						desiredPosition.AddToTileRel(0, -overlapRect.height - 0.01f);
						if (_collisionRepulse)
						{
							_velocity.y *= -0.3f;
							_velocity.x *= 0.6;
						}
						else
						{
							_velocity.y = 0.0f;
						}
						_onGround = true;
						_isFalling = false;
					}
					else if ((currentTile->collisionType == TileCollisionType::TILE_COLLISION_EOF_LADDER) &&
						(_velocity.y >= 0.0f) && (desiredPosition.tileRel.y < 10) &&
						(!_stateClimbing))
					{

						desiredPosition.AddToTileRel(0, -overlapRect.height);

						if (_collisionRepulse)
						{
							_velocity.y *= -0.3f;
							_velocity.x *= 0.6;
						}
						else
						{
							_velocity.y = 0.0f;
						}
						_canClimb = true;
						_onGround = true;
						_isFalling = false;
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
							if (_collisionRepulse)
							{
								_velocity.y = 0.0f;
								_velocity.x *= 0.6;
							}
							else
							{
								_velocity.y = 0.0f;
							}
							_headHit = true;
						}
						//왼 타일
						else if (i == 2)
						{
							desiredPosition.AddToTileRel(overlapRect.width, 0);
							if (_collisionRepulse) { _velocity.x *= -0.6f; }
							else { _velocity.x = 0.0f; }
							_onWall = true;
						}
						//오른 타일
						else if (i == 3)
						{
							desiredPosition.AddToTileRel(-overlapRect.width, 0);
							if (_collisionRepulse) { _velocity.x *= -0.6f; }
							else { _velocity.x = 0.0f; }
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
		CheckCurrentTile();
	}
}

void Player::CheckCurrentTile()
{
	TileCollisionType centerColType = (_nearTiles.tiles[8]) ? 
		_nearTiles.tiles[8]->collisionType : TileCollisionType::TILE_COLLISION_NONE;
	TileCollisionType upperColType = (_nearTiles.tiles[1]) ? 
		_nearTiles.tiles[1]->collisionType : TileCollisionType::TILE_COLLISION_NONE;
	TileCollisionType lowerColType = (_nearTiles.tiles[0]) ? 
		_nearTiles.tiles[0]->collisionType : TileCollisionType::TILE_COLLISION_NONE;

	TileCollisionType leftColType = (_nearTiles.tiles[2]) ? 
		_nearTiles.tiles[2]->collisionType : TileCollisionType::TILE_COLLISION_NONE;
	TileCollisionType rightColType = (_nearTiles.tiles[3]) ? 
		_nearTiles.tiles[3]->collisionType : TileCollisionType::TILE_COLLISION_NONE;


	TileCollisionType lowerLeftColType = (_nearTiles.tiles[6]) ? 
		_nearTiles.tiles[6]->collisionType : TileCollisionType::TILE_COLLISION_NONE;
	TileCollisionType lowerRightColType = (_nearTiles.tiles[4]) ? 
		_nearTiles.tiles[4]->collisionType : TileCollisionType::TILE_COLLISION_NONE;

	TileCollisionType upperLeftColType = (_nearTiles.tiles[5]) ? 
		_nearTiles.tiles[5]->collisionType : TileCollisionType::TILE_COLLISION_NONE;
	TileCollisionType upperRightColType = (_nearTiles.tiles[7]) ? 
		_nearTiles.tiles[7]->collisionType : TileCollisionType::TILE_COLLISION_NONE;

	if (centerColType == TILE_COLLISION_LADDER && lowerColType == TILE_COLLISION_BLOCK)
	{
		_endOfLadder = true;
	}

	if (centerColType == TILE_COLLISION_UPPER_DEAD && position.tileRel.y < 40)
	{
		_upperDeath = true;
		return;
	}

	if (centerColType == TILE_COLLISION_LADDER || centerColType == TILE_COLLISION_EOF_LADDER)
	{
		if (position.tileRel.x > 20 && position.tileRel.x < 44)
		{
			_canClimb = true;
			_canClimbUp = true;
		}
		if ((lowerColType == TILE_COLLISION_EOF_LADDER) &&
			(position.tileRel.y < 4.0f))
		{
			_canClimbUp = false;
		}
	}

	if ((lowerColType == TILE_COLLISION_BLOCK || lowerColType == TILE_COLLISION_EOF_LADDER) &&
		(lowerLeftColType != TILE_COLLISION_BLOCK))
	{
		if (position.tileRel.x < 8)
		{
			_onLedge = true;
		}
	}
	if ((lowerColType == TILE_COLLISION_BLOCK || lowerColType == TILE_COLLISION_EOF_LADDER) &&
		(lowerRightColType != TILE_COLLISION_BLOCK))
	{
		if (position.tileRel.x > 56)
		{
			_onLedge = true;
		}
	}
	if ((!_onGround) &&
		(leftColType == TILE_COLLISION_BLOCK) &&
		(upperLeftColType != TILE_COLLISION_BLOCK) &&
		(_seeingDirection == Direction::Left) &&
		(position.tileRel.x < 36.0f) &&
		desiredPosition.tileRel.y > 40.0f && desiredPosition.tileRel.y < 46.0f)
	{
		_canGrab = true;
	}
	else if ((!_onGround) &&
		(rightColType == TILE_COLLISION_BLOCK) &&
		(upperRightColType != TILE_COLLISION_BLOCK) &&
		(_seeingDirection == Direction::Right) &&
		(position.tileRel.x > 28.0f) &&
		desiredPosition.tileRel.y > 40.0f && desiredPosition.tileRel.y < 46.0f)
	{
		_canGrab = true;
	}
}

void Player::Reset()
{
	_canControl = true;
	_stateManager.ChangeState(new FallingState);
}

void Player::Damaged()
{
}
