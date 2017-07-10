#ifndef GAME_H
#define GAME_H


#include "gameNode.h"


class Game : public gameNode
{
public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget);

	Game();
	virtual ~Game();


private :
};

#endif
