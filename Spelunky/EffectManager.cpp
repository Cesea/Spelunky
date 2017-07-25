#include "stdafx.h"
#include "EffectManager.h"

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
}

HRESULT EffectManager::Init()
{
	if (!_wasInitialized)
	{
		_tileParticles.Init();
		_rockParticles.Init();
		_jarParticles.Init();
		_boneParticles.Init();
		_backBoneParticles.Init();
		_thornParticles.Init();
		_woodParticles.Init();
		_ladderParticles.Init();
		_bloodParticles.Init();
		_blingParticles.Init();

		_dustStopTimer.Init(2.0f);
		_wasInitialized = true;
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < DUST_SPRITE_NUM; ++j)
			{
				if (j == 0)
				{
					_dustChunks[i].sprite[j].
						SetDisappearEndFunction(Delegate<void>::FromFunction<EffectManager, &EffectManager::DustSpriteEnd>(this));
				}
				_dustChunks[i].sprite[j].Init(L"dustring", 0, 0, 512, 512, IntVector2(-256, -256));
			}
		}

		for (int i = 0; i < SMOKE_MAX_NUM; ++i)
		{
			for (int j = 0; j < SMOKE_SPRITE_NUM; ++j)
			{
				if (j == 0)
				{
					//_dustChunks[i].sprite[j].
					//	SetDisappearEndFunction(Delegate<void>::FromFunction<EffectManager, &EffectManager::DustSpriteEnd>(this));
				}
				_smokeChunk[i].sprite[j].Init(L"smokering", 0, 0, 512, 512, IntVector2(-256, -256));
			}
		}

		for (int i = 0; i < 6; ++i)
		{
			Animation *animation = new Animation;
			animation->InitCopy(KEYANIMANAGER->FindAnimation(L"explosion"));
			animation->SetEndFunction(Delegate<void>::FromFunction<EffectManager, &EffectManager::ExplosionEffectEndFunction>(this));
			_explosionSprites[i].InitAnimation(L"explosion", animation, IntVector2(-64, -128));
			_explosionSprites[i].scale = Vector2(1.8, 1.8);
		}
	}
	return S_OK;
}

void EffectManager::Release()
{
}

void EffectManager::Update(float deltaTime)
{
	for (auto &index : _dustUpdateChunks)
	{
		for (int j = 0; j < DUST_SPRITE_NUM; ++j)
		{
			_dustChunks[index].sprite[j].Update(deltaTime);
		}
	}

	for (auto &index : _explosionUpdateChunks)
	{
		_explosionSprites[index].Update(deltaTime);
	}

	if (_explosionShouldPop)
	{
		_explosionShouldPop = false;
		_explosionUpdateChunks.pop_front();
	}

	for (auto &index : _smokeUpdateChunks)
	{
		for (int j = 0; j < SMOKE_SPRITE_NUM; ++j)
		{
			_smokeChunk[index].sprite[j].Update(deltaTime);
		}
	}

	if (_dustTimerShouldGoOn && _dustTimerCount)
	{
		if (_dustStopTimer.Tick(deltaTime))
		{
			_dustTimerCount--;
			if (_dustTimerCount <= 0)
			{
				_dustTimerShouldGoOn = false;
			}
			_dustUpdateChunks.pop_front();
		}
	}

	_tileParticles.Update(deltaTime);
	_rockParticles.Update(deltaTime);
	_jarParticles.Update(deltaTime);
	_boneParticles.Update(deltaTime);
	_backBoneParticles.Update(deltaTime);
	_thornParticles.Update(deltaTime);
	_woodParticles.Update(deltaTime);
	_ladderParticles.Update(deltaTime);
	_bloodParticles.Update(deltaTime);
	_blingParticles.Update(deltaTime);
}

