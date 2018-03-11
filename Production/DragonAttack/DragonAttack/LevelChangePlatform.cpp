#include "LevelChangePlatform.h"
#include <iostream>

namespace
{
	Sprite LCPLAT_SPRITE = CreatePlatform(2.0f, 3.0f, ".//Textures/Win_Platform.png");
}


LevelChangePlatform::LevelChangePlatform(float x, float y)
	: GameObject{ &LCPLAT_SPRITE,
	Col_Comp{ x - 10.0f, y - 10.0f,
			  x + 100.0f, y + 10.0f, Rect },
			  x, y }
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
}
//(Jacob) Updated to make dragon stand on platform
void LevelChangePlatform::Update(Dragon &player, const float &dt)
{
	bool Collide = Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(),
										   player.GetVelocity(), dt);
	
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
	
	if (Collide)
	{
		switch (GSM::current) 
		{
			case GS_LEVEL1_1:
				GSM::next = GS_LEVEL1_2;
				break;
			case GS_LEVEL1_2:
				GSM::next = GS_LEVEL1_3;
				break;
			case GS_LEVEL1_3:
				GSM::next = GS_MAIN;
				break;
			default:
				break;
		}
	}
}
