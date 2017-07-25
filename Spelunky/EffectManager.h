#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "singletonBase.h"
#include "SpriteObject.h"

#include "Camera.h"

#include "Particle.h"

#define DUST_SPRITE_NUM 36
#define DUST_MAX_NUM 10

#define EXPLOSION_MAX_NUM 6

#define SMOKE_SPRITE_NUM 36
#define SMOKE_MAX_NUM 6

#define TILE_PARTICLE_SPRITE_NUM 8
#define TILE_PARTICLE_MAX_NUM 16

#define ROCK_PARTICLE_SPRITE_NUM 8
#define ROCK_PARTICLE_MAX_NUM 10

#define JAR_PARTICLE_SPRITE_NUM 8
#define JAR_PARTICLE_MAX_NUM 12

#define BONE_PARTICLE_SPRITE_NUM 8
#define BONE_PARTICLE_MAX_NUM 12

#define BACKBONE_PARTICLE_SPRITE_NUM 8
#define BACKBONE_PARTICLE_MAX_NUM 12

#define THORN_PARTICLE_SPRITE_NUM 8
#define THORN_PARTICLE_MAX_NUM 12

#define WOOD_PARTICLE_SPRITE_NUM 8
#define WOOD_PARTICLE_MAX_NUM 12

#define BLOOD_PARTICLE_SPRITE_NUM 6
#define BLOOD_PARTICLE_MAX_NUM 12

#define BLING_PARTICLE_SPRITE_NUM 6
#define BLING_PARTICLE_MAX_NUM 10

struct DustChunk
{
	SpriteObject sprite[DUST_SPRITE_NUM];
};

struct SmokeChunk
{
	SpriteObject sprite[SMOKE_SPRITE_NUM];
};


struct TileParticle
{
	Particle particles[TILE_PARTICLE_SPRITE_NUM];
};
struct TileParticleCollection
{
	void Init();
	void Update(float deltaTime);
	void Render(const Vector2 &camPos);
	void PlayParticle(const Vector2 &position, float scale = 1.0f);
	int currentParticleChunkTracker{0};
	TileParticle particles[TILE_PARTICLE_MAX_NUM];
	std::list<int> particleUpdateChunks;
	Timer particleStopTimer;
	bool timerShouldGoOn{ false };
	int timerCount{};
};
struct RockParticle
{
	Particle particles[ROCK_PARTICLE_SPRITE_NUM];
};
struct RockParticleCollection
{
	void Init();
	void Update(float deltaTime);
	void Render(const Vector2 &camPos);
	void PlayParticle(const Vector2 &position, float scale = 1.0f);
	int currentParticleChunkTracker{0};
	TileParticle particles[ROCK_PARTICLE_MAX_NUM];
	std::list<int> particleUpdateChunks;
	Timer particleStopTimer;
	bool timerShouldGoOn{ false };
	int timerCount{};
};
struct JarParticle
{
	Particle particles[JAR_PARTICLE_SPRITE_NUM];
};

struct JarParticleCollection
{
	void Init();
	void Update(float deltaTime);
	void Render(const Vector2 &camPos);
	void PlayParticle(const Vector2 &position, float scale = 1.0f);
	int currentParticleChunkTracker{0};
	TileParticle particles[JAR_PARTICLE_MAX_NUM];
	std::list<int> particleUpdateChunks;
	Timer particleStopTimer;
	bool timerShouldGoOn{ false };
	int timerCount{};
};
struct BoneParticle
{
	Particle particles[BONE_PARTICLE_SPRITE_NUM];
};
struct BoneParticleCollection
{
	void Init();
	void Update(float deltaTime);
	void Render(const Vector2 &camPos);
	void PlayParticle(const Vector2 &position, float scale = 1.0f);
	int currentParticleChunkTracker{0};
	TileParticle particles[JAR_PARTICLE_MAX_NUM];
	std::list<int> particleUpdateChunks;
	Timer particleStopTimer;
	bool timerShouldGoOn{ false };
	int timerCount{};
};
struct BackBoneParticle
{
	Particle particles[BACKBONE_PARTICLE_SPRITE_NUM];
};
struct BackBoneParticleCollection
{
	void Init();
	void Update(float deltaTime);
	void Render(const Vector2 &camPos);
	void PlayParticle(const Vector2 &position, float scale = 1.0f);
	int currentParticleChunkTracker{0};
	TileParticle particles[BACKBONE_PARTICLE_MAX_NUM];
	std::list<int> particleUpdateChunks;
	Timer particleStopTimer;
	bool timerShouldGoOn{ false };
	int timerCount{};
};
struct ThornParticle
{
	Particle particles[THORN_PARTICLE_SPRITE_NUM];
};
struct ThornParticleCollection
{
	void Init();
	void Update(float deltaTime);
	void Render(const Vector2 &camPos);
	void PlayParticle(const Vector2 &position, float scale = 1.0f);
	int currentParticleChunkTracker{0};
	TileParticle particles[THORN_PARTICLE_MAX_NUM];
	std::list<int> particleUpdateChunks;
	Timer particleStopTimer;
	bool timerShouldGoOn{ false };
	int timerCount{};
};
struct WoodParticle
{
	Particle particles[WOOD_PARTICLE_SPRITE_NUM];
};
struct WoodParticleCollection
{
	void Init();
	void Update(float deltaTime);
	void Render(const Vector2 &camPos);
	void PlayParticle(const Vector2 &position, float scale = 1.0f);
	int currentParticleChunkTracker{0};
	TileParticle particles[WOOD_PARTICLE_MAX_NUM];
	std::list<int> particleUpdateChunks;
	Timer particleStopTimer;
	bool timerShouldGoOn{ false };
	int timerCount{};
};