void EffectManager::Render()
{
	Vector2 camPosUntTiled = _pCamera->GetPosition().UnTilelize();
	for (auto &index : _dustUpdateChunks)
	{
		for (int i = 0; i < DUST_SPRITE_NUM; ++i)
		{
			_dustChunks[index].sprite[i].RenderScale(gRenderTarget, camPosUntTiled);
		}
	}

	for (auto &index : _explosionUpdateChunks)
	{
		_explosionSprites[index].RenderScale(gRenderTarget, camPosUntTiled);
	}

	for (auto &index : _smokeUpdateChunks)
	{
		for (int i = 0; i < SMOKE_SPRITE_NUM; ++i)
		{
			_smokeChunk[index].sprite[i].RenderScale(gRenderTarget, camPosUntTiled);
		}
	}

	_tileParticles.Render(camPosUntTiled);
	_rockParticles.Render(camPosUntTiled);
	_jarParticles.Render(camPosUntTiled);
	_boneParticles.Render(camPosUntTiled);
	_backBoneParticles.Render(camPosUntTiled);
	_thornParticles.Render(camPosUntTiled);
	_woodParticles.Render(camPosUntTiled);
	_ladderParticles.Render(camPosUntTiled);
	_bloodParticles.Render(camPosUntTiled);
	_blingParticles.Render(camPosUntTiled);
}

