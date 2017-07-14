#include "stdafx.h"
#include "ImageManager.h"

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
	
}

HRESULT ImageManager::Init()
{
	return S_OK;
}

void ImageManager::Release()
{
	//for (auto &sprite : _stillSprites)
	//{
	//	sprite.second->Release();
	//}
	//_stillSprites.clear();

	//for (auto &sprite : _animateSprites)
	//{
	//	sprite.second->Release();
	//}
	//_animateSprites.clear();

	for (auto &im : _images)
	{
		im.second->Release();
	}
	_images.clear();
}

void ImageManager::LoadImageFromFile(const std::wstring &filePath, const std::wstring &keyName)
{
	auto &found = _images.find(keyName);

	if (found != _images.end())
	{
//		이미지가 있다.
		Console::Log("Image %s already exist\n", keyName.c_str());
		return;
	}
	else
	{
		D2DImage *insertImage = new D2DImage;

		_d2d.LoadBitmapFromFile(filePath.c_str(), insertImage->GetBitmap());
		insertImage->Init();
		insertImage->SetName(keyName);

		_images.insert(std::make_pair(keyName, insertImage));
	}
}

//void ImageManager::LoadImageFromParsedText(std::vector<LPCTSTR>* text)
//{
//	Assert(text->size() == 2);
//
//	LPCTSTR filePath = (*text)[0];
//	LPCTSTR keyName = (*text)[1];
//
//	_imageIter = _images.end();
//	for (auto it = _images.begin(); it != _images.end(); )
//	{
//		if (_tcscmp(it->first, keyName) == 0)
//		{
//			_imageIter = it;
//			break;
//		}
//		else
//		{
//			++it;
//		}
//	}
//
//	if (_imageIter != _images.end())
//	{
//		//		Assert(0);
//		//		이미지가 있다.
//		TCHAR buffer[32];
//		wsprintf(buffer, _T("Image already exist\n"));
//		OutputDebugString(buffer);
//		return;
//	}
//	else
//	{
//		D2DImage *insertImage = _d2d.LoadBitmapFromFile(filePath);
//		//		insertImage->Init(filePath, width, height);
//		int numKeyName = _tcslen(keyName);
//		TCHAR *key = (TCHAR *)calloc(numKeyName, sizeof(TCHAR));
//		_tcscpy(key, keyName);
//		key[numKeyName] = 0;
//
//		_images.insert(std::make_pair(key, insertImage));
//
////		Assert(inserted.second);
//	}
//}

D2DImage * ImageManager::GetImage(const std::wstring &key)
{
	_imageIter = _images.end();
	for (auto it = _images.begin(); it != _images.end(); )
	{
		if (_tcscmp(it->first.c_str(), key.c_str()) == 0)
		{
			_imageIter = it;
			break;
		}
		else
		{
			++it;
		}
	}
	if (_imageIter != _images.end())
	{
		return _imageIter->second;
	}
	else
	{
		Assert(1);
		return nullptr;
	}
}

