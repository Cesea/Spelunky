#include "stdafx.h"
#include "Particle.h"

#include "CollisionComponent.h"

Particle::Particle()
{
}

Particle::~Particle()
{
}

HRESULT Particle::Init(const IntVector2 &sourceIndex, ParticleType particleType)
{
	_sourceIndex = sourceIndex;
	_particleType = particleType;

	//_collisionComp = new CollisionComponent;
	//_collisionComp->Init(RectMake(0, 0, 48, 48), Vector2(-24, -24));
	_firstRect = RectMake(0, 0, 48, 48);
	_rect = RectMake(0, 0, 48, 48);
	_rectOffset = Vector2(-24, -24);
	_firstOffset = Vector2(-24, -24);

	_firstScale = 1.0f;

	_sprite = new D2DFrameSprite;
	_sprite->Init(IMAGEMANAGER->GetImage(L"particles"), 64, 64, IntVector2(-32, -32));

	return S_OK;
}

void Particle::Release(void)
{
	SAFE_RELEASE_AND_DELETE(_sprite);
}

void Particle::Update(float deltaTime)
{
	switch (_particleType)
	{
	case RigidParticle  :
	{
		if (_valid)
		{
			_accel.y += GRAVITY;

			_nearTiles = STAGEMANAGER->GetCurrentStage()->GetAdjacent9(IntVector2(position.tileX, position.tileY));
			_velocity += _accel * deltaTime;

			_accel = Vector2();

			if (_lifeTimer.Tick(deltaTime))
			{
				_valid = false;
			}
			float t = 1.0f - _lifeTimer.GetCurrentSecond() / _lifeTimer.GetTargetSecond();

			//_sprite->SetCurrentAnchor(IntVector2(32 * t, 32 * t));

			_scale = _firstScale * t;

			if (_scale < 0.15)
			{
				_valid = false;
			}
			_degrees += _rotationDirection;
			_rect.width = _firstRect.width * t;
			_rect.height = _firstRect.height * t;
			//_rectOffset = _firstOffset * t;

			desiredPosition.AddToTileRel(_velocity * deltaTime);

			CalculateCollision();
		}
	}break;
	case BlingParticle :
	{
		_lifeTimer.Tick(deltaTime);
		float t = 1.0f - _lifeTimer.GetCurrentSecond() / _lifeTimer.GetTargetSecond();

		_scale = _firstScale * t;

		if (_scale < 0.15)
		{
			_valid = false;
		}
		_degrees += _rotationDirection;

		desiredPosition.AddToTileRel(_velocity * deltaTime);
		position = desiredPosition;
	}break;
	case SparkParticle:
	{

	}break;
	}

}

void Particle::Render(ID2D1HwndRenderTarget * renderTarget, const Vector2 & camPos)
{
	if (_valid)
	{
		Vector2 drawPos = position.UnTilelize() - camPos;
		_sprite->FrameRenderMatrix(gRenderTarget, drawPos.x, drawPos.y, _sourceIndex.x, _sourceIndex.y,
		D2D1::Matrix3x2F::Rotation(_degrees, D2D1::Point2F(_firstRect.width / 2, _firstRect.height / 2)) * 
			D2D1::Matrix3x2F::Scale(_scale, _scale, D2D1::Point2F(_firstRect.width / 2, _firstRect.height / 2)) );

		//const Vector2 itemUntiledPosition = position.UnTilelize();
		//Rect itemAbsRect =
		//	RectMake(itemUntiledPosition.x, itemUntiledPosition.y, _rect.width, _rect.height);
		//itemAbsRect += _rectOffset;

		//DrawBox(gRenderTarget, itemAbsRect.x - camPos.x, itemAbsRect.y - camPos.y, itemAbsRect.width, itemAbsRect.height, D2D1::ColorF(1.0f, 1.0f, 0.0f));
	}
}

