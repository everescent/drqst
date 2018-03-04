#include "Knight.h"

namespace
{
	const int HP = 100;
	const float KNIGHT_SCALE = 30.0f;
	const float ATTACK_RANGE = 50.0f;
}

Knight::Knight(const AEVec2 & spawn_location)
	: Characters(S_CreateSquare(KNIGHT_SCALE, ".//Textures/Knight.png"),
	  HP, Col_Comp{ spawn_location.x - KNIGHT_SCALE, spawn_location.y - KNIGHT_SCALE , 
					spawn_location.x + KNIGHT_SCALE , spawn_location.y + KNIGHT_SCALE, Rect }),
	  current_action{ IDLE }
{
	SetPos(spawn_location.x, spawn_location.y);
	SetActive(false);
	Transform_.SetTranslate(spawn_location.x, spawn_location.y);
	Transform_.Concat();
	Reset_Idle_Time(2.0f);
	SetVelocity(AEVec2{ 50.0f, 0.0f });
	Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);			   // rendering mode of mage
}

void Knight::Idle(const float dt)
{
	if (Get_Idle_Time() < 0)
	{
		Reset_Idle_Time(2.0f);
		current_action = MOVING;

	}
	else
	{
		Decrease_Idle_Time(dt);
	}
}

void Knight::Moving(const Dragon &d, const float dt)
{
	if (Line_Of_Sight(d))
	{
	}
	//else if(d.PosX - PosX <= ATTACK_RANGE)
	{
		current_action = ATTACK;
	}
}

void Knight::Update(Dragon &d, const float dt)
{
	
}