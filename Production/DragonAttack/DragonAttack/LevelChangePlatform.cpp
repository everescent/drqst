/* Start Header ************************************************************************/
/*!
\file	 LevelChangePlatform.cpp
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	LevelChangePlatform class body definitions.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "LevelChangePlatform.h"
#include <iostream>

namespace 
{
	// Fade effect variables
	float timer = 3.0f;
	static float vis = 0.0f;
}
 // LevelChangePlatform Constructor
LevelChangePlatform::LevelChangePlatform(Sprite* const p_Sprite, float x, float y)
	: GameObject{ p_Sprite,
	Col_Comp{ x - 10.0f, y - 10.0f,
			  x + 100.0f, y + 10.0f, Rect },
			  x, y } // Set collision positions
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
}
 // Update function for LevelChangePlatform
void LevelChangePlatform::Update(Dragon &player, const float &dt, Sprite& black, bool& FadeOut)
{
	bool Collide = Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(),
										   player.GetVelocity(), dt);
	
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();

	if (Collide) // When player collides
	{
		player.SetUpdateFlag(false);
		FadeOut = true;
		if (FadeOut) // Fade in effect
		{
			black.SetAlphaTransBM(1.0f, vis, AE_GFX_BM_BLEND);
			vis += 0.005f; // Increase visibility every loop
			timer -= dt;

			if (timer <= 0) // Stops fade out once timer reaches 0
			{
				FadeOut = false;
			}
			if (FadeOut)
			{
				return;
			}
		}
		// Switch case to change between game states depending on current level
		switch (SM::Get_Curr()) 
		{
			case STAGE_1_1:
				SM::Set_After_Score(STAGE_1_2);
				break;
			case STAGE_1_2:
				SM::Set_After_Score(STAGE_1_3);
				break;
			case STAGE_1_3:
				SM::Set_Next(SS_LOAD); // Send to load state
				SM::Set_After_Score(STAGE_2_1);
				break;
			case STAGE_2_1:
				SM::Set_After_Score(STAGE_2_2);
				break;
			case STAGE_2_2:
				SM::Set_After_Score(STAGE_2_3);
				break;
			case STAGE_2_3:
				SM::Set_Next(SS_LOAD); // Send to load state
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
		// Reset fade effect variables
		timer = 3.0f;
		vis = 0.0f;
		player.SetUpdateFlag(true); // Allow player to update

	// Set next state for boss stages to the next stage directly
    if(SM::Get_Curr() != STAGE_1_3 && SM::Get_Curr() != STAGE_2_3)
		  SM::Set_Next(SS_SCORE);

	}
}
