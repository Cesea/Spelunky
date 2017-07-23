#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "IScene.h"

#include "Camera.h"
#include "SpriteObject.h"
#include "TimeTrigger.h"

#define SPEAR_TIME 0.25f

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
};

struct MenuObjects
{
	SpriteObject doorBack;
	SpriteObject arch;
	SpriteObject leftStatues;
	SpriteObject rightStatues;

	SpriteObject leftCaveSide;
	SpriteObject rightCaveSide;

	SpriteObject sandDirt;

	SpriteObject alpha;


	SpriteObject doors[4];
	SpriteObject body;
	SpriteObject head;

	SpriteObject spears[6];
	SpriteObject ornaments[6];

	int animationEndTracker{ 0 };

	int selectingMenuIndex{ 0 };
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

	//private jtions
private:

	void ChangeCurrentSceneState(MenuSceneState state);
	void MenuAnimationEndFunction();

	void ShowTitleText();

	void HandleSceneChange();


private :
	bool _firstEntered{ true };

	Camera _camera;

	MenuSceneState _currentState{MenuSceneState::Title};

	TitleObjects _titleObjects;
	MenuObjects _menuObjects;

	ObjectId _lastId{ 1 };
	ObjectId GetNextId() { return _lastId++; }

	Timer _batTimer;
	int _currentBatTracker{ 0 };

	IDWriteTextFormat *_menuNormalText{};
	IDWriteTextFormat *_menuYellowText{};

	WCHAR *_menuTexts[6];

	float _textAlpha{ false };
	bool _showText{ false };

	bool _canReceiveInput{ false };

	TimeTrigger _menuFirstTrigger;
	TimeTrigger _menuSecondTrigger;
};


#endif