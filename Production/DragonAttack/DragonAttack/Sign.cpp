/* Start Header ************************************************************************/
/*!
\file	 Sign.cpp
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Sign class body definitions.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Sign.h"

namespace 
{
	float SIGN_SCALE = 70.0f;
}

// Sign constructor
Sign::Sign(Sprite* const p_Sprite, float x, float y)
	:GameObject{ p_Sprite,
	Col_Comp{ x - SIGN_SCALE, y - SIGN_SCALE,
	x + SIGN_SCALE, y + SIGN_SCALE, Rect },
	x, y } // Set collision positions
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
	ShowTutorial = false;
}

// Update function for Sign
void Sign::Update(Dragon &player, const float &dt)
{
	if (this->IsActive() == true)
	{
		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();

		if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt))
			ShowTutorial = true; // Show tutorial if there is collision
		else
			ShowTutorial = false;
	}

}