void EffectManager::PlayDustParticles(const Vector2 & position)
{
	for (int i = 0; i < DUST_SPRITE_NUM; ++i)
	{
		_dustChunks[_currentDustChunkTracker].sprite[i].position = position;
		_dustChunks[_currentDustChunkTracker].sprite[i].position += GetRandomVector2(-50, 50, -20, 1);

		_dustChunks[_currentDustChunkTracker].sprite[i].alpha = RND->GetFloat(0.4, 0.8);

		float randomStartScale = RND->GetFloat(0.01, 0.02);
		_dustChunks[_currentDustChunkTracker].sprite[i].scale = Vector2(randomStartScale, randomStartScale);

		float randomScale = RND->GetFloat(0.2, 0.34);

		float randomTime = RND->GetFloat(2.0f, 5.0f);

		_dustChunks[_currentDustChunkTracker].sprite[i].ScaleTo(randomScale, randomScale, randomTime);
		_dustChunks[_currentDustChunkTracker].sprite[i].MoveBy(GetRandomVector2(-60, 60, -60, -150), randomTime);
		_dustChunks[_currentDustChunkTracker].sprite[i].DisappearTo(0, randomTime);

	}
	if (_dustUpdateChunks.size() == 0)
	{
		_dustUpdateChunks.push_back(_currentDustChunkTracker);
	}
	else
	{
		for (auto &iter = _dustUpdateChunks.begin(); iter != _dustUpdateChunks.end();)
		{
			if (*iter != _currentDustChunkTracker)
			{
				_dustUpdateChunks.push_back(_currentDustChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}

	_currentDustChunkTracker++;
	if (_currentDustChunkTracker >= DUST_MAX_NUM)
	{
		_currentDustChunkTracker = 0;
	}
}

void EffectManager::PlayExplosionEffect(const Vector2 & position)
{
	_explosionSprites[_currentExplosionChunkTracker].position = position;
	_explosionSprites[_currentExplosionChunkTracker].position.y += 30 ;
	_explosionSprites[_currentExplosionChunkTracker].GetSprite()->GetAnimation()->Start();

	if (_explosionUpdateChunks.size() == 0)
	{
		_explosionUpdateChunks.push_back(_currentExplosionChunkTracker);
	}
	else
	{
		for (auto &iter = _explosionUpdateChunks.begin(); iter != _explosionUpdateChunks.end();)
		{
			if (*iter != _currentExplosionChunkTracker)
			{
				_explosionUpdateChunks.push_back(_currentExplosionChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}

	_currentExplosionChunkTracker++;
	if (_currentExplosionChunkTracker >= EXPLOSION_MAX_NUM)
	{
		_currentExplosionChunkTracker = 0;
	}
}

void EffectManager::PlaySmokeEffect(const Vector2 & position)
{
	for (int i = 0; i < SMOKE_SPRITE_NUM; ++i)
	{
		_smokeChunk[_currentSmokeChunkTracker].sprite[i].position = position;
		_smokeChunk[_currentSmokeChunkTracker].sprite[i].position += GetRandomVector2(-8, 8, -3, 3);

		_smokeChunk[_currentSmokeChunkTracker].sprite[i].alpha = RND->GetFloat(0.4, 0.8);

		float randomStartScale = RND->GetFloat(0.01, 0.02);
		_smokeChunk[_currentSmokeChunkTracker].sprite[i].scale = Vector2(randomStartScale, randomStartScale);

		float randomScale = RND->GetFloat(0.14, 0.30);

		float randomTime = RND->GetFloat(0.4f, 0.9f);

		_smokeChunk[_currentSmokeChunkTracker].sprite[i].ScaleTo(randomScale, randomScale, randomTime);
		_smokeChunk[_currentSmokeChunkTracker].sprite[i].MoveBy(GetRandomVector2(-60, 60, -60, -150), randomTime);
		_smokeChunk[_currentSmokeChunkTracker].sprite[i].DisappearTo(0, randomTime);

	}
	if (_smokeUpdateChunks.size() == 0)
	{
		_smokeUpdateChunks.push_back(_currentSmokeChunkTracker);
	}
	else
	{
		for (auto &iter = _smokeUpdateChunks.begin(); iter != _smokeUpdateChunks.end();)
		{
			if (*iter != _currentSmokeChunkTracker)
			{
				_smokeUpdateChunks.push_back(_currentSmokeChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	_currentSmokeChunkTracker++;
	if (_currentSmokeChunkTracker >= DUST_MAX_NUM)
	{
		_currentSmokeChunkTracker = 0;
	}
}

void EffectManager::ClearUpdateChunks()
{
	_dustUpdateChunks.clear();
}

void EffectManager::PlayTileParticle(const Vector2 & position, float scale)
{
	_tileParticles.PlayParticle(position, scale);
}

void EffectManager::PlayRockParticle(const Vector2 & position, float scale)
{
	_rockParticles.PlayParticle(position, scale);
}

void EffectManager::PlayJarParticle(const Vector2 & position, float scale)
{
	_jarParticles.PlayParticle(position, scale);
}

void EffectManager::PlayBoneParticle(const Vector2 & position, float scale)
{
	_boneParticles.PlayParticle(position, scale);
}

void EffectManager::PlayBackBoneParticle(const Vector2 & position, float scale)
{
	_backBoneParticles.PlayParticle(position, scale);
}

void EffectManager::PlayThornParticle(const Vector2 & position, float scale)
{
	_thornParticles.PlayParticle(position, scale);
}

void EffectManager::PlayWoodParticle(const Vector2 & position, float scale)
{
	_woodParticles.PlayParticle(position, scale);
}

void EffectManager::PlayLadderParticle(const Vector2 & position, float scale)
{
	_ladderParticles.PlayParticle(position, scale);
}

void EffectManager::PlayBloodParticle(const Vector2 & position, float scale)
{
	_bloodParticles.PlayParticle(position, scale);
}

void EffectManager::PlayBlingParticle(const Vector2 & position, float scale)
{
	_blingParticles.PlayParticle(position, scale);
}

Vector2 EffectManager::GetRandomVector2(float xMax, float yMax)
{
	return Vector2(RND->GetFloat(-xMax, xMax), RND->GetFloat(-yMax, yMax));
}

Vector2 EffectManager::GetRandomVector2(float xMin, float xMax, float yMin, float yMax)
{
	return Vector2(RND->GetFloat(xMin, xMax), RND->GetFloat(yMin, yMax));
}

void EffectManager::DustSpriteEnd()
{
	_dustTimerShouldGoOn = true;
	_dustTimerCount++;
}

void EffectManager::ExplosionEffectEndFunction()
{
	_explosionShouldPop = true;
}

void TileParticleCollection::Init()
{
	for (int i = 0; i < TILE_PARTICLE_MAX_NUM; ++i)
	{
		for (int j = 0; j < TILE_PARTICLE_SPRITE_NUM; ++j)
		{
			particles[i].particles[j].Init(IntVector2(0, 0), ParticleType::RigidParticle);
		}
	}
	particleStopTimer.Init(2.0);
}

void TileParticleCollection::Update(float deltaTime)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < TILE_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Update(deltaTime);
		}
	}

	if (timerShouldGoOn && timerCount)
	{
		if (particleStopTimer.Tick(deltaTime))
		{
			timerCount--;
			if (timerCount <= 0)
			{
				timerShouldGoOn = false;
			}
			particleUpdateChunks.pop_front();
		}
	}
}

void TileParticleCollection::Render(const Vector2 & camPos)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < TILE_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Render(gRenderTarget, camPos);
		}
	}
}

void TileParticleCollection::PlayParticle(const Vector2 & position, float scale)
{
	for (int i = 0; i < TILE_PARTICLE_SPRITE_NUM; ++i)
	{
		particles[currentParticleChunkTracker].particles[i].PlayAt(TilePosition(position));
	}

	if (particleUpdateChunks.size() == 0)
	{
		particleUpdateChunks.push_back(currentParticleChunkTracker);
	}
	else
	{
		for (auto &iter = particleUpdateChunks.begin(); iter != particleUpdateChunks.end();)
		{
			if (*iter != currentParticleChunkTracker)
			{
				particleUpdateChunks.push_back(currentParticleChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	currentParticleChunkTracker++;
	if (currentParticleChunkTracker >= TILE_PARTICLE_MAX_NUM)
	{
		currentParticleChunkTracker = 0;
	}
	timerShouldGoOn = true;
	timerCount++;
}

void RockParticleCollection::Init()
{
	for (int i = 0; i < ROCK_PARTICLE_MAX_NUM; ++i)
	{
		for (int j = 0; j < ROCK_PARTICLE_SPRITE_NUM; ++j)
		{
			particles[i].particles[j].Init(IntVector2(1, 0), ParticleType::RigidParticle);
		}
	}
	particleStopTimer.Init(2.0);
}

void RockParticleCollection::Update(float deltaTime)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < ROCK_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Update(deltaTime);
		}
	}

	if (timerShouldGoOn && timerCount)
	{
		if (particleStopTimer.Tick(deltaTime))
		{
			timerCount--;
			if (timerCount <= 0)
			{
				timerShouldGoOn = false;
			}
			particleUpdateChunks.pop_front();
		}
	}
}

void RockParticleCollection::Render(const Vector2 & camPos)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < ROCK_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Render(gRenderTarget, camPos);
		}
	}
}

void RockParticleCollection::PlayParticle(const Vector2 & position, float scale)
{
	for (int i = 0; i < ROCK_PARTICLE_SPRITE_NUM; ++i)
	{
		particles[currentParticleChunkTracker].particles[i].PlayAt(TilePosition(position));
	}

	if (particleUpdateChunks.size() == 0)
	{
		particleUpdateChunks.push_back(currentParticleChunkTracker);
	}
	else
	{
		for (auto &iter = particleUpdateChunks.begin(); iter != particleUpdateChunks.end();)
		{
			if (*iter != currentParticleChunkTracker)
			{
				particleUpdateChunks.push_back(currentParticleChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	currentParticleChunkTracker++;
	if (currentParticleChunkTracker >= ROCK_PARTICLE_MAX_NUM)
	{
		currentParticleChunkTracker = 0;
	}
	timerShouldGoOn = true;
	timerCount++;
}

void JarParticleCollection::Init()
{
	for (int i = 0; i < JAR_PARTICLE_MAX_NUM; ++i)
	{
		for (int j = 0; j < JAR_PARTICLE_SPRITE_NUM; ++j)
		{
			particles[i].particles[j].Init(IntVector2(2, 0), ParticleType::RigidParticle);
		}
	}
	particleStopTimer.Init(2.0);
}

void JarParticleCollection::Update(float deltaTime)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < ROCK_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Update(deltaTime);
		}
	}
	if (timerShouldGoOn && timerCount)
	{
		if (particleStopTimer.Tick(deltaTime))
		{
			timerCount--;
			if (timerCount <= 0)
			{
				timerShouldGoOn = false;
			}
			particleUpdateChunks.pop_front();
		}
	}
}

void JarParticleCollection::Render(const Vector2 & camPos)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < JAR_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Render(gRenderTarget, camPos);
		}
	}
}

