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
// For Player
void Platform::Update(Dragon &player, const float &dt)
{
	bool Collide = Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(),	player.GetVelocity(), dt);
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();

	if (Collide && !player.GetJump())
	{
    player.SetTouchBottom(true);
		if (player.PosY - player.Sprite_->Get_Height() < PosY)
			return;
		if (player.PosY <= PosY + Sprite_->Get_Height() + player.Sprite_->Get_Height())
			player.PosY = PosY + Sprite_->Get_Height() + player.Sprite_->Get_Height();
	}
}

// For AI
void Platform::Update(Characters &obj, const float &dt)
{
	bool Collide = Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(),
		obj.GetVelocity(), dt);
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();

	if (Collide)
	{
		//obj.SetTouchBottom(true);
		if (obj.PosY - obj.Sprite_->Get_Height() < PosY)
			return;
		if (obj.PosY <= PosY + Sprite_->Get_Height() + obj.Sprite_->Get_Height())
			obj.PosY = PosY + Sprite_->Get_Height() +  obj.Sprite_->Get_Height();
	}
}
