#include "Barrier.h"
#include <iostream>
Barrier::Barrier(float x, float y)
	:// GameObject{ S_CreateRectangle(120.0f, 100.0f, ".//Textures/box.png"),
	GameObject{S_CreateSquare(130.0f, ".//Textures/box.png" ),
	Col_Comp{ x - 100.0f, y - 100.0f,
		      x + 100.0f, y + 100.0f, Rect },
		      x, y }
{
	SetActive(true);
}

void Barrier::Update(Dragon &player, const float &dt)
{
	if (this->IsActive() == true)
	{
		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();

		if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt))
		{
			if (player.PosX > this->PosX)
			{
				player.PosX = PosX + Sprite_.Get_Width() + player.Sprite_.Get_Width();
			}
			else if (player.PosX < this->PosX)
			{
				player.PosX = PosX - Sprite_.Get_Width() - player.Sprite_.Get_Width();
			}
		}


		//check for player projectile
		for (char i = 0; i < Bullet_Buffer; ++i)
		{
			if (player.GetFireball()[i].IsActive())
			{	
				if (Collision_.Dy_Rect_Rect(player.GetFireball()[i].Collision_,
					this->GetVelocity(), player.GetFireball()[i].GetVelocity(), dt))
				{
					//Decrease_HP(player.GetDamage());
					SetActive(false);
					player.GetFireball()[i].Projectile::ResetDist();
					player.GetFireball()[i].SetActive(false);
				}
			}
		}
	}
	
}