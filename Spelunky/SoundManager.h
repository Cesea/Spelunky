#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "singletonbase.h"

#include <unordered_map>

//fmod.hpp 추가해준다
#include "inc/fmod.hpp"

//lib 링크
#pragma comment (lib, "lib/fmodex_vc.lib")

//채널 갯수 (버퍼~!!!)
#define EXTRACHANNLEBUFFER 50
#define SOUNDBUFFER 100

//총 사운드 갯수
#define TOTALSOUNDBUFFER (EXTRACHANNLEBUFFER + SOUNDBUFFER)

using namespace FMOD;

class SoundManager : public singletonBase <SoundManager>
{
private:
	typedef std::unordered_map<std::wstring, Sound**> ArrSounds;
	typedef std::unordered_map<std::wstring, Sound**>::iterator ArrSoundsIter;
	typedef std::unordered_map<std::wstring, Channel**> ArrChannels;
	typedef std::unordered_map<std::wstring, Channel**>::iterator ArrChannelsIter;

private:
	System* _system;
	Sound** _sound;
	Channel** _channel;

	ArrSounds _mTotalSounds;

public:
	HRESULT Init(void);
	void Release(void);

	void AddSound(const std::wstring &keyName, const std::wstring & soundName, bool background, bool loop);
	void Play(const std::wstring & keyName);
	void Play(const std::wstring & keyName, float volume);
	void Stop(const std::wstring & keyName);
	void Pause(const std::wstring & keyName);
	void Resume(const std::wstring & keyName);

	bool IsPause(const std::wstring & keyName);
	bool IsPlay(const std::wstring & keyName);

	//음악 길이값 가져옴.
	unsigned int GetLength(const std::wstring & keyName);

	//음악 재생 위치 설정
	void SetPosition(const std::wstring & keyName, unsigned int ms);
	unsigned int GetPosition(const std::wstring & keyName);

	//fmod 시스템 갱신한다
	void Update(void);

	SoundManager(void);
	virtual ~SoundManager(void);
};

#endif
