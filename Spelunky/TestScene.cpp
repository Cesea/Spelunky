#include "stdafx.h"
#include "TestScene.h"

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}

HRESULT TestScene::LoadContent()
{
	IMAGEMANAGER->LoadImageFromFile(L"resources\\gfx\\dustring.png", L"dustring");


	return S_OK;
}

Vector2 TestScene::GetRandomVector2(float xMax, float yMax)
{
	return Vector2(RND->GetFloat(-xMax, xMax), RND->GetFloat(-yMax, yMax));
}

Vector2 TestScene::GetRandomVector2(float xMin, float xMax, float yMin, float yMax)
{
	return Vector2(RND->GetFloat(xMin, xMax), RND->GetFloat(yMin, yMax));
}

void TestScene::MakeParticles(const Vector2 & mousePos)
{
	for (int i = 0; i < 20; ++i)
	{
		_particles[i].position = mousePos;
		_particles[i].position += GetRandomVector2(-20, 20, -10, 0);

		_particles[i].alpha = 1.0f;

		float randomScale = RND->GetFloat(0.1, 0.3);
		_particles[i].scale = Vector2(randomScale, randomScale);

		_particles[i].MoveBy(GetRandomVector2(-50, 50, -60, -150), RND->GetFloat(2.0f, 4.0f));
		_particles[i].DisappearTo(0, RND->GetFloat(2.0f, 4.0f));
	}
}


HRESULT TestScene::Init(void)
{
	HRESULT result = LoadContent();
	Assert(SUCCEEDED(result));

	std::wstring moduleLocation = Utils::GetWorkingDirectory();
	std::vector<std::pair<std::wstring, bool>> files = Utils::GetFileList(moduleLocation);

	_particle.Init(L"dustring", 0, 0, 512, 512, IntVector2(-256, -256));

	_particle.position = Vector2(200, 200);

	for (int i = 0; i < 20; ++i)
	{
		_particles[i].Init(L"dustring", 0, 0, 512, 512, IntVector2(-256, -256));
	}

	return S_OK;
}

void TestScene::Release(void)
{
	_particle.Release();
}

void TestScene::Update(void)
{
	float deltaTime = TIMEMANAGER->GetElapsedTime();

	Win32RawInputState rawInput = {};
	PullRawInput(&rawInput);

	ControlCommand playerCommand = _inputMapper.InterpretRawInput(&rawInput);

	if (playerCommand.action == Command::Attack)
	{
		MakeParticles(currentMouse);
	}

	for (int i = 0; i < 20; ++i)
	{
		_particles[i].Update(deltaTime);
	}
}

void TestScene::Render()
{
	//그리기 전에는 항상 BeginDraw()
	gRenderTarget->BeginDraw();
	gRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));

	Vector2 camPos{};

	for (int i = 0; i < 20; ++i)
	{
		_particles[i].RenderScale(gRenderTarget, camPos);
	}

	//그린 후에는 항상 EndDraw()
	gRenderTarget->EndDraw();
}


