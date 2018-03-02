#include "Floor.h"

Floor::Floor(float x, float y)
	: GameObject{ CreateFloor(2.0f, "../../Illustrations/Environment/Cobblestone.png", 1.0f, 6.0f),
	  Col_Comp{ x - FLOOR_WIDTH*2, y - FLOOR_HEIGHT, 
                x + FLOOR_WIDTH*2, y + FLOOR_HEIGHT, Rect }, 
                x, y }
{
	SetActive(true);
}

void Floor::Update(Dragon &player, const float &dt)
{
	//(Jacob) Changed to PosX and PosY
	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();

  //Checks for collision, and have player stand on platform if true
	//if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt) && player.PosX > Sprite_.Get_Width())//collison from right
	//{
	//	player.PosX = PosX + Sprite_.Get_Width() + 70;
	//}
	//player.GetFacing() == ;
	//if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt) && player.PosX < Sprite_.Get_Width())
	//{
	//	player.PosX = PosX + Sprite_.Get_Width() - 70;
	//}

  
  if (Collision_.Dy_Rect_Rect(player.Collision_, GetVelocity(), player.GetVelocity(), dt))
  {
    player.PosY = PosY + Sprite_.Get_Height() + 70;
  }
}