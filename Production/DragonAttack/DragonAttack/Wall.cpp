#include "Wall.h"

Wall::Wall(float x, float y)
	: GameObject{ S_CreateRectangle(50.0f, 200.0f, ".//Textures/download.jpg"),
	Col_Comp{ x - 50.0f, y - 200.0f,
			  x + 50.0f, y + 200.0f, Rect },
			  x, y }
{
	SetActive(true);
}

void Wall::Update(Dragon &player)
{
	if (player.PosX < AEGfxGetWinMinX())
	{
		player.PosX = PosX + Sprite_.Get_Width() - 50.0f;
	}

	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
}