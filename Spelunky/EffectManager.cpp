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
}

void EffectManager::Render()
{
	//Vector2 camPosUntTiled = _pCamera->GetPosition().UnTilelize();
	for (auto &index : _dustUpdateChunks)
	{
		for (int i = 0; i < DUST_SPRITE_NUM; ++i)
		{
			_dustChunks[index].sprite[i].RenderScale(gRenderTarget, Vector2());
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
