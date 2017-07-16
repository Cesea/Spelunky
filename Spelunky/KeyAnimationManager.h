#ifndef FRAME_ANIMATION_MANAGER_H
#define FRAME_ANIMATION_MANAGER_H

#include "singletonBase.h"
#include <unordered_map>

class D2DImage;
class Animation;

class KeyAnimationManager : public singletonBase <KeyAnimationManager>
{
private:
	typedef std::unordered_map<std::wstring, Animation*> AnimationMap;
	typedef std::unordered_map<std::wstring, Animation*>::iterator AnimationMapIter;

private:
	AnimationMap _totalAnimations;
public:

	KeyAnimationManager(void);
	virtual ~KeyAnimationManager(void);

	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

	void AddDefPlayFrameAnimation(const std::wstring &aniKeyName, const std::wstring &imgKeyName, int frameWidth, int frameHeight,
		int fps, bool reverse, bool loop);

	void AddArrayFrameAnimation(const std::wstring &aniKeyName, const std::wstring &imgKeyName, int frameWidth, int frameHeight,
		int* arr, int arrLen, int fps, bool loop);

	void AddCoordinateFrameAnimation(const std::wstring &aniKeyName, const std::wstring &imgKeyName, int frameWidth, int frameHeight,
		int start, int end, int fps, bool reverse, bool loop);

	void Start(const std::wstring &aniKeyName);
	void Stop(const std::wstring &aniKeyName);
	void Pause(const std::wstring &aniKeyName);
	void Resume(const std::wstring &aniKeyName);

	Animation* FindAnimation(const std::wstring &aniKeyName);

	void DeleteAll(void);

};


#endif