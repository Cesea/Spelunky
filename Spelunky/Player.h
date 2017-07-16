#ifndef PLAYER_H
#define PLAYER_H


//모든 상태에서 중력의 영향을 받아야지 제대로 작동함...

#include "MovingObject.h"

class Player : public MovingObject
{
	friend class StateManager<Player>;
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

public :
	Player(ObjectId id);
	virtual ~Player();

	virtual HRESULT Init(ArcheType type);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	virtual void Serialize(FileUtils::File &file);
	virtual void DeSerialize(FileUtils::File &file);

	virtual GameObject *Copy(ObjectId id);

	void HandlePlayerInputEvent(const IEvent *event);
	void HandleFrameEndEvent(const IEvent *event);
	virtual void HandleMessage(const IEvent *event);

	Rect GetRect() { return _rect; }

	D2DSprite *GetCurrentGraphics() { return _currentSprite; }

	void SetGraphics(const std::wstring &key);
	void SetDirection(Direction direction) { _seeingDirection = direction; }
	Direction GetDirection() { return _seeingDirection; }

	//void HandleCommand(IEvent *event);

private :
	void BuildAnimationSprite(const std::wstring & aniKey, const IntVector2 &anchor);

	void CollisionCheck();
	void CheckCurrentTile();

private :
	Vector2 _speed{400, 300};
	Vector2 _accel{};
	Vector2 _velocity{};
	Vector2 _prevVelocity{};
	Vector2 _maxVelocity{300, 550};

	float _dashSpeed{200};
	float _climbSpeed{ 120.0f };

	bool _canGrab{false};
	bool _headHit{ false };
	bool _onGround{ false };
	bool _onLedge{ false };
	bool _onWall{ false };
	bool _canClimb{ false };
	bool _canClimbUp{ false };

	DataSet<D2DSprite *> _graphics;
	D2DSprite *_currentSprite{};

	Direction _seeingDirection{};

	StateManager<Player> _stateManager;

	PlayScene::ReturnTile _nearTiles;
};

#endif