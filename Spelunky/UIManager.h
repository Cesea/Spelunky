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

private :

	void RegisterDelegates();
	void UnRegisterDelegates();

	D2DSprite *_playerHudSprite{};
	D2DSprite *_moneyHudSprite{};

	D2DSprite *_exitSprite{};

	Player *_pPlayer{};

	IDWriteTextFormat *_smallBlackText{};
	IDWriteTextFormat *_bigBlackText{};
	IDWriteTextFormat *_whiteText{};
	IDWriteTextFormat *_pinkText{};

	bool _drawExitText{false};
	TilePosition _exitPosition{};

};


#endif