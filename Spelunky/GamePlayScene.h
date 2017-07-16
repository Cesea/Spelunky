#ifndef GAME_PLAY_SCENE_H
#define GAME_PLAY_SCENE_H

#include "IScene.h"

#include "Room.h"

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

	void CreateAndPlaceObject(ArcheType type, const TilePosition &position);

private:
	Camera _camera;

private:
	//ObjectMap _objects;

	InputMapper _inputMapper;

	ObjectId _playerId;

	ObjectId _lastId{1};
	ObjectId GetNextId() { return _lastId++; }

};
#endif