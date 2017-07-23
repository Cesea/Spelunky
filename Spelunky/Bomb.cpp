#include "stdafx.h"
#include "Bomb.h"

Bomb::Bomb(ObjectId id)
	:MovingObject(id)
{
}

Bomb::~Bomb()
{
}

HRESULT Bomb::Init(BaseProperty * property)
{
	_sprite = new D2DAnimationSprite();
	Animation *animation = new Animation(); 
	animation->InitCopy(KEYANIMANAGER->FindAnimation(L"bomb"));
	_sprite->Init(IMAGEMANAGER->GetImage(L"bomb"), animation, IntVector2(-40, -40));

	_bombTimer.Init(1.4f);

	_scaleTimer.Init(0.2f);
	mat = D2D1::Matrix3x2F::Identity();

	BombProperty *convertedProperty = (BombProperty *)property;

	position.tileX = property->position.x;
	position.tileY = property->position.y;



	//property->position = position
	return S_OK;
}

void Bomb::Release(void)
{
	_sprite->Release();
	delete _sprite;
}

void Bomb::Update(float deltaTime)
{
	if (_bombTimer.Tick(deltaTime))
	{
		_bombStage++;
		if (_bombStage == 1)
		{
			_sprite->GetAnimation()->Start();
			_yScaling = true;

			_startScale = _scale;
			_targetScale = (_scale == 1.0f) ? 0.4f : 1.0f;
		}
		else if(_bombStage == 2)
		{
			_scaleTimer.ResetAndChangeTargetSecond(0.1f);
			_sprite->GetAnimation()->SetFPS(30);
		}
		else if (_bombStage == 3)
		{
			EVENTMANAGER->QueueEvent(new DestroyObjectEvent(_id));
			EFFECTMANAGER->PlayExplosionEffect(position.UnTilelize());
			EFFECTMANAGER->PlaySmokeEffect(position.UnTilelize());
		}
	}
	_sprite->Update(deltaTime);
	if (_yScaling)
	{
		if (_scaleTimer.Tick(deltaTime))
		{
			_scale = _targetScale;
			_startScale = _scale;
			_targetScale = (_scale == 1.0f) ? 0.4f : 1.0f;
		}

		float t = _scaleTimer.GetCurrentSecond() / _scaleTimer.GetTargetSecond();
		_scale = InterpolateFloat(_startScale, _targetScale, t);
			
		mat = D2D1::Matrix3x2F::Scale(_scale, _scale, D2D1::Point2F(40, 40));
	}
}

void Bomb::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->RenderMatrix(renderTarget, drawPos.x, drawPos.y, mat);
}

GameObject * Bomb::Copy(ObjectId id)
{
	return nullptr;
}
