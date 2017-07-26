#ifndef GAME_PLAY_SCENE_H
#define GAME_PLAY_SCENE_H

#include "IScene.h"
#include "Room.h"

#include "Player.h"

enum class PlaySceneState
{
	OnDungeon,
	PlayerDead
};

class GamePlayScene : public IScene
{
	friend class GameObjectManager;
public:
	GamePlayScene();
	virtual ~GamePlayScene();

	HRESULT Init(void) override;
	void Release(void) override;
	void Update(void) override;
	void Render(void) override;

	HRESULT LoadContent() override;

	void RegisterDelegates();
	void UnRegisterDelegates();

	void HandleLayerOnEvent(const IEvent *event);

private:
	Camera _camera;

private:

	IDWriteTextFormat *_smallText{};
	IDWriteTextFormat *_bigText{};
	//ObjectMap _objects;

	InputMapper _inputMapper;

	ObjectId _playerId;
	Player *_pPlayer{};

	ObjectId _lastId{1};
	ObjectId GetNextId() { return _lastId++; }

	Timer _layerRenderTimer;
	bool _layerRenderOn{false};

	ID2D1RadialGradientBrush *_radialBrush{};
	ID2D1GradientStopCollection *_pGradientStopCollection{};

	Vector2 _layeredCenter;
	float _t;
	float _startLayeredRadius;
	float _targetLayeredRadius;

	bool _updateOthers{true};
	bool _enterTheStage{false};

	bool _exitOnMiddle{false};


};
#endif