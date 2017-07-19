#include "stdafx.h"
#include "Rock.h"

Rock::Rock(ObjectId id)
	:EquipItem(id)
{
}

Rock::~Rock()
{
}

HRESULT Rock::Init(BaseProperty *property)
{
	EquipItem::Init(property);
	_sprite = new D2DSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"rock"), 0, 0, 80, 80, IntVector2(-40, -50));

	_speed = Vector2(400, 300);
	_maxVelocity = Vector2(300, 550);

	return S_OK;
}

void Rock::Release(void)
{
	EquipItem::Release();
	SAFE_RELEASE_AND_DELETE(_sprite);
}

void Rock::Update(float deltaTime)
{
	EquipItem::Update(deltaTime);
}

void Rock::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	EquipItem::Render(renderTarget, camPos);
}

void Rock::Use(const ControlCommand &commands)
{
	_equiped = false;

	if (commands.horizontal == Command::None)
	{
		if (_pOwner->GetDirection() == Direction::Left)
		{
			_velocity.x -= 900;
		}
		else if (_pOwner->GetDirection() == Direction::Right)
		{
			_velocity.x += 900;
		}
	}
	else
	{
		if (commands.horizontal == Command::MoveLeft)
		{
			_velocity.x -= 900;
		}
		else if (commands.horizontal == Command::MoveRight)
		{
			_velocity.x += 900;
		}
	}

	if (commands.vertical == Command::MoveUp)
	{
		_velocity.y -= 600;
	}
	else if(commands.vertical == Command::MoveDown)
	{
		_velocity.y = 100;
	}
	else if (commands.vertical == Command::None)
	{
		_velocity.y -= 500;
	}

	_velocity.Normalize();
	_velocity *= 1000.0f;


	_pOwner = nullptr;
}

GameObject * Rock::Copy(ObjectId id)
{
	return nullptr;
}

void Rock::Apply(ObjectId id)
{
}
