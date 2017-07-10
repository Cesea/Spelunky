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
	//���� Ÿ�̸Ӹ� �����ϴ����� ���� �κ��� üũ�Ѵ�
	//_periodFrequency������ �ʴ� �ľ� �� �� �ִ� �ð��� ���� ���´�
	//�ʴ� �и��������� ����� �����ϴٸ� 1000 ��...
	QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency);
	_isHardware = true;
	QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);

	//ġ�� : ������ �����ϴ�!
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
	//���� �ð� ����!
	//���� Ÿ�̸� ���
	QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);

	//������ �ð��� ����ð��� ����� �����Ѵ�
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	//���� ������ ó���� �ش�
	if (_targetLock > 0.0f)
	{
		//���� �������� �ð��� �����Ҷ����� ���� ����!!
		while (_timeElapsed < (1.0f / _targetLock))
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			//������ �ð��� ���� �ð��� ����� �����Ѵ�
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
			Sleep(1);
		}
	}
	//�ʴ� ������ ī��Ʈ ����!!
	_FPSFrameCount++;

	//�ʴ� ������ �ð� ����� ����!!
	_FPSTimeElapsed += _timeElapsed;

	//Console::Log("DeltaTime  : %f\n", _timeElapsed);

	//��ü �ð� ����� ����!!
	_worldTime += _timeElapsed;

	//������ �ð� ���!
	_lastTime = _curTime;

	_prevTimeElapsed = _timeElapsed;


	//������ �ʱ�ȭ �Ѵ� (1�ʸ���!)
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

	//���� �������� �ð��� �����Ҷ����� ���� ����!!
	if (_timeElapsed >= (1.0f / _targetLock))
	{
		_lastTime = _curTime;
		_FPSTimeElapsed += _timeElapsed;
		_worldTime += _timeElapsed;
		_prevTimeElapsed = _timeElapsed;
		result = true;
	}

	//������ �ʱ�ȭ �Ѵ� (1�ʸ���!)
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
