#include "Floor.h"

Floor::Floor()
	: GameObject{ CreateFloor(1.0f, 1.0f, 1.0f, ".//Textures/download.jpg"),
	  Col_Comp{ 0.0f - FLOOR_WIDTH, -300.0f - FLOOR_HEIGHT, 
              0.0f + FLOOR_WIDTH, -300.0f + FLOOR_HEIGHT, Rect }, 
              0.0f, -300.0f }
{
	SetActive(true);
}

void Floor::Update(Dragon &player, const float &dt)
{
  //Checks for collision, and have player stand on platform if true
  if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt))
  {
    player.PosY = PosY + Sprite_.Get_Height() + 100;
  }
  //(Jacob) Changed to PosX and PosY
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
}
