#ifndef MAP_TOOL_SCENE_H
#define MAP_TOOL_SCENE_H

#include "IScene.h"
#include "ImGUI.h"

#include "TileInfo.h"

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
	void PainterAction();
private:
	D2D1::ColorF _sceneClearColor{0.1f, 0.1f, 0.1f, 1.0f};

	TileSet<TileInfo> *_editingTileSet{};

	D2DSprite *_gridSelectorSprite{};

	//Paint Action에서 추가되는 이미지들의 맵
	std::map<std::wstring, D2DSprite *>_usingImages;
	//Paint Action에서 이미지가 추가되면 그에 따라서 이 변수 에도 추가 함.
	//std::vector<D2DSprite *> _drawingSprites;
	//D2DSprite

	int _slider1Value{};
	float _slider2Value{};

	int _xSelector{};
	int _ySelector{};

	int _xPainter{};
	int _yPainter{};

	int _paintingValue{};

	WCHAR _loadNameBuffer[40]{};

//Using Character
private  :
	const WCHAR *_load{ L"Load" };
	float _loadTextWidth{ 0 };
	const WCHAR *_save{ L"Save" };
	float _saveTextWidth{ 0 };


};


#endif