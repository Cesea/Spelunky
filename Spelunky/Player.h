#ifndef PLAYER_H
#define PLAYER_H

#include "MovingObject.h"

class Player : public MovingObject
{
	friend class StateManager<Player>;
	friend class IdleState;
	friend class WalkState;

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
	Vector2 _speed{400, 200};
	Vector2 _accel{};
	Vector2 _velocity{};
	Vector2 _maxVelocity{350, 550};

	DataSet<D2DSprite *> _graphics;
	D2DSprite *_currentSprite{};

	Direction _seeingDirection{};

	StateManager<Player> _stateManager;
};

#endif