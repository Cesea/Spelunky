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
	//void HandlePlayerDeadEvent(const IEvent *event);

private:
	Camera _camera;

private:
	IDWriteTextFormat *_smallText{};
	IDWriteTextFormat *_bigText{};


	IDWriteTextFormat *_deadOverText{};
	IDWriteTextFormat *_deadWhiteText{};
	IDWriteTextFormat *_deadBlackText{};


	InputMapper _inputMapper;

	ObjectId _playerId;
	Player *_pPlayer{};

	PlaySceneState _sceneState;

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

	D2DSprite *_deadBackground{};
	D2DSprite *_deadBook{};
	D2DSprite *_buttonSprite{};

	std::wstring _currentPlayingBGM;

};
#endif