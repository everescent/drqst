/* Start Header ************************************************************************/
/*!
\file	 Tower.cpp
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Tower class body definitions.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Tower.h"

// Tower constructor
Tower::Tower(Sprite* const p_Sprite, float x, float y)
	: GameObject{ p_Sprite,
	Col_Comp{ x - 100.0f, y - 300.0f,
			  x + 100.0f, y + 190.0f, Rect },
			  x, y } // Set collision positions
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
}

// Update function for Tower for player
void Tower::Update(Dragon &player, const float &dt)
{
	if (this->IsActive() == true)
	{
		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();
		 
		// Push back player on collision
		if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt))
		{
			if (player.PosX > this->PosX)
			{
				player.PosX = PosX + (Sprite_->Get_Width() - 200.0f) + player.Sprite_->Get_Width();
			}
			else if (player.PosX < this->PosX)
			{
				player.PosX = PosX - (Sprite_->Get_Width() - 200.0f) - player.Sprite_->Get_Width();
			}
		}

		// Destroy when hit by mega fireball
		if (Collision_.Dy_Rect_Rect(player.GetMfireball().Collision_, this->GetVelocity(), player.GetMfireball().GetVelocity(), dt))
		{
			SetActive(false);
			player.GetMfireball().Projectile::ResetDist();
			player.GetMfireball().SetActive(false);
		}

		// Charge when hit by normal fireball
		for (char i = 0; i < Bullet_Buffer; ++i)
		{
			if (player.GetFireball()[i].IsActive())
			{
				if (Collision_.Dy_Rect_Rect(player.GetFireball()[i].Collision_, this->GetVelocity(), player.GetFireball()[i].GetVelocity(), dt))
				{
					player.AddCharge();
					player.GetFireball()[i].Projectile::ResetDist();
					player.GetFireball()[i].SetActive(false);
				}
			}
		}
	}
}

// Update function for Tower for AI
void Tower::Update(Characters &obj, const float &dt)
{
	if (this->IsActive() == true)
	{
		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();

		// Push back AI on collision
		if (Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(), obj.GetVelocity(), dt))
		{
			if (obj.PosY > this->PosY)
			{
				obj.PosY = PosY + (Sprite_->Get_Height() - 110.0f) + obj.Sprite_->Get_Height();
			}
		}

	}
}