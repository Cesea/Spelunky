
//#include "stdafx.h"
//#include "SpriteBuilder.h"
//
//#include "D2DAnimationSprite.h"
//
//SpriteBuilder::SpriteBuilder()
//{
//}
//
//SpriteBuilder::~SpriteBuilder()
//{
//}
//
//D2DSprite * SpriteBuilder::BuildStillSprite(std::vector<std::wstring>* text)
//{
//	return nullptr;
//}
//
//D2DSprite * SpriteBuilder::BuildAnimationSprite(const std::wstring & sourceKey, const std::wstring & spriteTextPath)
//{
//	std::vector<std::vector<std::wstring>> text = TEXTDATAMANAGER->TextLoad(spriteTextPath.c_str());
//
//	D2DSprite *result = {};
//	result = new D2DAnimationSprite;
//
//	uint32 lineCounter = 0;
//
//	uint32 numAnimationFrame = (uint32)_wtof((text)[lineCounter++][2].c_str());
//
//	float time = (float)_wtof((text)[lineCounter++][2].c_str());
//	AnimationType type = AnimationType::Loop;
//
//	int typeInt = (int)_wtoi((text)[lineCounter++][2].c_str());
//	if (typeInt == 1)
//	{
//		type = AnimationType::Loop;
//	}
//	else
//	{
//		type = AnimationType::NoLoop;
//	}
//
//	//IntVector2 anchorPoint = {};
//	std::vector<IntVector2> anchorPoints;
//	anchorPoints.reserve(numAnimationFrame);
//	for (int i = 0; i < numAnimationFrame; ++i)
//	{
//		IntVector2 anchorPoint = {};
//		anchorPoint.x = (int)_wtoi((text)[lineCounter++][2].c_str());
//		anchorPoint.y = (int)_wtoi((text)[lineCounter++][2].c_str());
//		anchorPoints.push_back(anchorPoint);
//	}
//
//	std::vector<Rect> rects;
//	rects.reserve(numAnimationFrame);
//
//	for (uint32 numFrame = 0; numFrame < numAnimationFrame; ++numFrame)
//	{
//		Rect rect;
//		rect.x = _wtof((text)[lineCounter][2].c_str());
//		rect.y = _wtof((text)[lineCounter][3].c_str());
//		rect.width = _wtof((text)[lineCounter][4].c_str());
//		rect.height = _wtof((text)[lineCounter++][5].c_str());
//		rects.push_back(rect);
//	}
//	result->Init(IMAGEMANAGER->GetImage(sourceKey), time, numAnimationFrame - 1, rects, type, anchorPoints);
//	return result;
//
//}
//