void Particle::PlayAt(const TilePosition &position, float scaleMult)
{
	this->position = position;
	this->desiredPosition = this->position;

	switch (_particleType)
	{
	case RigidParticle  :
	{
		float randScale = RND->GetFloat(0.3, 1.0f) * scaleMult;
		_firstScale = randScale;
		_scale = randScale;
		_lifeTimer.Init(randScale * 2.0f);
		_valid = true;

		float randDirection = (RND->GetFloat() <= 0.5f) ? -1.0f : 1.0f;
		_velocity.x = randScale * 400 * randDirection;
		_velocity.y = randScale * -400 - 150;

		if (_velocity.x > 0)
		{
			_rotationDirection = 6.9 * randScale +3;
		}
		else
		{
			_rotationDirection = -6.9 * randScale + 3;
		}
	}break;
	case BlingParticle :
	{
		_valid = true;
		float randScale = RND->GetFloat(0.4f, 0.8f);
		_lifeTimer.Init(randScale * 2.0f);
		float randOffsetAngle = randScale * M_PI2;
		this->position.AddToTileRel(cosf(randOffsetAngle) * 20, -sinf(randOffsetAngle) * 20);
		
		_firstScale = randScale;
		_scale = randScale;


		_velocity.y = -randScale * 20.0f;
		float randDirection = (RND->GetFloat() <= 0.5f) ? -1.0f : 1.0f;
		if (randDirection < 0.0f)
		{
			_rotationDirection = 6.9 * randScale +3;
		}
		else
		{
			_rotationDirection = -6.9 * randScale + 3;
		}

	}break;
	case SparkParticle :
	{

	}break;
	}
}


void Particle::CalculateCollision()
{
	for (int i = 0; i < 8; ++i)
	{
		if (_nearTiles.tiles[i] == nullptr ||
			_nearTiles.tiles[i]->sourceIndex.x == -1)
			continue;

		Tile *currentTile = _nearTiles.tiles[i];

		Rect absRect = desiredPosition.UnTilelize() + _rect + _rectOffset;
		Rect tileRect = RectMake(currentTile->position.tileX * TILE_SIZE, currentTile->position.tileY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

		Rect overlapRect;
		//�Ʒ� Ÿ���� ��� üũ�� �� �־�� �� ��찡 �ִ�. ��ٸ�, ������ �������� �״� Ÿ�� ��....
		if (i == 0)
		{
			if (IsRectangleOverlap(absRect, tileRect, overlapRect))
			{
				if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
				{
					desiredPosition.AddToTileRel(0, -overlapRect.height);
					_velocity.y *= -0.3f;
					_velocity.x *= 0.6;
				}
			}
		}
		else
		{
			if (currentTile->collisionType == TileCollisionType::TILE_COLLISION_BLOCK)
			{
				if (IsRectangleOverlap(absRect, tileRect, overlapRect))
				{
					//�� Ÿ��
					if (i == 1)
					{
						desiredPosition.AddToTileRel(0, overlapRect.height);
						_velocity.y = 0.0f;
						_velocity.x *= 0.6;
					}
					//�� Ÿ��
					else if (i == 2)
					{
						desiredPosition.AddToTileRel(overlapRect.width, 0);
						_velocity.x *= -0.6f;
					}
					//���� Ÿ��
					else if (i == 3)
					{
						desiredPosition.AddToTileRel(-overlapRect.width, 0);
						_velocity.x *= -0.6f;
					}
					//�밢��
					else
					{
						//�������� �浹�� �Ͼ
						if (overlapRect.width > overlapRect.height)
						{
							_velocity.y = 0.0f;
							float pushingHeight;
							if (i == 4 || i == 6)
							{
								pushingHeight = -overlapRect.height;
								_velocity.y *= -0.4f;
							}
							else
							{
								pushingHeight = overlapRect.height;
								_velocity.y = 0.0f;
							}
							desiredPosition.AddToTileRel(0, pushingHeight);
						}
						//�������� �浹�� �Ͼ
						else
						{
							_velocity.x = 0.0f;
							float pushingWidth;
							if (i == 5 || i == 6)
							{
								pushingWidth = overlapRect.width;
								_velocity.x *= -0.5f;
							}
							else
							{
								pushingWidth = -overlapRect.width;
								_velocity.x *= -0.5f;
							}
							desiredPosition.AddToTileRel(pushingWidth, 0);
						}
					}
				}
			}
		}
	}
	position = desiredPosition;
}

