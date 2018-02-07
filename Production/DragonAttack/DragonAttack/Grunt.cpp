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
	float MovementX = 0.5f;
	float MovementY = 0.5f;
	float CurrXPos;
	float CurrYPos;
}

Grunt::Grunt()
	: Characters(S_CreateSquare(100.0f, 1.0f, 1.0f, ".//Textures/grunt.png"),
		grunt_hp, { 0.0f, 0.0f, 5.0f, 5.0f, Rect })
{
	SetActive(true);
	PosX = 400.0f;
	PosY = 100.0f;
	CurrXPos = this->PosX;
	CurrYPos = this->PosY;
	//Transform_.SetTranslate(400.0f, 0.0f);
	//Transform_.Concat();
}

void Grunt::Update(float dt, const Dragon &d)
{
	UNREFERENCED_PARAMETER(dt);
	//update the behaviour of the grunt
	//check for vision range here
	LineOfSight(d);
	//check for attack range here

	//PlayerSeen is a bool that returns true or false based on the vision spheres/squares 
	//between enemies and the player
	if (PlayerSeen == true && PlayerInRange == false)
	{
		//Once player has been seen, use pathfinding to move toward player.
		MoveTowardPlayer(d);
	}
	else
	//PlayerInRange is a bool that returns true or false based on line of sight
	//of the enemies and the player
	if (PlayerInRange == true)
	{
		//AttackPlayer will run the attack animation/knock back the player/damage the player.
		AttackPlayer(d);
	}
	else // If neither of the above conditions are true, the enemy will be in its idle state(fn).
	{
		Idle();
	}
}

void Grunt::Pos()
{
}

void Grunt::MoveTowardPlayer(const Dragon &d)
{
	if (!LineOfSight(d))
	{
		PlayerSeen = false;
	}
	else
	{
		int findingPlayer = 1;

		while (findingPlayer--)
		{
			/*
			s32 mx , my;
			AEInputGetCursorPosition(&mx, &my);
			mx -= 640; my -= 640;
			*/
		
			MovementX = (this->PosX) - d.PosX;
			MovementY = (this->PosY) - d.PosY;

			if (MovementX < 100.0f && MovementY < 50.0f &&
				MovementX > -100.0f && MovementY > -50.0f) // Attack Range (change accordingly)
			{
				PlayerInRange = true;
			}
			else
			{
				PlayerInRange = false;
			}
		
			PosX -= MovementX / 60;
			PosY -= MovementY / 60;
		
			this->Transform_.SetTranslate(PosX, PosY);
			this->Transform_.Concat();
		}
	}
	
}

bool Grunt::LineOfSight(const Dragon &d)
{
	float playerDist = (d.PosX - this->PosX);
	if (playerDist <= 20.0f) // vision range is 20 units
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Grunt::AttackPlayer(const Dragon &d)
{
	MovementX = (this->PosX) - d.PosX;
	MovementY = (this->PosY) - d.PosY;

	if (MovementX > 100.0f && MovementY > 50.0f &&
		MovementX < -100.0f && MovementY < -50.0f)
	{
		PlayerInRange = false;
	}
	
	
	//attack animation
	std::cout << "Attacking" << std::endl;
}

void Grunt::Idle()
{
	float MaxXPos = CurrXPos + 100.0f; // max boundary
	float MinXPos = CurrXPos - 100.0f; // min boundary
	float moveSpd = 4.0f;
	int patrol = 1;

	while (patrol--)
	{
		std::cout << PosX << std::endl;
		if (PosX >= MaxXPos || PosX <= MinXPos)
		{
			std::cout << "bump" << std::endl<<std::endl;
			moveSpd *= -1;
		}

		PosX += moveSpd;
		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();
	}
}