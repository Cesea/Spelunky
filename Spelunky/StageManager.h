#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

#include "singletonBase.h"
#include "Camera.h"
#include "Room.h"


class StageManager : public singletonBase<StageManager>
{
public:
	StageManager();
	~StageManager();

	HRESULT Init();
	void Release();

	void SetCameraLink(Camera *camera) { _pCamera = camera; }

	void Update(float deltaTime);
	void Render();

	Stage *GetCurrentStage();
private:
	RandomRoomGenerated MakeRandomRoomTypes();

	void BuildMiddleStage();
	void BuildNextStage();

	void HandleStageTransitionEvent(const IEvent *event);

private:
	Stage *_currentStage;

	Stage *_middleStage{};
	Camera *_pCamera{ nullptr };

	float _timeElapsedInStage{0};
	int _currentStageCount{ 0 };

	bool _inMiddleStage{ false };

};



#endif