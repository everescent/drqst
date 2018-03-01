#include "Barrier.h"

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
	UNREFERENCED_PARAMETER(player);
	UNREFERENCED_PARAMETER(dt);
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
}