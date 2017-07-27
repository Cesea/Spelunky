#include "stdafx.h"
#include "SoundManager.h"

HRESULT SoundManager::Init(void)
{
	//사운드 시스템 생성
	System_Create(&_system);

	//채널수 설정
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//사운드 채널 생성
	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];

	//초기화
	ZeroMemory(_sound, sizeof(Sound*) * TOTALSOUNDBUFFER);
	ZeroMemory(_channel, sizeof(Channel*) * TOTALSOUNDBUFFER);

	return S_OK;
}

void SoundManager::Release(void)
{
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}
			if (_sound != NULL)
			{
				if (_sound[i]) _sound[i]->release();
			}
		}
	}

	//메모리 지우고..
	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	//시스템 닫는다
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}

	_mTotalSounds.clear();
}

void SoundManager::AddSound(const std::wstring & keyName, const std::wstring & soundName, bool background, bool loop)
{
	char buffer[50];
	wcstombs(buffer, soundName.c_str(), 49);
	//반복이냐?
	if (loop)
	{
		//배경음임?
		if (background)
		{
			_system->createStream(buffer,
				FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(buffer,
				FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
	}
	else
	{
		//한번만 사운드 재생
		_system->createSound(buffer,
			FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
	}

	//맵에 사운드를 키값과 함께 넣어준다
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

void SoundManager::Play(const std::wstring & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이~~~
			_system->playSound(FMOD_CHANNEL_FREE, 
				*iter->second, false, &_channel[count]);
		}
	}
}

void SoundManager::Play(const std::wstring & keyName, float volume)
{
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이~~~
			_system->playSound(FMOD_CHANNEL_FREE, 
				*iter->second, false, &_channel[count]);
			_channel[count]->setVolume(volume);
		}
	}
}

void SoundManager::Stop(const std::wstring & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void SoundManager::Pause(const std::wstring & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::Resume(const std::wstring & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

bool SoundManager::IsPause(const std::wstring & keyName)
{
	bool pause;
	ArrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&pause);
			break;
		}
	}

	return pause;
}

bool SoundManager::IsPlay(const std::wstring & keyName)
{
	bool pause;
	ArrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&pause);
			break;
		}
	}

	return pause;
}

unsigned int SoundManager::GetLength(const std::wstring & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();
	
	for (iter; iter != _mTotalSounds.end(); ++iter)
	{
		if (keyName == iter->first)
		{
			unsigned int length = 0;
			(*iter->second)->getLength(&length, FMOD_TIMEUNIT_MS);

			return length;
		}
	}

	return NULL;
}

void SoundManager::SetPosition(const std::wstring &keyName, unsigned int ms)
{
	ArrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPosition(ms, FMOD_TIMEUNIT_MS);
			break;
		}
	}
}

unsigned int SoundManager::GetPosition(const std::wstring & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			unsigned int length = 0;
			_channel[count]->getPosition(&length, FMOD_TIMEUNIT_MS);
			return length;
		}
	}

	return NULL;

}

void SoundManager::Update(void)
{
	_system->update();
}

SoundManager::SoundManager(void)
{
}

SoundManager::~SoundManager(void)
{
}
