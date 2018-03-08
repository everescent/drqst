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


void Floor::Update(Characters &obj, const float &dt)
{
	//(Jacob) Changed to PosX and PosY
	if (Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(), obj.GetVelocity(), dt))
	{
		

		if ( (obj.PosY - obj.Sprite_.Get_Height() )  > this->PosY)
		{
			//COLLISION_TOP = true ; 
			obj.PosY = PosY + this->Sprite_.Get_Height() + obj.Sprite_.Get_Height();
			//std::cout << "top " << std::endl;
		}
		 	
		else if ((obj.PosY + obj.Sprite_.Get_Height())  < (this->PosY) - (this->Sprite_.Get_Height()))
		{
			
			obj.PosY = PosY - this->Sprite_.Get_Height() - obj.Sprite_.Get_Height();
			//std::cout << "down" << std::endl;
		}

		else {
			if (obj.PosX > ((this->PosX - this->Sprite_.Get_Width())))
			{
				
				//if the dragon is on the right of platform
				obj.PosX = PosX + Sprite_.Get_Width() + obj.Sprite_.Get_Width();
				//std::cout << "right" << std::endl;
			}

			if (obj.PosX < ((this->PosX - this->Sprite_.Get_Width())))//obj.Get_Direction() == RIGHT)
			{
				
				//if the dragon is on the left of platform
				obj.PosX = PosX - Sprite_.Get_Width() - obj.Sprite_.Get_Width();
				//std::cout << "left" << std::endl;
			}
		}
		
	}

	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
}