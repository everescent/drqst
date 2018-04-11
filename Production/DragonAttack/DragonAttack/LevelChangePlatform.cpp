#include "LevelChangePlatform.h"
#include <iostream>

namespace 
{
	float timer = 3.0f;
	static float vis = 0.0f;
}
LevelChangePlatform::LevelChangePlatform(Sprite* const p_Sprite, float x, float y)
	: GameObject{ p_Sprite,
	Col_Comp{ x - 10.0f, y - 10.0f,
			  x + 100.0f, y + 10.0f, Rect },
			  x, y }
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
}

void LevelChangePlatform::Update(Dragon &player, const float &dt, Sprite& black, bool& FadeOut)
{
	bool Collide = Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(),
										   player.GetVelocity(), dt);
	
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
	if (Collide)
	{
		FadeOut = true;
		if (FadeOut) // Fade in effect
		{
			black.SetAlphaTransBM(1.0f, vis, AE_GFX_BM_BLEND);
			vis += 0.005f;
			timer -= dt;

			if (timer <= 0)
			{
				FadeOut = false;
			}
			if (FadeOut)
				return;
		}

		switch (SM::Get_Curr()) 
		{
			case STAGE_1_1:
				SM::Set_After_Score(STAGE_1_2);
				break;
			case STAGE_1_2:
				SM::Set_After_Score(STAGE_1_3);
				break;
			case STAGE_1_3:
				SM::Set_Next(STAGE_2_1);
				break;
			case STAGE_2_1:
				SM::Set_After_Score(STAGE_2_2);
				break;
			case STAGE_2_2:
				SM::Set_After_Score(STAGE_2_3);
				break;
			case STAGE_2_3:
				SM::Set_Next(STAGE_3_1);
				break;
			case STAGE_3_1:
				SM::Set_After_Score(STAGE_3_2);
				break;
			case STAGE_3_2:
				SM::Set_After_Score(STAGE_3_3);
				break;
			default:
				break;
		}
		timer = 3.0f;
		vis = 0.0f;
    if(SM::Get_Curr() != STAGE_1_3 && SM::Get_Curr() != STAGE_2_3)
		  SM::Set_Next(SS_SCORE);
	}
}
