#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "singletonBase.h"
#include "SpriteObject.h"

#include "Camera.h"

#define DUST_SPRITE_NUM 36
#define DUST_MAX_NUM 10

#define EXPLOSION_MAX_NUM 6

#define SMOKE_SPRITE_NUM 36
#define SMOKE_MAX_NUM 6

struct DustChunk
{
	SpriteObject sprite[DUST_SPRITE_NUM];
	Timer endTimer;
};

struct SmokeChunk
{
	SpriteObject sprite[SMOKE_SPRITE_NUM];
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
	void PlayExplosionEffect(const Vector2 &position);
	void PlaySmokeEffect(const Vector2 &position);

	void ClearUpdateChunks();

private :
	Vector2 GetRandomVector2(float xMax, float yMax);
	Vector2 GetRandomVector2(float xMin, float xMax, float yMin, float yMax);

	void DustSpriteEnd();
	void ExplosionEffectEndFunction();

	int _currentDustChunkTracker{ 0 };
	DustChunk _dustChunks[DUST_MAX_NUM];
	std::list<int> _dustUpdateChunks;

	int _currentExplosionChunkTracker{ 0 };
	SpriteObject _explosionSprites[EXPLOSION_MAX_NUM];
	std::list<int> _explosionUpdateChunks;
	bool _explosionShouldPop{false};

	int _currentSmokeChunkTracker{ 0 };
	SmokeChunk _smokeChunk[SMOKE_MAX_NUM];
	std::list<int> _smokeUpdateChunks;

	Camera *_pCamera{};

	bool _wasInitialized{ false };
};

#endif