void JarParticleCollection::PlayParticle(const Vector2 & position, float scale)
{
	for (int i = 0; i < JAR_PARTICLE_SPRITE_NUM; ++i)
	{
		particles[currentParticleChunkTracker].particles[i].PlayAt(TilePosition(position));
	}

	if (particleUpdateChunks.size() == 0)
	{
		particleUpdateChunks.push_back(currentParticleChunkTracker);
	}
	else
	{
		for (auto &iter = particleUpdateChunks.begin(); iter != particleUpdateChunks.end();)
		{
			if (*iter != currentParticleChunkTracker)
			{
				particleUpdateChunks.push_back(currentParticleChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	currentParticleChunkTracker++;
	if (currentParticleChunkTracker >= JAR_PARTICLE_MAX_NUM)
	{
		currentParticleChunkTracker = 0;
	}
	timerShouldGoOn = true;
	timerCount++;
}

void BoneParticleCollection::Init()
{
	for (int i = 0; i < BONE_PARTICLE_MAX_NUM; ++i)
	{
		for (int j = 0; j < BONE_PARTICLE_SPRITE_NUM; ++j)
		{
			particles[i].particles[j].Init(IntVector2(4, 0), ParticleType::RigidParticle);
		}
	}
	particleStopTimer.Init(2.0);
}

void BoneParticleCollection::Update(float deltaTime)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < BONE_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Update(deltaTime);
		}
	}
	if (timerShouldGoOn && timerCount)
	{
		if (particleStopTimer.Tick(deltaTime))
		{
			timerCount--;
			if (timerCount <= 0)
			{
				timerShouldGoOn = false;
			}
			particleUpdateChunks.pop_front();
		}
	}
}

void BoneParticleCollection::Render(const Vector2 & camPos)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < BONE_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Render(gRenderTarget, camPos);
		}
	}
}

