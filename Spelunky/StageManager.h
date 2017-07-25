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
	void DestroyTile(const int xStartIndex, const int yStartIndex, const int width, const int height);

	Stage *GetCurrentStage();

	void SetCurrentStage(int i) { _currentStageCount = i; }
	int GetCurrentStageCount() { return _currentStageCount; }

	bool GetIsInMiddleStage() { return _inMiddleStage; }

	float GetCurrentStageElapsedTime() { return _currentStageElapsedTime; }
	float GetTotalStageElapsedTime() { return _totalStageElapsedTime; }

	int GetNormalStageMoneyCollected() { return _normalStageMoneyCollected; }

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

	float _currentStageElapsedTime{ 0 };
	float _totalStageElapsedTime{ 0 };

	int _normalStageMoneyCollected{ 0 };

};



#endif