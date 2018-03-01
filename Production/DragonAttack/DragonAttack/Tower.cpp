#include "Tower.h"

Tower::Tower(float x, float y)
	: GameObject{ S_CreateRectangle(300.0f, 300.0f, ".//Textures/tower.png"),
	Col_Comp{ x - 200.0f, y - 300.0f,
	x + 200.0f, y + 300.0f, Rect },
	x, y }
{
	SetActive(true);
}

void Tower::Update(Dragon &player, const float &dt)
{
	UNREFERENCED_PARAMETER(player);
	UNREFERENCED_PARAMETER(dt);
	/*if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt))
	{
		player.PosX = PosX + Sprite_.Get_Width() + 70.0f;
	}*/

	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
}