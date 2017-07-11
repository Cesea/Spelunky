#ifndef MAP_TOOL_SCENE_H
#define MAP_TOOL_SCENE_H

#include "IScene.h"


#include "ImGUI.h"

class MapToolScene : public IScene
{
public:
	MapToolScene();
	virtual ~MapToolScene();

	virtual HRESULT Init(void) override;
	virtual void Release(void) override;
	virtual void Update(void) override;
	virtual void Render(void) override;

	virtual HRESULT LoadContent() override;

	//private functions
private:
	D2D1::ColorF _sceneClearColor{0.1f, 0.1f, 0.1f, 1.0f};

	TileSet *_editingTileSet{};

	int _slider1Value{};
	float _slider2Value{};
	
	WCHAR _buffer[40]{};

};


#endif