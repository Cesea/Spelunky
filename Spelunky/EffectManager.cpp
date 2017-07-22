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
		for (int j = 0; j < 25; ++j)
		{
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
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 25; ++j)
		{
			_dustChunks[i].sprite[j].Update(deltaTime);
		}
	}
}

void EffectManager::Render()
{
	//Vector2 camPosUntTiled = _pCamera->GetPosition().UnTilelize();
	for (int i = 0; i < 1; ++i)
	{
		for (int j = 0; j < 25; ++j)
		{
			_dustChunks[i].sprite[j].RenderScale(gRenderTarget, Vector2());
		}
	}
}

void EffectManager::PlayDustParticles(const Vector2 & position)
{
	for (int i = 0; i < 25; ++i)
	{
		_dustChunks[_currentDustChunkTracker].sprite[i].position = position;
		_dustChunks[_currentDustChunkTracker].sprite[i].position += GetRandomVector2(-30, 30, -10, 0);

		_dustChunks[_currentDustChunkTracker].sprite[i].alpha = RND->GetFloat(0.4, 0.8);

		float randomScale = RND->GetFloat(0.15, 0.3);
		_dustChunks[_currentDustChunkTracker].sprite[i].scale = Vector2(randomScale, randomScale);

		_dustChunks[_currentDustChunkTracker].sprite[i].MoveBy(GetRandomVector2(-50, 50, -60, -150), RND->GetFloat(2.0f, 5.0f));
		_dustChunks[_currentDustChunkTracker].sprite[i].DisappearTo(0, RND->GetFloat(2.0f, 5.0f));
	}

	_currentDustChunkTracker++;
}

Vector2 EffectManager::GetRandomVector2(float xMax, float yMax)
{
	return Vector2(RND->GetFloat(-xMax, xMax), RND->GetFloat(-yMax, yMax));
}

Vector2 EffectManager::GetRandomVector2(float xMin, float xMax, float yMin, float yMax)
{
	return Vector2(RND->GetFloat(xMin, xMax), RND->GetFloat(yMin, yMax));
}
