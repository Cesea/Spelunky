#include "stdafx.h"
#include "KeyAnimationManager.h"

#include "Animation.h"
#include "D2DImage.h"

KeyAnimationManager::KeyAnimationManager(void)
{
}

KeyAnimationManager::~KeyAnimationManager(void)
{
}

HRESULT KeyAnimationManager::Init(void)
{
	return S_OK;
}

void KeyAnimationManager::Release(void)
{
	DeleteAll();
}

void KeyAnimationManager::Update(void)
{
	AnimationMapIter iter = _totalAnimations.begin();

	for (iter; iter != _totalAnimations.end(); ++iter)
	{
		if (!iter->second->IsPlay()) continue;
		iter->second->FrameUpdate(TIMEMANAGER->GetElapsedTime());
	}
}

void KeyAnimationManager::Render(void)
{
}

void KeyAnimationManager::AddDefPlayFrameAnimation(const std::wstring &aniKeyName, const std::wstring &imgKeyName, int frameWidth, int frameHeight,
	int fps, bool reverse, bool loop)
{
	D2DImage *image = IMAGEMANAGER->GetImage(imgKeyName);
	Animation* ani = new Animation;
	ani->Init(image->GetWidth(), image->GetHeight(), frameWidth, frameHeight);
	ani->SetDefPlayFrame(reverse, loop);
	ani->SetFPS(fps);

	_totalAnimations.insert(make_pair(aniKeyName, ani));
}

void KeyAnimationManager::AddArrayFrameAnimation(const std::wstring &aniKeyName, const std::wstring &imgKeyName, int frameWidth, int frameHeight,
	int* arr, int arrLen, int fps, bool loop)
{
	D2DImage* image = IMAGEMANAGER->GetImage(imgKeyName);

	Animation* ani = new Animation;
	ani->Init(image->GetWidth(), image->GetHeight(), frameWidth, frameHeight);
	ani->SetPlayFrame(arr, arrLen, loop);
	ani->SetFPS(fps);

	_totalAnimations.insert(make_pair(aniKeyName, ani));
}

void KeyAnimationManager::AddCoordinateFrameAnimation(const std::wstring &aniKeyName, const std::wstring &imgKeyName, int frameWidth, int frameHeight,
	int start, int end, int fps, bool reverse, bool loop)
{
	D2DImage* image = IMAGEMANAGER->GetImage(imgKeyName);

	Animation* ani = new Animation;
	ani->Init(image->GetWidth(), image->GetHeight(), frameWidth, frameHeight);
	ani->SetPlayFrame(start, end, reverse, loop);
	ani->SetFPS(fps);

	_totalAnimations.insert(make_pair(aniKeyName, ani));
}

void KeyAnimationManager::Start(const std::wstring & aniKeyName)
{
	_totalAnimations.find(aniKeyName)->second->Start();
}

void KeyAnimationManager::Stop(const std::wstring & aniKeyName)
{
	_totalAnimations.find(aniKeyName)->second->Stop();
}

void KeyAnimationManager::Pause(const std::wstring & aniKeyName)
{
	_totalAnimations.find(aniKeyName)->second->Pause();
}

void KeyAnimationManager::Resume(const std::wstring & aniKeyName)
{
	_totalAnimations.find(aniKeyName)->second->Resume();
}

Animation * KeyAnimationManager::FindAnimation(const std::wstring & aniKeyName)
{
	AnimationMapIter iter = _totalAnimations.find(aniKeyName);
	if (iter != _totalAnimations.end())
	{
		return iter->second;
	}
	return NULL;
}

void KeyAnimationManager::DeleteAll(void)
{
	AnimationMapIter iter = _totalAnimations.begin();

	for (; iter != _totalAnimations.end();)
	{
		if (iter->second != NULL)
		{
			iter->second->Release();
			SAFE_DELETE(iter->second);
			iter = _totalAnimations.erase(iter);
		}
		else ++iter;
	}

	_totalAnimations.clear();
}
