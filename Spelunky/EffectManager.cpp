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
		_wasInitialized = true;
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < DUST_SPRITE_NUM; ++j)
			{
				if (j == 0)
				{
					//_dustChunks[i].sprite[j].
					//	SetDisappearEndFunction(Delegate<void>::FromFunction<EffectManager, &EffectManager::DustSpriteEnd>(this));
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
}

void EffectManager::ExplosionEffectEndFunction()
{
	_explosionShouldPop = true;
}
