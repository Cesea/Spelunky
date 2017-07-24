#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "IScene.h"

#include "SpriteObject.h"

//#include "Bomb.h"

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

	//private jtions
private:
	ObjectId _lastId{ 1 };
	ObjectId GetNextId() { return _lastId++; }

	//Bomb* _bomb{};

	Camera cameraTemp;


};


#endif