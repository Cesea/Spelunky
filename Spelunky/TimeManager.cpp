#include "stdafx.h"
#include "TimeManager.h"

TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}

HRESULT TimeManager::Init(void)
{
	//고성능 타이머를 지원하는지에 대한 부분을 체크한다
	//_periodFrequency쪽으로 초당 파악 할 수 있는 시간의 값이 들어온다
	//초당 밀리쎄컨까지 계산이 가능하다면 1000 들어감...
	QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency);
	_isHardware = true;
	QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);

	//치수 : 어려운것 같습니다!
	_timeScale = 1.0f / _periodFrequency;

	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0;
	_worldTime = 0;

	return S_OK;
}

void TimeManager::Release(void)
{
}

void TimeManager::Tick()
{
	//현재 시간 얻어옴!
	//고성능 타이머 사용
	QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);

	//마지막 시간과 현재시간의 경과량 측정한다
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	//고정 프레임 처리해 준다
	if (_targetLock > 0.0f)
	{
		//고정 프레임의 시간을 충족할때까지 루프 돈다!!
		while (_timeElapsed < (1.0f / _targetLock))
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			//마지막 시간과 현재 시간의 경과량 측정한다
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
			Sleep(1);
		}
	}
	//초당 프레임 카운트 증가!!
	_FPSFrameCount++;

	//초당 프레임 시간 경과량 증가!!
	_FPSTimeElapsed += _timeElapsed;

	//Console::Log("DeltaTime  : %f\n", _timeElapsed);

	//전체 시간 경과량 증가!!
	_worldTime += _timeElapsed;

	//마지막 시간 기록!
	_lastTime = _curTime;

	_prevTimeElapsed = _timeElapsed;


	//프레임 초기화 한다 (1초마다!)
	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		//#if defined _DEBUG
		Console::Log("FPS : %d\n", (int)_frameRate);
		//#endif
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}

}

bool TimeManager::HasToAdvance()
{
	bool result = false;
	QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);

	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	//고정 프레임의 시간을 충족할때까지 루프 돈다!!
	if (_timeElapsed >= (1.0f / _targetLock))
	{
		_lastTime = _curTime;
		_FPSTimeElapsed += _timeElapsed;
		_worldTime += _timeElapsed;
		_prevTimeElapsed = _timeElapsed;
		result = true;
	}

	//프레임 초기화 한다 (1초마다!)
	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		//#if defined _DEBUG
		Console::Log("FPS : %d\n", (int)_frameRate);
		//#endif
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;

	}
	return result;
}

unsigned long TimeManager::GetFrameRate(char * str) const
{
	if (str != NULL)
	{
		sprintf(str, "FPS : %d", _frameRate);
	}
	return _frameRate;
}
