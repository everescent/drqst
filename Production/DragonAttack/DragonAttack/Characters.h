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
#include <utility>
#include "GameObject.h"
#include "Transform.h"
#include "Create_Object.h"

enum Direction
{
	LEFT,
	RIGHT,
	TOP,
	DOWN
};

// A class that creates a basic character
class Characters : public GameObject
{
	private:
		int hp;      // hp the character have
		boolean moving;  // whether its moving or not
		Direction direction;   // moving in left or right, up or down?

	public:
		Characters(Sprite&& t_sprite, const int HP, Col_Comp &&t_col); // default constructor

		int Get_HP() const;         //get HP of mob
		void Set_HP(int hp); //set the hp for mob

		boolean Is_Moving() const;     // is character moving
		void Set_Moving(boolean moving); // make a character stop/move

		Direction Get_Direction() const; // get the direction of mob
		void Set_Direction(Direction direction);// set the direction of mob

		void virtual Update(float dt) = 0;    //  might not be needed for character class
		bool collision();
};
