#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include "singletonBase.h"

class TimeManager : public singletonBase<TimeManager>
{
public:
	TimeManager();
	virtual ~TimeManager();

public:
	//초기화
	HRESULT Init(void);

	//릴리즈
	void Release(void);

	//현재 시간을 계산한다
	void Tick();
	bool HasToAdvance();

	//현재 FPS
	unsigned long GetFrameRate(char* str = NULL) const;

	//마지막 시간과 현재 시간의 경과량 반환
	inline float GetElapsedTime(void) const { return _prevTimeElapsed; }

	//전체 경과시간!!
	inline float GetWorldTime(void) const { return _worldTime; }

	inline float GetTargetLock() const { return _targetLock; }

	//Have to implement this
	void Render() {}

private:
	bool _isHardware;			//고성능 타이머 지원되냐?
	float _timeScale;			//시간의 경과량 조절
	float _timeElapsed;			//마지막 시간과 현재 시간의 경과량
	float _prevTimeElapsed;
	__int64 _curTime;			//현재시간
	__int64 _lastTime;			//이전시간
	__int64 _periodFrequency;	//시간주기

	float			_targetLock{ 60.0f };

	unsigned long	_frameRate;		//FPS
	unsigned long	_FPSFrameCount;	//FPS 카운트
	double			_FPSTimeElapsed;
	double			_worldTime;		//게임 시작후에 흘러간 전체시간
};


#endif