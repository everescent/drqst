#include "Block.h"
#include <iostream>

Block::Block(Sprite* const p_Sprite, float x, float y)
	: half_block_width{ 40.0f }, half_block_height{45.0f},
	Top_HS1{ x - half_block_width /2, y + half_block_height },
	Top_HS2{ x + half_block_width /2, y + half_block_height },
	Bottom_HS1{ x - half_block_width / 2, y - half_block_height },
	Bottom_HS2{ x + half_block_width / 2, y - half_block_height },
	Left_HS1{ x - half_block_width, y + half_block_height/2 },
	Left_HS2{ x - half_block_width, y - half_block_height / 2 },
	Right_HS1{ x + half_block_width, y + half_block_height / 2 },
	Right_HS2{ x + half_block_width, y - half_block_height / 2 },
	GameObject {
	p_Sprite,
	Col_Comp{ x - half_block_width, y - half_block_height,
	x + half_block_width, y + half_block_height, Rect },
	x, y }
{
	SetActive(true);
	PosX = x;
	PosY = y;

	std::cout << "Position of BLOCK:" << PosX << "," << PosY << std::endl;
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
			obj.PosY = PosY + half_block_height;
		}

		//If dragon collided from the bottom 
		//if ((obj.Collision_.St_Rect_Point(Bottom_HS1.x, Bottom_HS1.y)) || (obj.Collision_.St_Rect_Point(Bottom_HS2.x, Bottom_HS2.y)))
		//{
		//	std::cout << "THERE IS btm COLLISION" << std::endl;
		//	//snap back the Dragon up to the bottom of the block
		//	//obj.SetPos(obj.PosX, (PosY - half_block_height));
		//	obj.PosY = PosY - half_block_height;
		//}

		//If dragon collided from the left 
		if ((obj.Collision_.St_Rect_Point(Left_HS1.x, Left_HS1.y)) || (obj.Collision_.St_Rect_Point(Left_HS2.x, Left_HS2.y)))
		{
			std::cout << "THERE IS left COLLISION" << std::endl;
			//snap back the Dragon up to the bottom of the block
			//obj.SetPos((PosX - half_block_width), obj.PosY);
			obj.PosX = PosX - Sprite_->Get_Width() - obj.Sprite_->Get_Width();
			
		}

		//If dragon collided from the right  
		if ((obj.Collision_.St_Rect_Point(Right_HS1.x, Right_HS1.y)) || (obj.Collision_.St_Rect_Point(Right_HS2.x, Right_HS2.y)))
		{
			std::cout << "THERE IS right COLLISION" << std::endl;
			//snap back the Dragon up to the bottom of the block
			//obj.SetPos((PosX + half_block_width), obj.PosY);
			obj.PosX = PosX + Sprite_->Get_Width() + obj.Sprite_->Get_Width();
		}
	}

		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();
}

//
//void Block::Update(Characters &obj, const float &dt)
//{
//	//need to convert to dt based
//	if (Collision_.Dy_Rect_Rect(obj.Collision_, GetVelocity(), obj.GetVelocity(), dt))
//	{
//		if ((obj.PosY - obj.Sprite_->Get_Height())  > PosY)
//		{
//			obj.PosY = PosY + Sprite_->Get_Height() + obj.Sprite_->Get_Height();
//			//std::cout << "top " << std::endl;
//		}
//
//		else {
//			if (obj.PosX > ((PosX - Sprite_->Get_Width())))
//			{
//				//if the dragon is on the right of platform
//				obj.PosX = PosX + Sprite_->Get_Width() + obj.Sprite_->Get_Width();
//				//std::cout << "right" << std::endl;
//			}
//
//			if (obj.PosX < ((PosX - Sprite_->Get_Width())))
//			{
//				//if the dragon is on the left of platform
//				obj.PosX = PosX - Sprite_->Get_Width() - obj.Sprite_->Get_Width();
//				//std::cout << "left" << std::endl;
//			}
//		}
//	}
//
//	this->Transform_.SetTranslate(PosX, PosY);
//	this->Transform_.Concat();
//}