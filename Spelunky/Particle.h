#ifndef PARTICLE_H
#define PARTICLE_H

#include "MovingObject.h"

enum ParticleType
{
	ParticleNone,
	RigidParticle,
	BlingParticle,
	SparkParticle,
};

enum ParticleSoundType
{
	PARTICLE_None,
	PARTICLE_Boulder,
	PARTICLE_Vase,
	PARTICLE_Bone,
};

class Particle 
{
public:
	Particle();
	virtual ~Particle();

	virtual HRESULT Init(const IntVector2 &sourceIndex, ParticleType particleType);
	virtual void Release(void);
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1HwndRenderTarget *renderTarget, const Vector2 &camPos);

	void PlayAt(const TilePosition &position, float scaleMult = 1.0f);

	TilePosition position;
	TilePosition desiredPosition;

protected:
	void CalculateCollision();

protected:
	bool _firstHitted{ false };
	IntVector2 _sourceIndex{};
	ParticleType _particleType{};
	D2DSprite *_sprite{};

	ReturnTile _nearTiles;

	float _firstScale{ 1.0f };
	float _scale{1.0f};
	float _degrees{ 0.0 };

	Vector2 _accel{};
	Vector2 _velocity{};
	Vector2 _maxVelocity{};

	Rect _rect;
	Rect _firstRect;
	Vector2 _rectOffset;
	Vector2 _firstOffset;

	Timer _lifeTimer;

	bool32 _valid{ false };
	float _rotationDirection{};

	ParticleSoundType _soundType;
};
#endif