/* Start Header ************************************************************************/
/*!
\file    Block.cpp
\project Dragon Attack
\author  Javon Lee
\email   xiongweijavon.lee@digipen.edu
\brief
Block class functions are defined here.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#define USE_BLOCK_MACRO
#include "Block.h"
#include <iostream>

Block::Block(Sprite* const p_Sprite, float x, float y)
	: 
	Top_HS1{ x - Block_Macro::half_block_width /2, y + Block_Macro::half_block_height },
	Top_HS2{ x + Block_Macro::half_block_width /2, y + Block_Macro::half_block_height },
	Bottom_HS1{ x - Block_Macro::half_block_width / 2, y - Block_Macro::half_block_height },
	Bottom_HS2{ x + Block_Macro::half_block_width / 2, y - Block_Macro::half_block_height },
	Left_HS1{ x - Block_Macro::half_block_width, y + Block_Macro::half_block_height/2 },
	Left_HS2{ x - Block_Macro::half_block_width, y - Block_Macro::half_block_height / 2 },
	Right_HS1{ x + Block_Macro::half_block_width, y + Block_Macro::half_block_height / 2 },
	Right_HS2{ x + Block_Macro::half_block_width, y - Block_Macro::half_block_height / 2 },
	GameObject {
	p_Sprite,
	Col_Comp{ x - Block_Macro::half_block_width, y - Block_Macro::half_block_height,
	x + Block_Macro::half_block_width, y + Block_Macro::half_block_height, Rect },
	x, y }
{
	SetActive(true);
	PosX = x;
	PosY = y;
}


void Block::Update(Dragon& obj, const float &dt)
{
	if (Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(), obj.GetVelocity(), dt))
	{
		
		// Use the hotspots to check for collision to determine the direction the dragon is coming from. 
		//If dragon collided from the TOP 
		if ((obj.Collision_.St_Rect_Point(Top_HS1.x, Top_HS1.y)) || (obj.Collision_.St_Rect_Point(Top_HS2.x, Top_HS2.y)))
		{
			//std::cout << "THERE IS top COLLISION" << std::endl;
			obj.SetTouchBottom(true); // Dragon touched the top of the block platform
			//snap back the Dragon up to the top of the block
			//obj.SetPos(obj.PosX, (PosY + half_block_height));
			obj.PosY = PosY + Block_Macro::half_block_height + obj.Sprite_->Get_Height();
		}

		//If dragon collided from the bottom 
		else if ((obj.Collision_.St_Rect_Point(Bottom_HS1.x, Bottom_HS1.y)) || (obj.Collision_.St_Rect_Point(Bottom_HS2.x, Bottom_HS2.y)))
		{
			//std::cout << "THERE IS btm COLLISION" << std::endl;
			//snap back the Dragon up to the bottom of the block
			//obj.SetPos(obj.PosX, (PosY - half_block_height));
			obj.PosY = PosY - Block_Macro::half_block_height - obj.Sprite_->Get_Height();
		}

		//If dragon collided from the left 
		else if ((obj.Collision_.St_Rect_Point(Left_HS1.x, Left_HS1.y)) || (obj.Collision_.St_Rect_Point(Left_HS2.x, Left_HS2.y)))
		{
			//std::cout << "THERE IS left COLLISION" << std::endl;
			//snap back the Dragon up to the bottom of the block
			//obj.SetPos((PosX - half_block_width), obj.PosY);
			obj.PosX = PosX - Sprite_->Get_Width() - obj.Sprite_->Get_Width();
			
		}

		//If dragon collided from the right  
		else if ((obj.Collision_.St_Rect_Point(Right_HS1.x, Right_HS1.y)) || (obj.Collision_.St_Rect_Point(Right_HS2.x, Right_HS2.y)))
		{
			//std::cout << "THERE IS right COLLISION" << std::endl;
			//snap back the Dragon up to the bottom of the block
			//obj.SetPos((PosX + half_block_width), obj.PosY);
			obj.PosX = PosX + Sprite_->Get_Width() + obj.Sprite_->Get_Width();
		}
	}

		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();
}

void Block::Update(Characters& obj, const float &dt)
{
	if (Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(), obj.GetVelocity(), dt))
	{

		// Use the hotspots to check for collision to determine the direction the dragon is coming from. 
		//If dragon collided from the TOP 
		if ((obj.Collision_.St_Rect_Point(Top_HS1.x, Top_HS1.y)) || (obj.Collision_.St_Rect_Point(Top_HS2.x, Top_HS2.y)))
		{
			//std::cout << "THERE IS top COLLISION" << std::endl;
			//obj.SetTouchBottom(true); // Dragon touched the top of the block platform
									  //snap back the Dragon up to the top of the block
									  //obj.SetPos(obj.PosX, (PosY + half_block_height));
			obj.PosY = PosY + Block_Macro::half_block_height + obj.Sprite_->Get_Height();
		}

		//If dragon collided from the bottom 
		else if ((obj.Collision_.St_Rect_Point(Bottom_HS1.x, Bottom_HS1.y)) || (obj.Collision_.St_Rect_Point(Bottom_HS2.x, Bottom_HS2.y)))
		{
			//std::cout << "THERE IS btm COLLISION" << std::endl;
			//snap back the Dragon up to the bottom of the block
			//obj.SetPos(obj.PosX, (PosY - half_block_height));
			obj.PosY = PosY - Block_Macro::half_block_height - obj.Sprite_->Get_Height();
		}

		//If dragon collided from the left 
		else if ((obj.Collision_.St_Rect_Point(Left_HS1.x, Left_HS1.y)) || (obj.Collision_.St_Rect_Point(Left_HS2.x, Left_HS2.y)))
		{
			//std::cout << "THERE IS left COLLISION" << std::endl;
			//snap back the Dragon up to the bottom of the block
			//obj.SetPos((PosX - half_block_width), obj.PosY);
			obj.PosX = PosX - Sprite_->Get_Width() - obj.Sprite_->Get_Width();

		}

		//If dragon collided from the right  
		else if ((obj.Collision_.St_Rect_Point(Right_HS1.x, Right_HS1.y)) || (obj.Collision_.St_Rect_Point(Right_HS2.x, Right_HS2.y)))
		{
			//std::cout << "THERE IS right COLLISION" << std::endl;
			//snap back the Dragon up to the bottom of the block
			//obj.SetPos((PosX + half_block_width), obj.PosY);
			obj.PosX = PosX + Sprite_->Get_Width() + obj.Sprite_->Get_Width();
		}
	}

	this->Transform_.SetTranslate(PosX, PosY);
	this->Transform_.Concat();
}
