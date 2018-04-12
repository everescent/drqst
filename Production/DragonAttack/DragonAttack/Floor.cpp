/* Start Header ************************************************************************/
/*!
\file	 Floor.cpp
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Floor class body definitions.
	
All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Floor.h"

// Floor constructor
Floor::Floor(Sprite* const p_Sprite, float x, float y)
	: GameObject{ p_Sprite,
	Col_Comp{ x - FLOOR_WIDTH, y - FLOOR_HEIGHT,
	x + FLOOR_WIDTH, y + FLOOR_HEIGHT, Rect },
	x, y } // Set collision positions
{
	SetActive(true);
	PosX = x;
	PosY = y;
}

// Update function for Floor
void Floor::Update(Dragon &obj, const float &dt)
{
	if (Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(), obj.GetVelocity(), dt))
	{
    obj.SetTouchBottom(true);
		if ( (obj.PosY - obj.Sprite_->Get_Height() )  > this->PosY)
		{
			obj.PosY = PosY + Sprite_->Get_Height() + obj.Sprite_->Get_Height();
		}
		 	
		else if ((obj.PosY + obj.Sprite_->Get_Height())  < (this->PosY) )
		{			
			obj.PosY = PosY - Sprite_->Get_Height() - obj.Sprite_->Get_Height();
		}

		else {
			if (obj.PosX > ((PosX - Sprite_->Get_Width())))
			{				
				//if the dragon is on the right of platform
				obj.PosX = PosX + Sprite_->Get_Width() + obj.Sprite_->Get_Width() ;
			}

			if (obj.PosX < ((PosX - Sprite_->Get_Width())))
			{				
				//if the dragon is on the left of platform
				obj.PosX = PosX - Sprite_->Get_Width() - obj.Sprite_->Get_Width();
			}
		}
	}

	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
}

void Floor::Update(Characters &obj, const float &dt)
{
	//need to convert to dt based
	if (Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(), obj.GetVelocity(), dt))
	{
		if ((obj.PosY - obj.Sprite_->Get_Height())  > PosY)
		{
			obj.PosY = PosY + Sprite_->Get_Height() + obj.Sprite_->Get_Height();
		}
		else {
			if (obj.PosX > ((PosX - Sprite_->Get_Width())))
			{
				//if the dragon is on the right of platform
				obj.PosX = PosX + Sprite_->Get_Width() + obj.Sprite_->Get_Width();
			}

			if (obj.PosX < ((PosX - Sprite_->Get_Width())))
			{
				//if the dragon is on the left of platform
				obj.PosX = PosX - Sprite_->Get_Width() - obj.Sprite_->Get_Width();
			}
		}
	}

	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
}