void BoneParticleCollection::PlayParticle(const Vector2 & position, float scale)
{
	for (int i = 0; i < BONE_PARTICLE_SPRITE_NUM; ++i)
	{
		particles[currentParticleChunkTracker].particles[i].PlayAt(TilePosition(position));
	}

	if (particleUpdateChunks.size() == 0)
	{
		particleUpdateChunks.push_back(currentParticleChunkTracker);
	}
	else
	{
		for (auto &iter = particleUpdateChunks.begin(); iter != particleUpdateChunks.end();)
		{
			if (*iter != currentParticleChunkTracker)
			{
				particleUpdateChunks.push_back(currentParticleChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	currentParticleChunkTracker++;
	if (currentParticleChunkTracker >= BONE_PARTICLE_MAX_NUM)
	{
		currentParticleChunkTracker = 0;
	}
	timerShouldGoOn = true;
	timerCount++;
}

void BackBoneParticleCollection::Init()
{
	for (int i = 0; i < BACKBONE_PARTICLE_MAX_NUM; ++i)
	{
		for (int j = 0; j < BACKBONE_PARTICLE_SPRITE_NUM; ++j)
		{
			particles[i].particles[j].Init(IntVector2(3, 0), ParticleType::RigidParticle);
		}
	}
	particleStopTimer.Init(2.0);
}

void BackBoneParticleCollection::Update(float deltaTime)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < BACKBONE_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Update(deltaTime);
		}
	}
	if (timerShouldGoOn && timerCount)
	{
		if (particleStopTimer.Tick(deltaTime))
		{
			timerCount--;
			if (timerCount <= 0)
			{
				timerShouldGoOn = false;
			}
			particleUpdateChunks.pop_front();
		}
	}
}

void BackBoneParticleCollection::Render(const Vector2 & camPos)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < BACKBONE_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Render(gRenderTarget, camPos);
		}
	}
}

void BackBoneParticleCollection::PlayParticle(const Vector2 & position, float scale)
{
	for (int i = 0; i < BACKBONE_PARTICLE_SPRITE_NUM; ++i)
	{
		particles[currentParticleChunkTracker].particles[i].PlayAt(TilePosition(position));
	}

	if (particleUpdateChunks.size() == 0)
	{
		particleUpdateChunks.push_back(currentParticleChunkTracker);
	}
	else
	{
		for (auto &iter = particleUpdateChunks.begin(); iter != particleUpdateChunks.end();)
		{
			if (*iter != currentParticleChunkTracker)
			{
				particleUpdateChunks.push_back(currentParticleChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	currentParticleChunkTracker++;
	if (currentParticleChunkTracker >= BACKBONE_PARTICLE_MAX_NUM)
	{
		currentParticleChunkTracker = 0;
	}
	timerShouldGoOn = true;
	timerCount++;
}

void ThornParticleCollection::Init()
{
	for (int i = 0; i < THORN_PARTICLE_MAX_NUM; ++i)
	{
		for (int j = 0; j < THORN_PARTICLE_SPRITE_NUM; ++j)
		{
			particles[i].particles[j].Init(IntVector2(5, 0), ParticleType::RigidParticle);
		}
	}
	particleStopTimer.Init(2.0);
}

void ThornParticleCollection::Update(float deltaTime)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < THORN_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Update(deltaTime);
		}
	}
	if (timerShouldGoOn && timerCount)
	{
		if (particleStopTimer.Tick(deltaTime))
		{
			timerCount--;
			if (timerCount <= 0)
			{
				timerShouldGoOn = false;
			}
			particleUpdateChunks.pop_front();
		}
	}
}

void ThornParticleCollection::Render(const Vector2 & camPos)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < THORN_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Render(gRenderTarget, camPos);
		}
	}
}

