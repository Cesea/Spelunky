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
	void LoadButtonAction();
	void PainterAction();
	void SaveMapButtonAction();
	void LoadMapButtonAction();

	void LoadEditingTileImageInfo();

	void CheckUsingImageExistence(const std::wstring &key);

	void SaveCurrentEditingImageInfoAction();

	void CalculateBitMask(TileInfo *sourceLayer, TileInfo *maskLayer);
	void RenderMasks(int drawXIndex, int drawYIndex, const uint32 maskInfo, D2DSprite *image);
	void ClearAllTheBits(RoomInfo *roomInfo);

	void TileInfoBitmaskCopy(const std::wstring imageKey, TileInfo &sourTile, TileInfo &maskTile, uint32 offset);

	int InSyncImageInfo();
	int OutSyncImageInfo();

	void WriteTileInfoChunkForMap(FileUtils::File &file, const TileInfo *infos, int xCount, int yCount);
	void ReadTileInfoChunkForMap(FileUtils::File &file, TileInfo *infos, int xCount, int yCount);
private:
	D2D1::ColorF _sceneClearColor{0.1f, 0.1f, 0.1f, 1.0f};
	//현재 수정하고 있는 이미지의 정보
	TileImageInfo _editingTileImageInfo{};
	//현재 수정하고 있는 방의 정보
	//TileSet<TileInfo> *_editingTileSet{};
	RoomInfo _roomInfo{};
	//그리드 셀렉터 이미지
	D2DSprite *_gridSelectorSprite{};

	//Paint Action에서 추가되는 이미지들의 맵
	std::map<std::wstring, D2DSprite *>_usingImages;

	IntRect _selectorRect{};
	
	int _xPainter{};
	int _yPainter{};

	WCHAR _loadImageNameBuffer[40]{};
	WCHAR _mapLoadSaveNameBuffer[40]{};
	WCHAR _propertyEditBuffer[40]{};

	WCHAR _boomDestroyBuffer[16]{};
	WCHAR _destroyedIndexBuffer[16]{};
	WCHAR _thisMaskBuffer[16]{};
	WCHAR _nearMaskBuffer[16]{};
	WCHAR _collisionTypeBuffer[16]{};
	WCHAR _layerBuffer[16]{};

	bool _applyTimerFlag{false};
	WCHAR _applySuccessBuffer[16]{};

	DrawingMode _drawingMode{};

//Using Character
private  :
	const WCHAR *_propertyEditorText{L"Property Editor"};

	const WCHAR *_saveImageProperty{ L"Save Image Property" };
	float _saveImagePropertyWidth{ 0 };

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

	Timer _timer;

	Timer _applySuccessFlagTimer;


};


#endif