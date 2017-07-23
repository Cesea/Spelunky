#ifndef ANIMATION_H
#define ANIMATION_H

#include "stdafx.h"
#include "Delegate.h"

class GameObject;

class Animation
{
public:
	typedef std::vector<IntVector2> vFrameList; //������ ����Ʈ
	typedef std::vector<int> vPlayList;	//��������� ����Ʈ

public:
	Animation(void);
	~Animation(void);

	//�ʱ�ȭ
	HRESULT Init(int totalW, int totalH, int frameW, int frameH);
	HRESULT InitCopy(const Animation *animaiton);
	void SetOwner(GameObject *owner) { _pOwner = owner; }
	void Release(void);

	//ó������ ������ �ִϸ��̼�... (����Ʈ ��)
	void SetDefPlayFrame(BOOL reverse = FALSE, BOOL loop = FALSE);

	//���ϴ� �ε��� �ִϸ��̼�...
	void SetPlayFrame(int* arr, int arrLen, BOOL loop = FALSE);

	//���� �ִϸ��̼�...
	void SetPlayFrame(int start, int end, BOOL reverse = FALSE, BOOL loop = FALSE);

	//�ʴ� ���� Ƚ����...
	void SetFPS(int framePerSec);

	//������ ������Ʈ...
	void FrameUpdate(float elapsedTime);

	void Start(void);		//�ִ� ����
	void Stop(void);		//�ִ� ����
	void Pause(void);		//�ִ� �Ͻ� ���� 
	void Resume(void);		//�ִ� �ٽ� ����

							//�÷��̳�?
	inline BOOL IsPlay(void) { return _isPlay; }

	//������ ��ġ ���´�
	inline IntVector2 GetFramePos(void) { return _frameList[_playList[_nowPlayIdx]]; }

	//������ ���� ũ�� ���´�
	inline int GetFrameWidth(void) { return _framePerWidth; }

	//������ ���� ũ�� ���´�
	inline int GetFrameHeight(void) { return _framePerHeight; }

	bool GetCopyed() { return _copyed; }

	void SetEndFunction(const Delegate<void> &function);

private:
	vFrameList _frameList;
	vPlayList _playList;

	int _framePerWidth{};		//������ ����ũ��
	int _framePerHeight{};		//������ ����ũ��

	int _frameNum{};			//������ ����

	bool _loop{};				//�ݺ�?
	float _frameUpdateSec{};	//������ ������Ʈ �ð�
	float _elapsedSec{};		//������ �ð��� ���� �ֱ�

	uint32 _nowPlayIdx{};		//���� �ִϸ��̼� �ε���
	bool _isPlay{};			//�����?


	bool _hasEndFunction{ false };
	Delegate<void> _endFunction{};
	//bool _hasUpdateFunction{ false };
	//Delegate<void> _frameUpdateFunction{};

	bool _copyed{ false };

	GameObject *_pOwner{nullptr};
};


#endif