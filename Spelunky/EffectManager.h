#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "singletonBase.h"
#include "SpriteObject.h"

#include "Camera.h"

#define DUST_SPRITE_NUM 36
#define DUST_MAX_NUM 10


struct DustChunk
{
	SpriteObject sprite[DUST_SPRITE_NUM];
	Timer endTimer;
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

	void ClearUpdateChunks();

private :
	Vector2 GetRandomVector2(float xMax, float yMax);
	Vector2 GetRandomVector2(float xMin, float xMax, float yMin, float yMax);

	void DustSpriteEnd();

	int _currentDustChunkTracker{ 0 };
	DustChunk _dustChunks[DUST_MAX_NUM];
	std::list<int> _dustUpdateChunks;
	

	Camera *_pCamera{};
};

#endif