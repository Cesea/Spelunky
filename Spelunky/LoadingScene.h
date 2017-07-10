#ifndef LOADING_SCENE_H
#define LOADING_SCENE_H


#include "IScene.h"

class LoadingScene : public IScene
{
public:
	LoadingScene();
	virtual ~LoadingScene();

	virtual HRESULT Init(void) override;
	virtual void Release(void) override;
	virtual void Update(void) override;
	virtual void Render(void) override;

	virtual HRESULT LoadContent() override;

	//private functions
private:


};


#endif