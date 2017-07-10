#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include "singletonBase.h"

class TimeManager : public singletonBase<TimeManager>
{
public:
	TimeManager();
	virtual ~TimeManager();

public:
	//�ʱ�ȭ
	HRESULT Init(void);

	//������
	void Release(void);

	//���� �ð��� ����Ѵ�
	void Tick();
	bool HasToAdvance();

	//���� FPS
	unsigned long GetFrameRate(char* str = NULL) const;

	//������ �ð��� ���� �ð��� ����� ��ȯ
	inline float GetElapsedTime(void) const { return _prevTimeElapsed; }

	//��ü ����ð�!!
	inline float GetWorldTime(void) const { return _worldTime; }

	inline float GetTargetLock() const { return _targetLock; }

	//Have to implement this
	void Render() {}

private:
	bool _isHardware;			//���� Ÿ�̸� �����ǳ�?
	float _timeScale;			//�ð��� ����� ����
	float _timeElapsed;			//������ �ð��� ���� �ð��� �����
	float _prevTimeElapsed;
	__int64 _curTime;			//����ð�
	__int64 _lastTime;			//�����ð�
	__int64 _periodFrequency;	//�ð��ֱ�

	float			_targetLock{ 60.0f };

	unsigned long	_frameRate;		//FPS
	unsigned long	_FPSFrameCount;	//FPS ī��Ʈ
	double			_FPSTimeElapsed;
	double			_worldTime;		//���� �����Ŀ� �귯�� ��ü�ð�
};


#endif