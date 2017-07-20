#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

#include "singletonBase.h"
#include "Camera.h"
#include "Room.h"


struct RandomRoomGenerated
{
	IntVector2 startRoomIndex{};
	IntVector2 endRoomIndex{};
	RoomType roomTypes[16]{ ROOM_NONE, };
};

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

private:
	Stage *_currentStage;
	Camera *_pCamera{ nullptr };
};



#endif