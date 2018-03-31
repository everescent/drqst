#include "LevelChangePlatform.h"
#include <iostream>

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
//(Jacob) Updated to make dragon stand on platform
void LevelChangePlatform::Update(Dragon &player, const float &dt)
{
	bool Collide = Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(),
										   player.GetVelocity(), dt);
	
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
	
	if (Collide)
	{
		switch (SM::Get_Curr()) 
		{
			case STAGE_1_1:
				SM::Set_After_Score(STAGE_1_2);
				break;
			case STAGE_1_2:
				SM::Set_After_Score(STAGE_1_3);
				break;
			case STAGE_1_3:
				SM::Set_After_Score(STAGE_2_1);
				break;
			case STAGE_2_1:
				SM::Set_After_Score(STAGE_2_2);
				break;
			case STAGE_2_2:
				SM::Set_After_Score(STAGE_2_3);
				break;
			case STAGE_2_3:
				SM::Set_After_Score(STAGE_3_1);
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
		SM::Set_Next(SS_SCORE);
	}
}
