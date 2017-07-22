#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "IScene.h"

#include "SpriteObject.h"

#define SPEAR_TIME 0.25f

class TestScene : public IScene
{
public:
	TestScene();
	virtual ~TestScene();

	virtual HRESULT Init(void) override;
	virtual void Release(void) override;
	virtual void Update(void) override;
	virtual void Render(void) override;

	virtual HRESULT LoadContent() override;


	Vector2 GetRandomVector2(float xMax, float yMax);
	Vector2 GetRandomVector2(float xMin, float xMax, float yMin, float yMax);
	void MakeParticles(const Vector2 &mousePos);

	//private jtions
private:
	ObjectId _lastId{ 1 };
	ObjectId GetNextId() { return _lastId++; }

	SpriteObject _particle;

	SpriteObject _particles[20];
};


#endif