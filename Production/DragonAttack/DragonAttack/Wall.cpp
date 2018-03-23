#include "Wall.h"
#include <iostream>

Wall::Wall(Sprite* const p_Sprite, float x, float y)
	: GameObject{ p_Sprite,
	Col_Comp{ x - 40.0f, y - 45.0f,
	x + 40.0f, y + 45.0f, Rect },
	x, y }
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = x;
	PosY = y;
}

void Wall::Update(Characters &obj, const float &dt)
{
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();

	if (Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(), obj.GetVelocity(), dt))
	{
		if (obj.PosX > this->PosX)
		{
			obj.PosX = PosX + Sprite_->Get_Width() + obj.Sprite_->Get_Width() * 0.5f;
		}
		else if (obj.PosX < this->PosX)
		{
			obj.PosX = PosX - Sprite_->Get_Width() - obj.Sprite_->Get_Width()* 0.5f;
		}
	}

}