void ThornParticleCollection::PlayParticle(const Vector2 & position, float scale)
{
	for (int i = 0; i < THORN_PARTICLE_SPRITE_NUM; ++i)
	{
		particles[currentParticleChunkTracker].particles[i].PlayAt(TilePosition(position));
	}

	if (particleUpdateChunks.size() == 0)
	{
		particleUpdateChunks.push_back(currentParticleChunkTracker);
	}
	else
	{
		for (auto &iter = particleUpdateChunks.begin(); iter != particleUpdateChunks.end();)
		{
			if (*iter != currentParticleChunkTracker)
			{
				particleUpdateChunks.push_back(currentParticleChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	currentParticleChunkTracker++;
	if (currentParticleChunkTracker >= THORN_PARTICLE_MAX_NUM)
	{
		currentParticleChunkTracker = 0;
	}
	timerShouldGoOn = true;
	timerCount++;
}

void WoodParticleCollection::Init()
{
	for (int i = 0; i < WOOD_PARTICLE_MAX_NUM; ++i)
	{
		for (int j = 0; j < WOOD_PARTICLE_SPRITE_NUM; ++j)
		{
			particles[i].particles[j].Init(IntVector2(6, 0), ParticleType::RigidParticle);
		}
	}
	particleStopTimer.Init(2.0);
}

void WoodParticleCollection::Update(float deltaTime)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < WOOD_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Update(deltaTime);
		}
	}
	if (timerShouldGoOn && timerCount)
	{
		if (particleStopTimer.Tick(deltaTime))
		{
			timerCount--;
			if (timerCount <= 0)
			{
				timerShouldGoOn = false;
			}
			particleUpdateChunks.pop_front();
		}
	}
}

void WoodParticleCollection::Render(const Vector2 & camPos)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < WOOD_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Render(gRenderTarget, camPos);
		}
	}
}

void WoodParticleCollection::PlayParticle(const Vector2 & position, float scale)
{
	for (int i = 0; i < WOOD_PARTICLE_SPRITE_NUM; ++i)
	{
		particles[currentParticleChunkTracker].particles[i].PlayAt(TilePosition(position));
	}

	if (particleUpdateChunks.size() == 0)
	{
		particleUpdateChunks.push_back(currentParticleChunkTracker);
	}
	else
	{
		for (auto &iter = particleUpdateChunks.begin(); iter != particleUpdateChunks.end();)
		{
			if (*iter != currentParticleChunkTracker)
			{
				particleUpdateChunks.push_back(currentParticleChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	currentParticleChunkTracker++;
	if (currentParticleChunkTracker >= WOOD_PARTICLE_MAX_NUM)
	{
		currentParticleChunkTracker = 0;
	}
	timerShouldGoOn = true;
	timerCount++;
}

void LadderParticleCollection::Init()
{
	for (int i = 0; i < WOOD_PARTICLE_MAX_NUM; ++i)
	{
		for (int j = 0; j < WOOD_PARTICLE_SPRITE_NUM; ++j)
		{
			particles[i].particles[j].Init(IntVector2(2, 1), ParticleType::RigidParticle);
		}
	}
	particleStopTimer.Init(2.0);
}

void LadderParticleCollection::Update(float deltaTime)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < WOOD_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Update(deltaTime);
		}
	}
	if (timerShouldGoOn && timerCount)
	{
		if (particleStopTimer.Tick(deltaTime))
		{
			timerCount--;
			if (timerCount <= 0)
			{
				timerShouldGoOn = false;
			}
			particleUpdateChunks.pop_front();
		}
	}
}

void LadderParticleCollection::Render(const Vector2 & camPos)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < WOOD_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Render(gRenderTarget, camPos);
		}
	}
}

