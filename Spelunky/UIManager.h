#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "singletonBase.h"
#include "SpriteObject.h"

class Player;

class UIManager : public singletonBase<UIManager>
{
public :
	UIManager();
	~UIManager();

	HRESULT Init();
	void Release();
	void Update(float deltaTime);
	void Render(const Vector2 &camPos);

	void SetPlayerLink(Player *player) { _pPlayer = player; }

	void SetDrawExitText(bool b) { _drawExitText = b; }
private :
	void HandleOnTunnelEvent(const IEvent *event);
	void HandleOnMiddleStageEvent(const IEvent *event);
	void HandleExitMiddleStageEvent(const IEvent *event);

	void RenderMiddleStageStatus(float deltaTime);

	void SetShowIndexCorrespondingToGemType(GemType type);
	void SetShowIndexCorrespondingToEnemyType(EnemyType type);

private :

	void RegisterDelegates();
	void UnRegisterDelegates();

	D2DSprite *_playerHudSprite{};
	D2DSprite *_moneyHudSprite{};

	D2DSprite *_exitSprite{};

	Player *_pPlayer{};

	IDWriteTextFormat *_whiteText{};
	IDWriteTextFormat *_smallWhiteText{};
	IDWriteTextFormat *_pinkText{};

	bool _drawExitText{false};
	TilePosition _exitPosition{};

	bool _drawMiddleStageUI{ false };
	SpriteObject _board;
	SpriteObject _tunnelTitle;
	SpriteObject _scroll;
	SpriteObject _paper;

	D2DSprite *_uiElements{};
	D2DSprite *_objectElements{};

	std::wstring _timeText{};
	std::wstring _moneyText{};

	bool32 _finishedShowGold{ false };
	Timer _showTimer;
	bool32 _finishedShowEnemy{ false };

	IntVector2 _objectShowingIndex{ -1, -1 };
	int _gemShowValue{ 0 };
	bool32 _endShowing{ false };

};


#endif