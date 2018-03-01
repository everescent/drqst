/* Start Header ************************************************************************/
/*!
\file       Grunt.cpp
\author     Andrew Chong
\par email: c.jiahaoandrew\@digipen.edu
\brief
Grunts to be created for the game.

Copyright (C) 2018 DigiPen Institute of Technology.
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
	const int grunt_hp = 20;
	bool  PlayerSeen = false;
	bool  PlayerInRange = false;
	float MovementX = 0.5f;
	float MovementY = 0.5f;
	float moveSpd = 3.0f;
	int EstIdleX;
	const float GRUNT_SCALE = 70.0F;
}

Grunt::Grunt(float x, float y)
	: Characters(S_CreateSquare(GRUNT_SCALE, ".//Textures/grunt.png"),
	  grunt_hp, 
	  Col_Comp{ x - GRUNT_SCALE, y - GRUNT_SCALE , x + GRUNT_SCALE , y + GRUNT_SCALE, Rect })
{
	SetActive(true);
	PosX = x;
	PosY = y;/*
	IdleXPos = this->PosX;
	IdleYPos = this->PosY;
	EstIdleX = (int)IdleXPos;*/
}

void Grunt::Update(Dragon &d, const float dt)
{
	LineOfSight(d);

	if (PlayerSeen == true)
	{
		MoveTowardPlayer(d);

		if (PlayerInRange == true)
		{
			AttackPlayer(d);
		}
	}
	else
	{
		Idle(d);
	}



	// update the collision box of grunt
	this->Collision_.Update_Col_Pos(this->PosX - GRUNT_SCALE, this->PosY - GRUNT_SCALE,  // min point
									this->PosX + GRUNT_SCALE, this->PosY + GRUNT_SCALE);	// max point

	/*this->Transform_.SetTranslate(this->PosX, this->PosY);
	this->Transform_.Concat();		*/						



	for (char i = 0; i < Bullet_Buffer; ++i)
		if (d.GetFireball()[i].IsActive())
			if (Collision_.Dy_Rect_Rect(d.GetFireball()[i].Collision_, this->GetVelocity(), d.GetFireball()[i].GetVelocity(), dt))
			{
				Decrease_HP(d.GetDamage());
				//SetActive false here
				d.GetFireball()[i].Projectile::ResetDist();
				d.GetFireball()[i].SetActive(false);
			}
}

//void Grunt::Pos()
//{
//}

void Grunt::MoveTowardPlayer(const Dragon &d)
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
	if (MovementX > 100.0f || MovementX < -100.0f) // not entering
	{
		PlayerInRange = false;
	}

	PosX -= MovementX / 60;
	//PosY -= MovementY / 60;

	this->Transform_.SetTranslate(PosX, PosY); //PosX += GetVelocity
	this->Transform_.Concat();
}

void Grunt::LineOfSight(const Dragon &d)
{
	float playerDist = (d.PosX - this->PosX);
	if (playerDist <= 400.0f && playerDist >= -400.0f)  // vision range
	{
		PlayerSeen = true;
		std::cout << "seen" << std::endl;
	}
	else
	{
		PlayerSeen = false;
	}
}

void Grunt::AttackPlayer(const Dragon &d)
{
	UNREFERENCED_PARAMETER(d);
	//std::cout << "Attacking" << std::endl;
	//MovementX = (this->PosX) - d.PosX;
	//MovementY = (this->PosY) - d.PosY;
	//MovementX = d.PosX - (this->PosX);
	//MovementY = d.PosY - (this->PosY);
	//// if player moves out of range
	//// if (MovementX > 100.0f && MovementY > 50.0f ||
	////	   MovementX < -100.0f && MovementY < -50.0f)
	//if (MovementX > 100.0f || MovementX < -100.0f) // not entering
	//{
	//	std::cout << "out of range" << std::endl;
	//	PlayerInRange = false;
	//}
	//else
	//{
	//	PlayerInRange = true;
	//	//attack animation
	//	std::cout << "Attacking" << std::endl;
	//}
}

void Grunt::Idle(const Dragon &d)
{
	UNREFERENCED_PARAMETER(d);
	float IdleXPos = PosX;

	int MaxXPos = (int)(IdleXPos + 120.0f); // max boundary
	int MinXPos = (int)(IdleXPos - 120.0f); // min boundary
	int EstCurrentX = (int)(this->PosX);

	if (EstCurrentX < (EstIdleX - 121) || EstCurrentX >(EstIdleX + 120)) // if lesser than min boundary OR greater than max boundary
	{
		//reason for strange values is due to how the obj will enter the bottom condition if it is close to the boundary
		//then re-enter this condition because of the *= -1, forever continuing the back and forth

		MovementX = (this->PosX) - IdleXPos;
		PosX -= MovementX / 120;

		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();
	}
	else
	{
		if (PlayerSeen == false)
		{
			if ((PosX > MaxXPos) || (PosX < MinXPos))
			{
				moveSpd *= -1;
			}

			PosX += moveSpd;
			this->Transform_.SetTranslate(PosX, PosY);
			this->Transform_.Concat();
		}
	}
}
