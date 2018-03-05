#include "Floor.h"
#include <iostream>

Floor::Floor(float x, float y)
	: GameObject{ CreateFloor(1.0f, "../../Illustrations/Environment/Cobblestone.png", 1.0f, 1.0f),
	Col_Comp{ x - FLOOR_WIDTH, y - FLOOR_HEIGHT,
	x + FLOOR_WIDTH, y + FLOOR_HEIGHT, Rect },
	x, y }
{
	SetActive(true);
	PosX = x;
	PosY = y;
}

/*void Floor::Update(Dragon &player, const float &dt)
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
}*/

void Floor::Update(Characters &obj, const float &dt)
{
	//(Jacob) Changed to PosX and PosY
	if (Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(), obj.GetVelocity(), dt))
	{
		if (obj.PosY > this->PosY)
		{
			obj.PosY = PosY + Sprite_.Get_Height() + obj.Sprite_.Get_Height();
		}
		/*else
		{
		if (obj.PosX > this->PosX)
		{
		obj.PosX = PosX + Sprite_.Get_Width() + obj.Sprite_.Get_Width();
		std::cout << "right" << std::endl;
		}
		if (obj.PosX < this->PosX)
		{
		obj.PosX = PosX - Sprite_.Get_Width() - obj.Sprite_.Get_Width();
		std::cout << "left" << std::endl;
		}
		}*/
	}

	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
}