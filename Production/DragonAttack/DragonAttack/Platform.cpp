/* Start Header ************************************************************************/
/*!
\file	 Platform.cpp
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Platform class body definitions.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Platform.h"
#include <iostream>

Platform::Platform(Sprite* const p_Sprite, float x, float y)
	: GameObject{ p_Sprite,
	Col_Comp{ x - PLAT_WIDTH, y - PLAT_HEIGHT,
	x + PLAT_WIDTH, y + PLAT_HEIGHT, Rect },
	x, y }
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
}

// Update function for Platform for player
void Platform::Update(Dragon &player, const float &dt)
{
	bool Collide = Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt);
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();

	if (Collide && !player.GetJump()) // Collision when not in the air
	{
    player.SetTouchBottom(true);
		if (player.PosY - player.Sprite_->Get_Height() < PosY)
			return;
		if (player.PosY <= PosY + Sprite_->Get_Height() + player.Sprite_->Get_Height())
			player.PosY = PosY + Sprite_->Get_Height() + player.Sprite_->Get_Height(); // Move player back
	}
}

// Update function for Platform for AI
void Platform::Update(Characters &obj, const float &dt)
{
	bool Collide = Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(),
		obj.GetVelocity(), dt);
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();

	if (Collide) // Collision with AI
	{
		if (obj.PosY - obj.Sprite_->Get_Height() < PosY)
			return;
		if (obj.PosY <= PosY + Sprite_->Get_Height() + obj.Sprite_->Get_Height())
			obj.PosY = PosY + Sprite_->Get_Height() +  obj.Sprite_->Get_Height(); //  Move AI back
	}
}
