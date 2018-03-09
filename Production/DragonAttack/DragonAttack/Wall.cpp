#include "Wall.h"
#include <iostream>

Wall::Wall(float x, float y)
	: GameObject{ S_CreateRectangle(50.0f, 50.0f, ".//Textures/download.jpg"),
	Col_Comp{ x - 50.0f, y - 50.0f,
	x + 50.0f, y + 50.0f, Rect },
	x, y }
{
	SetActive(true);
	Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
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
			obj.PosX = PosX + Sprite_.Get_Width() + obj.Sprite_.Get_Width();
		}
		else if (obj.PosX < this->PosX)
		{
			obj.PosX = PosX - Sprite_.Get_Width() - obj.Sprite_.Get_Width();
		}
	}

}