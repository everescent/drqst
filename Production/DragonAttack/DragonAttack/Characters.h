/* Start Header ************************************************************************/
/*!
\file       Characters.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Character class, parent class of the characters to be created

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "Objects.h"
#include "Transform.h"

// A class that creates a basic character
class Characters : public virtual Object, public virtual Transform
{
	private:
		boolean removed; //determines if its still on the map
		int hp;      // damage it should deal
		boolean moving;  // whether its moving or not
		int direction;   // moving in left or right

	public:
		Characters(); // default constructor

		boolean Is_Removed() const;     //get removed
		void Set_Removed(boolean dead); //set the ai to die

		int Get_HP() const;         //get HP of mob
		void Set_HP(int hp); //set the hp for mob

		boolean Is_Moving() const;     // is character moving
		void Set_Moving(boolean moving); // make a character stop/move

		void Render();               //unsure, placing it incase i need it
		void move();          
		void update();
		boolean collision();
};