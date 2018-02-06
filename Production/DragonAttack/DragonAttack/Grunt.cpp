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
#include "Physics.h"
#include <iostream>

namespace // global variables just for THIS file
{
	const int grunt_hp = 30;
	bool PlayerSeen = true;
	bool PlayerInRange = false;
	bool Dir_left = true;
	bool Dir_right = false;
	float Movement = ApplyMovement(0.0016f);
	static float x = 0;
	static float y = 0;
}

/*
Grunt::Grunt(Sprite&& t_sprite)
	: Characters(std::move(t_sprite), grunt_hp, std::move(t_col))
{
	//float scaleX, scaleY;
	//this->Transform_.SetTranslate(10.0f, 10.0f);
	//(void)SetScale(scaleX, scaleY);
}*/

Grunt::Grunt()
	: Characters(S_CreateSquare(100.0f, 1.0f, 1.0f, ".//Textures/grunt.png"),
		grunt_hp, { 0.0f, 0.0f, 5.0f, 5.0f, Rect })
{
	SetActive(true);
	Transform_.SetTranslate(0.0f, 200.0f);
	//Transform_.Concat();

}
/*
Grunt* Create_Grunt(void)
{
	Grunt *grunt = new Grunt{  };

	return grunt;
}

void Delete_Grunt(Grunt *grunt)
{
	delete grunt;
}
*/
void Grunt::Update(float dt, const Dragon &d)
{
	UNREFERENCED_PARAMETER(dt);
	//update the behaviour of the grunt
	//check for vision range here
	LineOfSight(d);
	//check for attack range here

	//PlayerSeen is a bool that returns true or false based on the vision spheres/squares 
	//between enemies and the player
	if (PlayerSeen == true)
	{
		//Once player has been seen, use pathfinding to move toward player.
		MoveTowardPlayer(d);
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

void Grunt::Pos()
{
	//grunt->Transform_.SetTranslate(0,0);
}

void Grunt::MoveTowardPlayer(const Dragon &d)
{
	UNREFERENCED_PARAMETER(d);
	//Pathfinding here
	//float PlayerCoordX = 100.0f;
	//float PlayerCoordY = 100.0f;

	int findingPlayer = 1;

	while (findingPlayer--)
	{
		s32 mx;
		s32 my;
		AEInputGetCursorPosition(&mx, &my);
		mx -= 640;
		my -= 640;
		std::cout << "mouse x " << mx << std::endl;
		/*std::cout << "mouse y " << my << std::endl;
		//Changing player position
		if (AEInputCheckCurr(AEVK_X))
		{
			std::cout << "Player Coords changed!" << std::endl;
			PlayerCoordX *= -1;
		}*/

		if ((this->Transform_.GetTranslateMatrix().m[0][2]) > mx)
		{
			Movement = (this->Transform_.GetTranslateMatrix().m[0][2]) - mx;
		}
		else
		if ((this->Transform_.GetTranslateMatrix().m[0][2]) < mx)
		{
			Movement = (this->Transform_.GetTranslateMatrix().m[0][2]) - mx;
		}

		if ((this->Transform_.GetTranslateMatrix().m[0][2]) == mx)
		{
			std::cout << "Player found!" << std::endl;
		}

		x -= Movement/60;
		
		this->Transform_.SetTranslate(x, y);
		this->Transform_.Concat();
		std::cout << "grunt x " << this->PosX << std::endl;
	}
}

bool Grunt::LineOfSight(const Dragon &d)
{
	//for (int i = 0;i<NbrObstacles;i++) 
	//{
		//Check linexObstacle
		/*(if your using rectangle's look up linexRectangle, for all simple shapes, 
		their's simple formula, for complex shapes, you simple loop though all the edges, 
		and check linexPlane(in 2D, it's basically linexline collision).)*/
	float playerDist = (d.PosX - this->PosX);
	if (playerDist > 20.0f || playerDist < 20.0f)
	{
		return true;
	}
	else
		return false;
		/*(if we just care if the other tank can't see the player, 
		you could return false for a line of sight function here.*/
	//}
}

void Grunt::AttackPlayer()
{
	UNREFERENCED_PARAMETER(this);
	//attack animation
}

void Grunt::Idle()
{
	//int PatrolDist;
	float MaxXPos = 120.0f;
	float MinXPos = -120.0f;
	int patrol = 1;

	//std::cout << "CurrentX position is " << grunt->Transform_.GetTranslateMatrix().m[0][2] << std::endl;

	while (patrol--)
	{
		if ((this->Transform_.GetTranslateMatrix().m[0][2]) >= MaxXPos ||
			(this->Transform_.GetTranslateMatrix().m[0][2]) <= MinXPos)
		{
			Movement *= -1;
		}

		x += Movement;
		this->Transform_.SetTranslate(x, y);
		this->Transform_.Concat();
	}
}