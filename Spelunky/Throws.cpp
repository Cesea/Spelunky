#include "stdafx.h"
#include "Throws.h"

Throws::Throws(ObjectId id)
	:EquipItem(id)
{
}

Throws::~Throws()
{
}

HRESULT Throws::Init(BaseProperty *property)
{
	EquipItem::Init(property);
	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"throws"), 80, 80, IntVector2(-40, -40));

	_speed = Vector2(400, 300);
	_maxVelocity = Vector2(300, 550);

	ThrowProperty *convertedProperty = (ThrowProperty *)(property);
	*this = convertedProperty;

	return S_OK;
}

void Throws::Release(void)
{
	EquipItem::Release();
	SAFE_RELEASE_AND_DELETE(_sprite);
}

void Throws::Update(float deltaTime)
{
	EquipItem::Update(deltaTime);
}

void Throws::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	Vector2 drawPos = position.UnTilelize() - camPos;
	_sprite->FrameRender(renderTarget, drawPos.x, drawPos.y, _sourceIndex.x, _sourceIndex.y);
}

void Throws::Use(const ControlCommand &commands)
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

GameObject * Throws::Copy(ObjectId id)
{
	return new Throws(id);
}

void Throws::Apply(ObjectId id)
{
}

void Throws::operator=(const ThrowProperty * property)
{
	position.tileX = property->position.x;
	position.tileY = property->position.y;
	_sourceIndex = property->sourceIndex;
	_breakable = property->breakable;
}
