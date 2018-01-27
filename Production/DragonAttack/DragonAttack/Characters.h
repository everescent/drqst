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

// A class that creates a basic character
class Characters : public virtual Object
{
	private:
		boolean removed; //determines if its still on the map
		int damage;      // damage it should deal
		boolean moving;  // whether its moving or not
		int direction;   // moving in left or right

	public:
		Characters();
		boolean IsRemoved() const;     //get removed
		void SetRemoved(boolean dead); //set the ai to die

		int GetDamage() const;         //get damage for mob
		void SetDamage(int damage_for_mob); //set the damage for mob

		boolean IsMoving() const;     // is character moving
		void SetMoving(boolean moving); // make a character stop/move

		void Render();               //unsure, placing it incase i need it
		void move();          
		void update();
		boolean collision();
};