#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "IScene.h"

#include "SpriteObject.h"

enum MenuSceneState
{
	CutScene,
	Title,
	Menu,
	CharSelect

};

struct TitleObjects
{
	SpriteObject title;
	SpriteObject player;
	SpriteObject ground;

	SpriteObject alpha;
	SpriteObject bat[8];

	~TitleObjects()
	{
	}
};

class MenuScene : public IScene
{
public:
	MenuScene();
	virtual ~MenuScene();

	virtual HRESULT Init(void) override;
	virtual void Release(void) override;
	virtual void Update(void) override;
	virtual void Render(void) override;

	virtual HRESULT LoadContent() override;

	//private functions
private:
	MenuSceneState _currentState{MenuSceneState::Title};


	TitleObjects _titleObjects;
	//SpriteObject _title;
	//SpriteObject _titlePlayer;
	//SpriteObject _titleGround;
	//SpriteObject _titleBat;

	Timer _batTimer;
	int _currentBatTracker{ 0 };
};


#endif