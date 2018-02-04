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
#include "Transform.h"

namespace // global variables just for THIS file
{
	const int grunt_hp = 30;
	Col_Comp t_col{ 0.0f, 0.0f, 5.0f, 5.0f, Rect };   // set the shape for grunt here
	bool PlayerSeen = false;
	bool PlayerInRange = false;

}


Grunt::Grunt(Sprite&& t_sprite)
	: Characters(std::move(t_sprite), grunt_hp, std::move(t_col))
{
	//float scaleX, scaleY;
	//this->Transform_.SetTranslate(10.0f, 10.0f);
	//(void)SetScale(scaleX, scaleY);
}

Grunt* Create_Grunt(void)
{
	const char* tex_Grunt = ".//Textures/grunt.png";

	Grunt *grunt = new Grunt{ S_CreateSquare(100.0f, 1.0f, 1.0f, tex_Grunt) };

	return grunt;
}

void Delete_Grunt(Grunt *grunt)
{
	delete grunt;
}

void Grunt::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void Grunt::G_Update(Grunt *grunt)
{
	//unref for now, will use for the movement funcs
	//UNREFERENCED_PARAMETER(dt);

	//update the behaviour of the grunt
	//check for vision range here
	//check for attack range here
		
	//PlayerSeen is a bool that returns true or false based on the vision spheres/squares 
	//between enemies and the player
	if (PlayerSeen == true)
	{
		//Once player has been seen, use pathfinding to move toward player.
		MoveTowardPlayer(grunt);
	}
	else
	//PlayerInRange is a bool that returns true or false based on the attack spheres/squares
	//of the enemies and the player
	if (PlayerInRange == true)
	{
		//AttackPlayer will run the attack animation/knock back the player/damage the player.
		AttackPlayer(grunt);
	}
	else // If neither of the above conditions are true, the enemy will be in its idle state(fn).
	{
		Idle(grunt);
	}
}

void Grunt::Pos(Grunt *grunt)
{
	grunt->Transform_.SetTranslate(0,0);
}

void MoveTowardPlayer(Grunt *grunt)
{
	UNREFERENCED_PARAMETER(grunt);
	//Pathfinding here
}

void AttackPlayer(Grunt *grunt)
{
	UNREFERENCED_PARAMETER(grunt);
	//attack animation
}

void Idle(Grunt *grunt)
{
	static float x = 0;
	static float y = 0;

	//Patrol
	int PatrolDist;
	bool left = true;
	bool right = false;

	//move left
	if (left == true) 
	{
		for (PatrolDist = 5; PatrolDist>0; --PatrolDist)
		{
			x -= 3;
			grunt->Transform_.SetTranslate(x, y);
		}
		left = false;
		right = true;
	}
	
	//move right
	if (right == true)
	{
		for (PatrolDist = 5; PatrolDist>0; --PatrolDist)
		{
			x += 3;
			grunt->Transform_.SetTranslate(x, y);
		}
		left = true;
		right = false;
	}
}