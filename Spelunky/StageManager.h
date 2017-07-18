#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

#include "singletonBase.h"
#include "Camera.h"
#include "Room.h"

namespace PlayScene
{

	class StageManager : public singletonBase<StageManager>
	{
	public :
		StageManager();
		~StageManager();

		HRESULT Init();
		void Release();

		void SetCameraLink(Camera *camera) { _pCamera = camera; }

		void Update(float deltaTime);
		void RenderBorderLayer();
		void RenderTileLayer();
		void RenderMaskLayer();


		Stage *GetCurrentStage();
	private :
		Stage *_currentStage;

		Camera *_pCamera{nullptr};
	};
}



#endif