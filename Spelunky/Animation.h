#ifndef ANIMATION_H
#define ANIMATION_H

#include "stdafx.h"
#include "Delegate.h"

class GameObject;

class Animation
{
public:
	typedef std::vector<IntVector2> vFrameList; //프레임 리스트
	typedef std::vector<int> vPlayList;	//프레임재생 리스트

public:
	Animation(void);
	~Animation(void);

	//초기화
	HRESULT Init(int totalW, int totalH, int frameW, int frameH);
	HRESULT InitCopy(const Animation *animaiton);
	void SetOwner(GameObject *owner) { _pOwner = owner; }
	void Release(void);

	//처음부터 끝까지 애니메이션... (디폴트 값)
	void SetDefPlayFrame(BOOL reverse = FALSE, BOOL loop = FALSE);

	//원하는 인덱스 애니메이션...
	void SetPlayFrame(int* arr, int arrLen, BOOL loop = FALSE);

	//구간 애니메이션...
	void SetPlayFrame(int start, int end, BOOL reverse = FALSE, BOOL loop = FALSE);

	//초당 갱신 횟수임...
	void SetFPS(int framePerSec);

	//프레임 업데이트...
	void FrameUpdate(float elapsedTime);

	void Start(void);		//애니 시작
	void Stop(void);		//애니 정지
	void Pause(void);		//애니 일시 정지 
	void Resume(void);		//애니 다시 시작

							//플레이냐?
	inline BOOL IsPlay(void) { return _isPlay; }

	//프레임 위치 얻어온다
	inline IntVector2 GetFramePos(void) { return _frameList[_playList[_nowPlayIdx]]; }

	//프레임 가로 크기 얻어온다
	inline int GetFrameWidth(void) { return _framePerWidth; }

	//프레임 세로 크기 얻어온다
	inline int GetFrameHeight(void) { return _framePerHeight; }

	bool GetCopyed() { return _copyed; }

	void SetEndFunction(const Delegate<void> &function);

private:
	vFrameList _frameList;
	vPlayList _playList;

	int _framePerWidth{};		//프레임 가로크기
	int _framePerHeight{};		//프레임 세로크기

	int _frameNum{};			//프레임 갯수

	bool _loop{};				//반복?
	float _frameUpdateSec{};	//프레임 업데이트 시간
	float _elapsedSec{};		//프레임 시간에 대한 주기

	uint32 _nowPlayIdx{};		//현재 애니메이션 인덱스
	bool _isPlay{};			//재생중?


	bool _hasEndFunction{ false };
	Delegate<void> _endFunction{};
	//bool _hasUpdateFunction{ false };
	//Delegate<void> _frameUpdateFunction{};

	bool _copyed{ false };

	GameObject *_pOwner{nullptr};
};


#endif