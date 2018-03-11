#include "Tower.h"

namespace
{
	Sprite TOWER_SPRITE = S_CreateRectangle(300.0f, 300.0f, ".//Textures/tower.png");
}

Tower::Tower(float x, float y)
	: GameObject{ &TOWER_SPRITE,
	Col_Comp{ x - 100.0f, y - 300.0f,
			  x + 100.0f, y + 190.0f, Rect },
			  x, y }
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
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
				player.PosX = PosX + (Sprite_->Get_Width() - 200.0f) + player.Sprite_->Get_Width();
			}
			else if (player.PosX < this->PosX)
			{
				player.PosX = PosX - (Sprite_->Get_Width() - 200.0f) - player.Sprite_->Get_Width();
			}
		}

		if (Collision_.Dy_Rect_Rect(player.GetMfireball().Collision_, this->GetVelocity(), player.GetMfireball().GetVelocity(), dt))
		{
			//Decrease_HP(player.GetDamage());
			SetActive(false);
			player.GetMfireball().Projectile::ResetDist();
			player.GetMfireball().SetActive(false);
		}

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

void Tower::Update(Characters &obj, const float &dt)
{
	if (this->IsActive() == true)
	{
		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();

		if (Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(), obj.GetVelocity(), dt))
		{
			if (obj.PosY > this->PosY)
			{
				obj.PosY = PosY + (Sprite_->Get_Height() - 110.0f) + obj.Sprite_->Get_Height();
			}
		}

	}
}