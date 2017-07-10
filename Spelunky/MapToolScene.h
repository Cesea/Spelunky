#ifndef MAP_TOOL_SCENE_H
#define MAP_TOOL_SCENE_H

#include "IScene.h"


class MapToolScene : public IScene
{
public:
	MapToolScene();
	virtual ~MapToolScene();

	virtual HRESULT Init(void) override;
	virtual void Release(void) override;
	virtual void Update(void) override;
	virtual void Render(ID2D1HwndRenderTarget *renderTarget) override;

	virtual HRESULT LoadContent() override;

	//private functions
private:


};


#endif