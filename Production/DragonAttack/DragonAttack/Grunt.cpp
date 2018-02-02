/* Start Header ************************************************************************/
/*!
\file       Grunt.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Grunts to be created for the game.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Grunt.h"

namespace // global variables just for THIS file
{
	const int grunt_hp = 30; // Health here

	Col_Comp t_col{ 0.0f, 0.0f, 5.0f, 5.0f, Rect };   // set the shape for grunt here

	bool PlayerSeen = false;

	bool PlayerInRange = false;
}


Grunt::Grunt(Sprite&& t_sprite)
	: Characters(std::move(t_sprite), grunt_hp, std::move(t_col))
{
	//float scaleX, scaleY;

	//(void)SetScale(scaleX, scaleY);
}

//Grunt::Grunt(Sprite&& t_sprite, const float x, const float y)
//	: Characters(std::move(t_sprite))
//{
//	//float scaleX, scaleY;
//
//	//(void)SetScale(scaleX, scaleY);
//	//translate the position of the grunt as well.
//}


Grunt *Create_Grunt()
{
	const char* tex_Grunt = ".//Textures/download.jpg";

	Grunt *grunt = new Grunt{ S_CreateSquare(100.0f, 1.0f, 1.0f, tex_Grunt) };

	return  grunt;
}

void Delete_Grunt(Grunt *grunt)
{
	delete grunt;
}

void Grunt::Update()
{
	//check for vision range here
	//check for attack range here

	//PlayerSeen is a bool that returns true or false based on the vision spheres/squares 
	//between enemies and the player
	if (PlayerSeen == true) 
	{
		//Once player has been seen, use pathfinding to move toward player.
		MoveTowardPlayer();
	}
	else 
	//PlayerInRange is a bool that returns true or false based on the attack spheres/squares
	//of the enemies and the player
	if (PlayerInRange == true) 
	{
		//AttackPlayer will run the attack animation/knock back the player/damage the player.
		AttackPlayer();
	}
	else // If neither of the above conditions are true, the enemy will be in its idle state(fn).
	{
		Idle();
	}
}

void Grunt::Pos(Grunt *grunt)
{
	//grunt->Transform_.SetTranslate(0, 0);
}

void MoveTowardPlayer()
{
	//Pathfinding algorithm here
}

void AttackPlayer()
{
	//Attack animation/knock back/damage player here
}

void Idle() 
{
	//Patrol or standstill
}