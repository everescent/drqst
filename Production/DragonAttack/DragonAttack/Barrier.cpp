/* Start Header ************************************************************************/
/*!
\file	 Barrier.cpp
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Barrier class body definitions.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Barrier.h"

// Barrier constructor
Barrier::Barrier(Sprite* const p_Sprite, float x, float y)
	:GameObject{p_Sprite,
	Col_Comp{ x - 100.0f, y - 100.0f,
		      x + 100.0f, y + 100.0f, Rect },
		      x, y } // Set collision positions
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
}

// Update function for Barrier
void Barrier::Update(Dragon &player, const float &dt)
{
	if (this->IsActive() == true)
	{
		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();

		// Check for player projectile
		for (char i = 0; i < Bullet_Buffer; ++i)
		{
			if (player.GetFireball()[i].IsActive()) // Only when fireball is active
			{	
				if (Collision_.Dy_Rect_Rect(player.GetFireball()[i].Collision_,
					this->GetVelocity(), player.GetFireball()[i].GetVelocity(), dt))
				{
					SetActive(false);
					player.GetFireball()[i].Projectile::ResetDist();
					player.GetFireball()[i].SetActive(false);
				}
			}
		}
	}
	
}