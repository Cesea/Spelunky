#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "singletonBase.h"
#include "SpriteObject.h"

#include "Camera.h"

#define DUST_SPRITE_NUM 30


struct DustChunk
{
	SpriteObject sprite[25];
};

class EffectManager : public singletonBase<EffectManager>
{
public :
	EffectManager();
	~EffectManager();

	HRESULT Init();
	void Release();

	void Update(float deltaTime);
	void Render();

	void SetCameraLink(Camera *camera) { _pCamera = camera; }


	void PlayDustParticles(const Vector2 &position);

private :
	Vector2 GetRandomVector2(float xMax, float yMax);
	Vector2 GetRandomVector2(float xMin, float xMax, float yMin, float yMax);


	int _currentDustChunkTracker{ 0 };
	DustChunk _dustChunks[10];
	std::list<int> _dustUpdateChunks;

	Camera *_pCamera{};
};

#endif