//void ImageManager::AddStillSprite(LPCTSTR keyName, D2DStillSprite * stillSprite)
//{
//	_stillSpriteIter = _stillSprites.end();
//	for (auto it = _stillSprites.begin(); it != _stillSprites.end(); )
//	{
//		if (_tcscmp(it->first, keyName) == 0)
//		{
//			_stillSpriteIter = it;
//			break;
//		}
//		else
//		{
//			++it;
//		}
//	}
//
////	auto &find = _stillSprites.find(keyName);
//	if (_stillSpriteIter != _stillSprites.end())
//	{
//		//		Assert(0);
//		//		이미지가 있다.
//		TCHAR buffer[32];
//		wsprintf(buffer, _T("Image already exist\n"));
//		OutputDebugString(buffer);
//		return;
//	}
//	else
//	{
//		int numKeyName = _tcslen(keyName);
//		TCHAR *key = (TCHAR *)calloc(numKeyName, sizeof(TCHAR));
//		_tcscpy(key, keyName);
//		key[numKeyName] = 0;
//
//		_stillSprites.insert(std::make_pair(key, stillSprite));
//
////		Assert(inserted.second);
//	}
//}
//
//void ImageManager::AddAnimateSprite(LPCTSTR keyName, D2DAnimateSprite * animateSprite)
//{
//	_animateSpriteIter = _animateSprites.end();
//	for (auto it = _animateSprites.begin(); it != _animateSprites.end(); )
//	{
//		if (_tcscmp(it->first, keyName) == 0)
//		{
//			_animateSpriteIter = it;
//			break;
//		}
//		else
//		{
//			++it;
//		}
//	}
//
//	if (_animateSpriteIter != _animateSprites.end())
//	{
//		//		Assert(0);
//		//		이미지가 있다.
//		TCHAR buffer[32];
//		wsprintf(buffer, _T("Image already exist\n"));
//		OutputDebugString(buffer);
//		return;
//	}
//	else
//	{
//		int numKeyName = _tcslen(keyName);
//		TCHAR *key = (TCHAR *)calloc(numKeyName, sizeof(TCHAR));
//		_tcscpy(key, keyName);
//		key[numKeyName] = 0;
//
//		_animateSprites.insert(std::make_pair(key, animateSprite));
//
////		Assert(inserted.second);
//	}
//}
//
//D2DStillSprite * ImageManager::GetStillSprite(LPCTSTR key)
//{
//	_stillSpriteIter = _stillSprites.end();
//	for (auto it = _stillSprites.begin(); it != _stillSprites.end(); )
//	{
//		if (_tcscmp(it->first, key) == 0)
//		{
//			_stillSpriteIter = it;
//			break;
//		}
//		else
//		{
//			++it;
//		}
//	}
////	auto &find = _stillSprites.find(key);
//	if (_stillSpriteIter != _stillSprites.end())
//	{
//		return _stillSpriteIter->second;
//	}
//	else
//	{
//		Assert(0);
//		return nullptr;
//	}
//}
//
//D2DAnimateSprite * ImageManager::GetAnimateSprite(LPCTSTR key)
//{
//	_animateSpriteIter = _animateSprites.end();
//	for (auto it = _animateSprites.begin(); it != _animateSprites.end(); )
//	{
//		if (_tcscmp(it->first, key) == 0)
//		{
//			_animateSpriteIter = it;
//			break;
//		}
//		else
//		{
//			++it;
//		}
//	}
////	auto &find = _animateSprites.find(key);
//	if (_animateSpriteIter != _animateSprites.end())
//	{
//		return _animateSpriteIter->second;
//	}
//	else
//	{
//		Assert(1);
//		return nullptr;
//	}
//}
//
//D2DStillSprite * ImageManager::GetCopyOfStillSprite(LPCTSTR key)
//{
//	_stillSpriteIter = _stillSprites.end();
//	for (auto it = _stillSprites.begin(); it != _stillSprites.end(); )
//	{
//		if (_tcscmp(it->first, key) == 0)
//		{
//			_stillSpriteIter = it;
//			break;
//		}
//		else
//		{
//			++it;
//		}
//	}
//	//	auto &find = _stillSprites.find(key);
//	if (_stillSpriteIter != _stillSprites.end())
//	{
//		D2DStillSprite *sourceSprite = _stillSpriteIter->second;
//
//		D2DStillSprite *copySprite = new D2DStillSprite();
//		HRESULT result = copySprite->InitCopy(sourceSprite);
//		Assert(SUCCEEDED(result));
//		return copySprite;
//	}
//	else
//	{
//		Assert(1);
//		return nullptr;
//	}return nullptr;
//}
//
//D2DAnimateSprite * ImageManager::GetCopyOfAnimateSprite(LPCTSTR key)
//{
//	_animateSpriteIter = _animateSprites.end();
//	for (auto it = _animateSprites.begin(); it != _animateSprites.end(); )
//	{
//		if (_tcscmp(it->first, key) == 0)
//		{
//			_animateSpriteIter = it;
//			break;
//		}
//		else
//		{
//			++it;
//		}
//	}
//	//	auto &find = _animateSprites.find(key);
//	if (_animateSpriteIter != _animateSprites.end())
//	{
//		D2DAnimateSprite *sourceSprite = _animateSpriteIter->second;
//
//		D2DAnimateSprite *copySprite = new D2DAnimateSprite();
//		HRESULT result = copySprite->InitCopy(sourceSprite);
//		copySprite->SetFrameBound(0, 16, 0, 0);
//		Assert(SUCCEEDED(result));
//		return copySprite;
////		return _animateSpriteIter->second;
//	}
//	else
//	{
//		Assert(1);
//		return nullptr;
//	}
//}
