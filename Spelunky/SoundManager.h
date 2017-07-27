#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "singletonbase.h"

#include <unordered_map>

//fmod.hpp �߰����ش�
#include "inc/fmod.hpp"

//lib ��ũ
#pragma comment (lib, "lib/fmodex_vc.lib")

//ä�� ���� (����~!!!)
#define EXTRACHANNLEBUFFER 50
#define SOUNDBUFFER 100

//�� ���� ����
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

	//���� ���̰� ������.
	unsigned int GetLength(const std::wstring & keyName);

	//���� ��� ��ġ ����
	void SetPosition(const std::wstring & keyName, unsigned int ms);
	unsigned int GetPosition(const std::wstring & keyName);

	//fmod �ý��� �����Ѵ�
	void Update(void);

	SoundManager(void);
	virtual ~SoundManager(void);
};

#endif
