#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

//#include "stdafx.h"
#include "singletonBase.h"
#include "D2DImage.h"

//#include "D2DAnimateSprite.h"
//#include "D2DStillSprite.h"

#include <map>
#include <string>

class ImageManager : public singletonBase<ImageManager>
{
public :

	ImageManager();
	~ImageManager();

	HRESULT Init();
	void Release();
//	std::tstring a;

	void LoadImageFromFile(const std::wstring& filePath, LPCTSTR keyName);
	//void LoadImageFromParsedText(std::vector<LPCTSTR> *text);
	D2DImage *GetImage(const std::wstring& key);

	//void AddStillSprite(LPCTSTR keyName, D2DStillSprite *stillSprite);
	//void AddAnimateSprite(LPCTSTR keyName, D2DAnimateSprite *animateSprite);

	//D2DStillSprite *GetStillSprite(LPCTSTR key);
	//D2DAnimateSprite *GetAnimateSprite(LPCTSTR key);

	//D2DStillSprite *GetCopyOfStillSprite(LPCTSTR key);
	//D2DAnimateSprite *GetCopyOfAnimateSprite(LPCTSTR key);

private :

	typedef std::map<std::wstring, D2DImage *>::iterator IMAGE_ITER;
	//typedef std::map<LPCTSTR, D2DStillSprite *>::iterator STILL_SPRITE_ITER;
	//typedef std::map<LPCTSTR, D2DAnimateSprite *>::iterator ANIMATE_SPRITEITER;

	std::map<std::wstring, D2DImage *> _images;
	//std::map<LPCTSTR, D2DStillSprite *> _stillSprites;
	//std::map<LPCTSTR, D2DAnimateSprite *> _animateSprites;

	IMAGE_ITER _imageIter;
	//STILL_SPRITE_ITER _stillSpriteIter;
	//ANIMATE_SPRITEITER _animateSpriteIter;
};

#endif