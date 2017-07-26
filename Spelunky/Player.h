#ifndef PLAYER_H
#define PLAYER_H

//모든 상태에서 중력의 영향을 받아야지 제대로 작동함...

enum class StateType
{
	None, Walk, Climb, Air
};

#include "MovingObject.h"
#include "EquipItem.h"

class Player : public MovingObject
{
	friend class UIManager;

	friend class PlayerStateManager;
	friend class IdleState;
	friend class WalkState;
	friend class CrawlState;
	friend class CrawlIdleState;
	friend class CrawlMoveState;
	friend class StandUpState;
	friend class LookUpState;
	friend class LookRevertState;
	friend class JumpState;
	friend class FallingState;
	friend class LadderClimbState;
	friend class LadderIdleState;
	friend class OnLedgeState;
	friend class LedgeGrabState;
	friend class UpperDeathState;
	friend class AttackState;
	friend class ThrowState;
	friend class FaintState;

public :
	Player(ObjectId id);
	virtual ~Player();

	virtual HRESULT Init(BaseProperty *property);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual void Serialize(FileUtils::File &file);
	virtual void DeSerialize(FileUtils::File &file);

	virtual GameObject *Copy(ObjectId id);

	void HandlePlayerInputEvent(const IEvent *event);
	void HandleFrameEndEvent(const IEvent *event);
	void HandleCollectedMoneyEvent(const IEvent *event);
	void HandleHoldingEvent(const IEvent *event);
	void HandleOnTunnelEvent(const IEvent *event);
	void HandlePlayerGoExitEvent(const IEvent *event);
	void HandlePlayerUpperJumpEvent(const IEvent *event);
	void HandlePlayerDamagedEvent(const IEvent *event);

	virtual void HandleMessage(const IEvent *event);

	Rect GetRect() { return _rect; }

	D2DSprite *GetCurrentGraphics() { return _currentSprite; }
	D2DSprite *GetCurrentWeaponGraphics() { return _currentWeaponSprite; }

	void SetGraphics(const std::wstring &key);

	int GetMoney() { return _money; }
	int GetHp() { return _hp; }
	int GetRopeCount() { return _rope; }
	int GetBombCount() { return _bomb; }

	bool32 GetDead() { return _dead; }

	void SetWeaponGraphics(const std::wstring &key);
	void EndWeaponGraphics();

	void SetExitPosition(const TilePosition &exitPosition) { _exitPosition = exitPosition; }
	void Reset();
private :
	void BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 &anchor);
	void BuildWeaponAnimationSprite(const std::wstring & aniKey, const IntVector2 &anchor);

	void CollisionCheck();
	void CheckCurrentTile();


	void Damaged();

private :
	Rect _rect;
	Vector2 _rectOffset;

	float _dashSpeed{200};
	float _climbSpeed{ 120.0f };

	bool _canControl{ true };

	bool _canGrab{false};
	bool _wasGrab{ false };
	bool _headHit{ false };
	bool _onLedge{ false };
	bool _onWall{ false };
	bool _canClimb{ false };
	bool _canClimbUp{ false };
	bool _upperDeath{ false };

	bool _stateClimbing{ false };

	bool _interpolating{ false };

	bool _climbing{ false };

	bool _onTunnel{ false };
	bool _moveToExitInterpolating{ false };

	bool _stickyBomb{ false };

	bool32 _endOfLadder{ false };
	bool32 _isFalling{ false };
	bool32 _collisionRepulse{ false };
	bool32 _isFaint{ false };
	bool32 _vulnerable{ true };
	bool32 _dead{ false };
	Timer _vulnerableTimer;

	bool32 _digging{ false };


	DataSet<D2DSprite *> _graphics;
	D2DSprite *_currentSprite{};

	DataSet<D2DSprite *> _weaponGraphics;
	D2DSprite *_currentWeaponSprite{};

	StateManager<Player> _stateManager;

	ReturnTile _nearTiles;
	int _money{ 0 };

	int _hp{ 2 };
	uint32 _bomb{ 4 };
	uint32 _rope{ 4 };

	GameObject *_holdingObject[2]{NULL, NULL};

	Vector2 _weaponOffset{};
	int _offsetCount{ 0 };

	EquipItem *_weaponHoldingItem{};
	EquipItem *_jumpHoldingItem{};

	TilePosition _exitStartPosition{};
	TilePosition _exitPosition{};

	Timer _exitTimer;
	bool32 _exitOnMiddleStage{ false };

	float _jumpPower{530};

	EnemyType _lastEnemyHittedType;


};

#endif