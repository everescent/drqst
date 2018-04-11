#include "Barrier.h"

Barrier::Barrier(Sprite* const p_Sprite, float x, float y)
	:GameObject{p_Sprite,
	Col_Comp{ x - 100.0f, y - 100.0f,
		      x + 100.0f, y + 100.0f, Rect },
		      x, y }
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
}

void Barrier::Update(Dragon &player, const float &dt)
{
	if (this->IsActive() == true)
	{
		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();

		//check for player projectile
		for (char i = 0; i < Bullet_Buffer; ++i)
		{
			if (player.GetFireball()[i].IsActive())
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