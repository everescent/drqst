#include "Wall.h"
#include <iostream>
Wall::Wall(float x, float y)
	: GameObject{ S_CreateRectangle(50.0f, 200.0f, ".//Textures/download.jpg"),
	Col_Comp{ x - 50.0f, y - 200.0f,
			  x + 50.0f, y + 200.0f, Rect },
			  x, y }
{
	SetActive(true);
}

void Wall::Update(Dragon &player, const float &dt)
{
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();

	if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt))
	{
		if (player.PosX > this->PosX)
		{
			player.PosX = PosX + Sprite_.Get_Width() + 70.0f;
		}
		else if (player.PosX < this->PosX)
		{
			player.PosX = PosX - Sprite_.Get_Width() - 70.0f;
		}
	}

}