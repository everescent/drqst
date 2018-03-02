#include "Tower.h"

Tower::Tower(float x, float y)
	: GameObject{ S_CreateRectangle(300.0f, 300.0f, ".//Textures/tower.png"),
	Col_Comp{ x - 100.0f, y - 300.0f,
			  x + 100.0f, y + 300.0f, Rect },
			  x, y }
{
	SetActive(true);
}

void Tower::Update(Dragon &player, const float &dt)
{
	if (this->IsActive() == true)
	{
		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();

		if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt))
		{
			if (player.PosX > this->PosX)
			{
				player.PosX = PosX + (Sprite_.Get_Width() - 200.0f) + 70.0f;
			}
			else if (player.PosX < this->PosX)
			{
				player.PosX = PosX - (Sprite_.Get_Width() - 200.0f) - 70.0f;
			}
		}

		/*for (char i = 0; i < Bullet_Buffer; ++i)
			if (player.GetMfireball()[i].IsActive())*/

			if (Collision_.Dy_Rect_Rect(player.GetMfireball().Collision_, this->GetVelocity(), player.GetMfireball().GetVelocity(), dt))
			{
				//Decrease_HP(player.GetDamage());
				SetActive(false);
				player.GetMfireball().Projectile::ResetDist();
				player.GetMfireball().SetActive(false);
			}

			for (char i = 0; i < Bullet_Buffer; ++i)
				if (player.GetFireball()[i].IsActive())
					if (Collision_.Dy_Rect_Rect(player.GetFireball()[i].Collision_, this->GetVelocity(), player.GetFireball()[i].GetVelocity(), dt))
					{
						player.GetFireball()[i].Projectile::ResetDist();
						player.GetFireball()[i].SetActive(false);
					}

	}
}