struct LadderParticle
{
	Particle particles[WOOD_PARTICLE_SPRITE_NUM];
};

struct LadderParticleCollection
{
	void Init();
	void Update(float deltaTime);
	void Render(const Vector2 &camPos);
	void PlayParticle(const Vector2 &position, float scale = 1.0f);
	int currentParticleChunkTracker{0};
	TileParticle particles[WOOD_PARTICLE_MAX_NUM];
	std::list<int> particleUpdateChunks;
	Timer particleStopTimer;
	bool timerShouldGoOn{ false };
	int timerCount{};
};

struct BloodParticle
{
	Particle particles[BLOOD_PARTICLE_SPRITE_NUM];
};
struct BloodParticleCollection
{
	void Init();
	void Update(float deltaTime);
	void Render(const Vector2 &camPos);
	void PlayParticle(const Vector2 &position, float scale = 1.0f);
	int currentParticleChunkTracker{ 0 };
	TileParticle particles[BLOOD_PARTICLE_MAX_NUM];
	std::list<int> particleUpdateChunks;
	Timer particleStopTimer;
	bool timerShouldGoOn{ false };
	int timerCount{};
};

struct BingParticle
{
	Particle particle[BLING_PARTICLE_SPRITE_NUM];
};

struct BlingParticleCollection
{
	void Init();
	void Update(float deltaTime);
	void Render(const Vector2 &camPos);
	void PlayParticle(const Vector2 &position, float scale = 1.0f);
	int currentParticleChunkTracker{ 0 };
	TileParticle particles[BLING_PARTICLE_MAX_NUM];
	std::list<int> particleUpdateChunks;
	Timer particleStopTimer;
	bool timerShouldGoOn{ false };
	int timerCount{};
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

	void PlayTileParticle(const Vector2 &position, float scale = 1.0f);
	void PlayRockParticle(const Vector2 &position, float scale = 1.0f);
	void PlayJarParticle(const Vector2 &position, float scale = 1.0f);
	void PlayBoneParticle(const Vector2 &position, float scale = 1.0f);
	void PlayBackBoneParticle(const Vector2 &position, float scale = 1.0f);
	void PlayThornParticle(const Vector2 &position, float scale = 1.0f);
	void PlayWoodParticle(const Vector2 &position, float scale = 1.0f);
	void PlayLadderParticle(const Vector2 &position, float scale = 1.0f);
	void PlayBloodParticle(const Vector2 &position, float sclae = 1.0f);
	void PlayBlingParticle(const Vector2 &posiiton, float scale = 1.0f);

private :
	Vector2 GetRandomVector2(float xMax, float yMax);
	Vector2 GetRandomVector2(float xMin, float xMax, float yMin, float yMax);

	void DustSpriteEnd();
	void ExplosionEffectEndFunction();

	int _currentDustChunkTracker{ 0 };
	DustChunk _dustChunks[DUST_MAX_NUM];
	std::list<int> _dustUpdateChunks;
	Timer _dustStopTimer;
	bool _dustTimerShouldGoOn{ false };
	int _dustTimerCount{ 0 };

	int _currentExplosionChunkTracker{ 0 };
	SpriteObject _explosionSprites[EXPLOSION_MAX_NUM];
	std::list<int> _explosionUpdateChunks;
	bool _explosionShouldPop{false};

	int _currentSmokeChunkTracker{ 0 };
	SmokeChunk _smokeChunk[SMOKE_MAX_NUM];
	std::list<int> _smokeUpdateChunks;

	TileParticleCollection _tileParticles;
	RockParticleCollection _rockParticles;
	JarParticleCollection _jarParticles;
	BoneParticleCollection _boneParticles;
	BackBoneParticleCollection _backBoneParticles;
	ThornParticleCollection _thornParticles;
	WoodParticleCollection _woodParticles;
	LadderParticleCollection _ladderParticles;
	BloodParticleCollection _bloodParticles;
	BlingParticleCollection _blingParticles;


	Camera *_pCamera{};

	bool _wasInitialized{ false };
};

#endif