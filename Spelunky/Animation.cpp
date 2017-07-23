#include "stdafx.h"
#include "Animation.h"

#include "GameObject.h"

Animation::Animation(void)
{
}

Animation::~Animation(void)
{
}

HRESULT Animation::Init(int totalW, int totalH, int frameW, int frameH)
{
	_framePerWidth = frameW;
	_framePerHeight = frameH;

	int frameNumWidth = totalW / frameW;
	int frameNumHeight = totalH / frameH;

	_frameNum = frameNumWidth * frameNumHeight;

	_frameList.clear();
	for (int y = 0; y < frameNumHeight; ++y)
	{
		for (int x = 0; x < frameNumWidth; ++x)
		{
			IntVector2 framePos(x * frameW, y * frameH);
			_frameList.push_back(framePos);
		}
	}
	return S_OK;
}

HRESULT Animation::InitCopy(const Animation *animation)
{
	_copyed = true;

	_framePerWidth = animation->_framePerWidth;
	_framePerHeight = animation->_framePerHeight;

	_frameNum = animation->_frameNum;

	_frameList.clear();
	_frameList.assign(animation->_frameList.begin(), animation->_frameList.end());

	_loop = animation->_loop;
	_frameUpdateSec = animation->_frameUpdateSec;	//프레임 업데이트 시간
	_elapsedSec = animation->_elapsedSec; //프레임 시간에 대한 주기

	_nowPlayIdx = animation->_nowPlayIdx;
	_isPlay = animation->_isPlay;

	_playList.clear();
	_playList.assign(animation->_playList.begin(), animation->_playList.end());

	return S_OK;
}

void Animation::Release(void)
{
}

void Animation::SetDefPlayFrame(BOOL reverse, BOOL loop)
{
	_loop = loop;
	_playList.clear();

	if (reverse)
	{
		if (loop)
		{
			//갈때
			for (int i = 0; i < _frameNum; ++i)
			{
				_playList.push_back(i);
			}
			//올때
			for (int i = _frameNum - 2; i >= 0; --i)
			{
				_playList.push_back(i);
			}

		}
		else
		{
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}

			//올때 프레임
			for (int i = _frameNum - 2; i >= 0; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		for (int i = 0; i < _frameNum; ++i)
		{
			_playList.push_back(i);
		}
	}
}

void Animation::SetPlayFrame(int * arr, int arrLen, BOOL loop)
{
	//뺑뻉이냐?
	_loop = loop;

	//플레이 리스트 끌리어..
	_playList.clear();

	for (int i = 0; i < arrLen; i++)
	{
		_playList.push_back(arr[i]);
	}
}

void Animation::SetPlayFrame(int start, int end, BOOL reverse, BOOL loop)
{
	//뺑뻉이냐?
	_loop = loop;

	//플레이 리스트 끌리어..
	_playList.clear();

	//시작과 끝이 같으면.......
	if (start == end)
	{
		_playList.clear();
		Stop();
		return;
	}

	//시작프레임이 종료프레임보다 큰 경우
	if (start > end)
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i < start; i++)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i <= start; i++)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i >= end; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	//스타트 < 엔드
	else
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i <= end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end - 1; i > start; i--)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i <= end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end - 1; i >= start; i--)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i <= end; i++)
			{
				_playList.push_back(i);
			}
		}
	}
}



void Animation::SetFPS(int framePerSec)
{
	_frameUpdateSec = 1.0f / framePerSec;
}

void Animation::FrameUpdate(float elapsedTime)
{
	if (_isPlay)
	{
		_elapsedSec += elapsedTime;

		//프레임 업데이트 시간이 되었으면....
		if (_elapsedSec >= _frameUpdateSec)
		{
			_elapsedSec -= _frameUpdateSec;
			_nowPlayIdx++;

			if (_nowPlayIdx == _playList.size())
			{
				//뺑뺑이냐?
				if (_loop)
				{
					_nowPlayIdx = 0;
				}
				else
				{
					_nowPlayIdx--;
					_isPlay = false;

					if (_hasEndFunction)
					{
						_endFunction();
					}

					if (_pOwner)
					{
						EVENTMANAGER->QueueEvent(new FrameEndedEvent(_pOwner->GetId()));
					}
				}
			}
		}
	}
}

void Animation::Start(void)
{
	_isPlay = true;
	_nowPlayIdx = 0;
}

void Animation::Stop(void)
{
	_isPlay = false;
	_nowPlayIdx = 0;
}

void Animation::Pause(void)
{
	_isPlay = false;
}

void Animation::Resume(void)
{
	_isPlay = true;
}

void Animation::SetEndFunction(const Delegate<void>& function)
{
	_hasEndFunction = true;
	_endFunction = function;
}
