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
private :
	void LoadButtonAction();
private:
	D2D1::ColorF _sceneClearColor{0.1f, 0.1f, 0.1f, 1.0f};

	TileSet<IntVector2> *_editingTileSet{};

	D2DSprite *_labelTestSprite{};
	D2DSprite *_gridSelectorSprite{};

	int _slider1Value{};
	float _slider2Value{};
	

	int _xSelecting{};
	int _ySelecting{};

	WCHAR _loadNameBuffer[40]{};

//Using Character
private  :
	const WCHAR *_load{ L"Load" };
	float _loadTextWidth{ 0 };
	const WCHAR *_save{ L"Save" };
	float _saveTextWidth{ 0 };


};


#endif