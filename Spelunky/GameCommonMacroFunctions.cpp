#include "stdafx.h"
#include "GameCommonMacroFunctions.h"

const char * ConvertEntityStateEnumToString(EntityState state)
{
	switch(state)
	{
	case EntityState::Idle:
	{
		return "Idle";
	}break;
	case EntityState::CrawlUp:
	{
		return "CrawlUp";
	}break;
	case EntityState::Grabbing :
	{
		return "Grabbing";
	}break;
	case EntityState::CrawlDown:
	{
		return "CrawlDown";
	}break;
	case EntityState::CrawlMove:
	{
		return "CrawlMove";
	}break;
	case EntityState::CrawlIdle :
	{
		return "CrawlIdle";
	}break;
	case EntityState::LookUp :
	{
		return "LookUp";
	}break;
	case EntityState::LookDown :
	{
		return "LookDown";
	}break;
	case EntityState::Walking:
	{
		return "Walking";
	}break;
	case EntityState::Jumping:
	{
		return "Jumping";
	}break;
	case EntityState::Falling:
	{
		return "Falling";
	}break;
	case EntityState::Climbing:
	{
		return "Climbing";
	}break;
	case EntityState::OnLedge:
	{
		return "OnLedge";
	}break;
	case EntityState::Attacking:
	{
		return "Attacking";
	}break;
	case EntityState::Throwing:
	{
		return "Throwing";
	}break;
	case EntityState::Hurt:
	{
		return "Hurt";
	}break;
	case EntityState::Faint:
	{
		return "Faint";
	}break;
	case EntityState::Dead:
	{
		return "Dead";
	}break;
	}
}

const char * ConvertArchTypeEnumToString(ArcheType type)
{
	switch (type)
	{
	case ArcheType::Player:
	{
		return "Player";
	}break;
	case ArcheType::Tile:
	{
		return "Tile";
	}
	case ArcheType::MineBG :
	{
		return "MineBG";
	}break;
	}
}



void DrawBox(ID2D1HwndRenderTarget *renderTarget, float x, float y, float width, float height, const D2D1_COLOR_F &color)
{
	ID2D1SolidColorBrush *brush = nullptr;
	renderTarget->CreateSolidColorBrush(color, &brush);
	renderTarget->DrawRectangle(D2D1::RectF(x, y, x + width, y + height), brush);
	brush->Release();
}

void FillBox(ID2D1HwndRenderTarget * renderTarget, float x, float y, float width, float height, const D2D1_COLOR_F & color)
{
	ID2D1SolidColorBrush *brush = nullptr;
	renderTarget->CreateSolidColorBrush(color, &brush);
	renderTarget->FillRectangle(D2D1::RectF(x, y, x + width, y + height), brush);
	brush->Release();
}

void DrawBoxFromCenter(ID2D1HwndRenderTarget *renderTarget, const Rect &rect, const D2D1_COLOR_F & color)
{
	ID2D1SolidColorBrush *brush = nullptr;
	renderTarget->CreateSolidColorBrush(color, &brush);
	renderTarget->DrawRectangle(D2D1::RectF(rect.x - (rect.width / 2.0f), rect.y - (rect.height / 2),
		rect.x + (rect.width / 2.0f), rect.y + (rect.height / 2.0f)), brush);
	brush->Release();
}

void DrawCircleInterpolated(ID2D1HwndRenderTarget * renderTarget, 
	const ScreenVector2 & start, const ScreenVector2 & end, float t, ID2D1SolidColorBrush *brush)
{
	ScreenVector2 drawPosition = InterpolateVector(start, end, t);
	renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(drawPosition.x, drawPosition.y), 5, 5), brush);
	

}
