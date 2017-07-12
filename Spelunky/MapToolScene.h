#ifndef MAP_TOOL_SCENE_H
#define MAP_TOOL_SCENE_H

#include "IScene.h"
#include "ImGUI.h"

#include "TileInfo.h"

enum class DrawingMode
{
	Draw,
	Erase
};

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
	void DoGUIS();

	void LoadButtonAction();
	void PainterAction();
	void SaveMapButtonAction();
	void LoadMapButtonAction();

	void CheckUsingImageExistence(const std::wstring &key);

	void CalculateBitMask(TileSet<TileInfo> &tileSet);
	void RenderMasks(int drawXIndex, int drawYIndex, const uint16 maskInfo, D2DSprite *image);
private:

	D2D1::ColorF _sceneClearColor{0.1f, 0.1f, 0.1f, 1.0f};

	TileSet<TileInfo> *_editingTileSet{};

	D2DSprite *_gridSelectorSprite{};

	//Paint Action에서 추가되는 이미지들의 맵
	std::map<std::wstring, D2DSprite *>_usingImages;

	IntRect _selectorRect{};
	
	//int _xSelector{};
	//int _ySelector{};

	int _xPainter{};
	int _yPainter{};

	WCHAR _loadImageNameBuffer[40]{};
	WCHAR _mapLoadSaveNameBuffer[40]{};
	WCHAR _propertyEditBuffer[40]{};

	DrawingMode _drawingMode{};

//Using Character
private  :
	const WCHAR *_propertyEditorText{L"Property Editor"};

	const WCHAR *_loadImageText{ L"LoadIM" };
	float _loadImageTextWidth{ 0 };
	const WCHAR *_save{ L"Save" };
	float _saveTextWidth{ 0 };

	const WCHAR *_saveMapText{ L"SaveMap" };
	float _saveMapTextWidth{0};
	const WCHAR *_loadMapText{ L"LoadMap" };
	float _loadMapTextWidth{0};

	const WCHAR *_drawText{ L"Draw" };
	float _drawTextWidth{0};
	const WCHAR *_eraseText{ L"Erase" };
	float _eraseTextWidth{0};

	const WCHAR *_maskText{ L"Do Mask" };
	float _maskTextWidth{0};



};


#endif