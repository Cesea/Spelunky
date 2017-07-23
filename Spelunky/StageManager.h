#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

#include "singletonBase.h"
#include "Camera.h"
#include "Room.h"

#include "Player.h" 

class StageManager : public singletonBase<StageManager>
{
public:
	StageManager();
	~StageManager();

	HRESULT Init();
	void Release();

	void SetCameraLink(Camera *camera) { _pCamera = camera; }
	void SetPlayerLink(Player *player) { _pPlayer = player; }

	void Update(float deltaTime);
	void Render();

	void DestroyTile(const IntVector2 &tilePos);

	Stage *GetCurrentStage();

	void SetCurrentStage(int i) { _currentStageCount = i; }

private:
	RandomRoomGenerated MakeRandomRoomTypes();

	void BuildMiddleStage();
	void BuildNextStage();

	void HandleStageTransitionEvent(const IEvent *event);

private:
	Stage *_currentStage{};

	Stage *_middleStage{};
	Camera *_pCamera{ nullptr };
	Player *_pPlayer{ nullptr };

	float _timeElapsedInStage{0};
	int _currentStageCount{ 0 };

	bool _inMiddleStage{ true };

};



#endif