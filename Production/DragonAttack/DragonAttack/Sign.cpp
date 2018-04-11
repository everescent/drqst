#include "Sign.h"

namespace 
{
	float SIGN_SCALE = 70.0f;
}

Sign::Sign(Sprite* const p_Sprite, float x, float y)
	:GameObject{ p_Sprite,
	Col_Comp{ x - SIGN_SCALE, y - SIGN_SCALE,
	x + SIGN_SCALE, y + SIGN_SCALE, Rect },
	x, y }
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
	ShowTutorial = false;
}

void Sign::Update(Dragon &player, const float &dt)
{
	if (this->IsActive() == true)
	{
		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();

		if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt))
			ShowTutorial = true;
		else
			ShowTutorial = false;
	}

}