void LadderParticleCollection::PlayParticle(const Vector2 & position, float scale)
{
	for (int i = 0; i < WOOD_PARTICLE_SPRITE_NUM; ++i)
	{
		particles[currentParticleChunkTracker].particles[i].PlayAt(TilePosition(position));
	}

	if (particleUpdateChunks.size() == 0)
	{
		particleUpdateChunks.push_back(currentParticleChunkTracker);
	}
	else
	{
		for (auto &iter = particleUpdateChunks.begin(); iter != particleUpdateChunks.end();)
		{
			if (*iter != currentParticleChunkTracker)
			{
				particleUpdateChunks.push_back(currentParticleChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	currentParticleChunkTracker++;
	if (currentParticleChunkTracker >= WOOD_PARTICLE_MAX_NUM)
	{
		currentParticleChunkTracker = 0;
	}
	timerShouldGoOn = true;
	timerCount++;
}

void BloodParticleCollection::Init()
{
	for (int i = 0; i < BLOOD_PARTICLE_MAX_NUM; ++i)
	{
		for (int j = 0; j < BLOOD_PARTICLE_SPRITE_NUM; ++j)
		{
			particles[i].particles[j].Init(IntVector2(0, 1), ParticleType::RigidParticle);
		}
	}
	particleStopTimer.Init(2.0);
}

void BloodParticleCollection::Update(float deltaTime)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < BLOOD_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Update(deltaTime);
		}
	}
	if (timerShouldGoOn && timerCount)
	{
		if (particleStopTimer.Tick(deltaTime))
		{
			timerCount--;
			if (timerCount <= 0)
			{
				timerShouldGoOn = false;
			}
			particleUpdateChunks.pop_front();
		}
	}
}

void BloodParticleCollection::Render(const Vector2 & camPos)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < BLOOD_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Render(gRenderTarget, camPos);
		}
	}
}

void BloodParticleCollection::PlayParticle(const Vector2 & position, float scale)
{
	for (int i = 0; i < WOOD_PARTICLE_SPRITE_NUM; ++i)
	{
		particles[currentParticleChunkTracker].particles[i].PlayAt(TilePosition(position));
	}

	if (particleUpdateChunks.size() == 0)
	{
		particleUpdateChunks.push_back(currentParticleChunkTracker);
	}
	else
	{
		for (auto &iter = particleUpdateChunks.begin(); iter != particleUpdateChunks.end();)
		{
			if (*iter != currentParticleChunkTracker)
			{
				particleUpdateChunks.push_back(currentParticleChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	currentParticleChunkTracker++;
	if (currentParticleChunkTracker >= WOOD_PARTICLE_MAX_NUM)
	{
		currentParticleChunkTracker = 0;
	}
	timerShouldGoOn = true;
	timerCount++;
}

void BlingParticleCollection::Init()
{
	for (int i = 0; i < BLING_PARTICLE_MAX_NUM; ++i)
	{
		for (int j = 0; j < BLING_PARTICLE_SPRITE_NUM; ++j)
		{
			particles[i].particles[j].Init(IntVector2(0, 3), ParticleType::BlingParticle);
		}
	}
	particleStopTimer.Init(2.0);
}

void BlingParticleCollection::Update(float deltaTime)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < BLING_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Update(deltaTime);
		}
	}
	if (timerShouldGoOn && timerCount)
	{
		if (particleStopTimer.Tick(deltaTime))
		{
			timerCount--;
			if (timerCount <= 0)
			{
				timerShouldGoOn = false;
			}
			particleUpdateChunks.pop_front();
		}
	}
}

void BlingParticleCollection::Render(const Vector2 & camPos)
{
	for (auto &index : particleUpdateChunks)
	{
		for (int i = 0; i < BLING_PARTICLE_SPRITE_NUM; ++i)
		{
			particles[index].particles[i].Render(gRenderTarget, camPos);
		}
	}
}

void BlingParticleCollection::PlayParticle(const Vector2 & position, float scale)
{
	for (int i = 0; i < BLING_PARTICLE_SPRITE_NUM; ++i)
	{
		particles[currentParticleChunkTracker].particles[i].PlayAt(TilePosition(position));
	}
	if (particleUpdateChunks.size() == 0)
	{
		particleUpdateChunks.push_back(currentParticleChunkTracker);
	}
	else
	{
		for (auto &iter = particleUpdateChunks.begin(); iter != particleUpdateChunks.end();)
		{
			if (*iter != currentParticleChunkTracker)
			{
				particleUpdateChunks.push_back(currentParticleChunkTracker);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	currentParticleChunkTracker++;
	if (currentParticleChunkTracker >= BLING_PARTICLE_MAX_NUM)
	{
		currentParticleChunkTracker = 0;
	}
	timerShouldGoOn = true;
	timerCount++;
}
