#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "IScene.h"

#include "Camera.h"
#include "SpriteObject.h"
#include "TimeTrigger.h"

#define SPEAR_TIME 0.25f

enum MenuSceneState
{
	Title,
	Menu,
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

enum LOADING_KIND
{
	LOADING_IMAGE,
	LOADING_SOUND,
};


struct ImageResources
{
	ImageResources(const std::wstring &fileName, const std::wstring &keyName)
		:fileName(fileName), keyName(keyName)
	{}
	std::wstring fileName;
	std::wstring keyName;
};
struct SoundResources
{
	SoundResources( const std::wstring &keyName,const std::wstring &fileName, bool bgm, bool loop)
		:fileName(fileName), keyName(keyName), isBackground(bgm), loop(loop)
	{}
	std::wstring fileName;
	std::wstring keyName;
	bool isBackground;
	bool loop;
};

class ImageLoadItem
{
public :
	ImageLoadItem(const std::wstring &fileName, const std::wstring &keyName) 
		:_resources(fileName, keyName)
	{}
	void Load();
	const ImageResources &GetResources() { return _resources; }
private :
	ImageResources _resources;
};

class SoundLoadItem
{
public :
	SoundLoadItem(const std::wstring &fileName, const std::wstring &keyName, bool bgm, bool loop) 
		:_resources(fileName, keyName, bgm, loop)
	{}
	void Load();
	const SoundResources &GetResources() { return _resources; }
private :
	SoundResources _resources;
};

class Loading
{
public :
	Loading() {}
	virtual ~Loading() { }

	void LoadImageResources(const std::wstring &fileName, const std::wstring &keyName);
	void LoadSoundResources(const std::wstring &fileName, const std::wstring &keyName, bool bgm, bool loop);

	float LoadNext(void);

private :

	int _imageCurrent{};
	int _soundCurrent{};

	bool _loadSound{};
	std::vector<ImageLoadItem *> _imageLoadItems;
	std::vector<SoundLoadItem *> _soundLoadItems;

	int _totalSize{};
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

	Loading *_loading;


	Timer _loadStartTimer;
	bool _loadingStart{false};
	Timer _titleTransitionTimer;
	bool _titleTransitionOn{false};
};


#endif