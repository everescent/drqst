#include "Wall2.h"

Wall2::Wall2(float x, float y)
	: GameObject{ S_CreateRectangle(50.0f, 200.0f, ".//Textures/download.jpg"),
	Col_Comp{ x - 50.0f, y - 200.0f,
	x + 50.0f, y + 200.0f, Rect },
	x, y }
{
	SetActive(true);
}

void Wall2::Update(Dragon &player, const float &dt)
{
	if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt))
	{
		player.PosX = PosX - Sprite_.Get_Width() - 70.0f;
	}

	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
}