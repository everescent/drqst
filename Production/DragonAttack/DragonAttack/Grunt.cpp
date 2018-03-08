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
	const float GRUNT_SCALE = 70.0F;
}

Audio_Engine Grunt::SFX{ 1, [](std::vector<std::string> &playlist)
->void {	playlist.push_back(".//Audio/Hit_02.mp3");
} };

Grunt::Grunt(const float posX, const float posY)
	: Characters(S_CreateSquare(GRUNT_SCALE, ".//Textures/grunt.png"),
		grunt_hp,
		Col_Comp{ posX - GRUNT_SCALE, posY - GRUNT_SCALE ,
				  posX + GRUNT_SCALE - 60.0f , posY + GRUNT_SCALE - 100.0f, Rect }),
	PlayerSeen{ false },
	PlayerInRange{ false },
	MovementX{ 0.5f },
	MovementY{ 0.5f },
	moveSpd{ 3.0f },
	EstIdleX{ 0 }
{
	SetActive(true);
	Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = posX;
	PosY = posY;
}

void Grunt::Update(Dragon &d, const float dt)
{

	if (this->IsActive() == true)
	{
		LineOfSight(d);

		if (PlayerSeen == true)
		{
			MoveTowardPlayer(d, dt);

			if (PlayerInRange == true)
			{
				AttackPlayer(d);
			}
		}
		else
		{
			Idle(d);
		}

		this->Transform_.SetTranslate(this->PosX, this->PosY);
		this->Transform_.Concat();

		PosY -= 10.0f; //testing gravity
		//std::cout << PosX << " " << d.PosX<<std::endl;
		// update the collision box of grunt
		this->Collision_.Update_Col_Pos(this->PosX - GRUNT_SCALE, this->PosY - GRUNT_SCALE,  // min point
										this->PosX + GRUNT_SCALE - 60.0f, this->PosY + GRUNT_SCALE - 100.0f); // max point

		/*if (Collision_.Dy_Rect_Rect(d.Collision_, GetVelocity(), d.GetVelocity(), dt))
		{
		if (d.PosX > this->PosX)
		{
		d.PosX = PosX + Sprite_.Get_Width() + Sprite_.Get_Height();
		}
		else if (d.PosX < this->PosX)
		{
		d.PosX = PosX - Sprite_.Get_Width() - Sprite_.Get_Height();
		}
		}*/

		//collision with player, player loses health
		if(!Knockback)
			if (Collision_.Dy_Rect_Rect(d.Collision_, GetVelocity(), d.GetVelocity(), dt))
			{
				d.Decrease_HP();
				d.PlayHit();
				Knockback = true;
				posit_tmp = d.PosX;
				/*if (d.PosX > this->PosX)
				{
					d.PosX = PosX + 200.0f;
				}
				else if (d.PosX < this->PosX)
				{
					d.PosX = PosX - 200.0f;
				}*/
			}

		if (Knockback)
		{
			if (d.PosX > PosX)
				posit_tmp += 1000.0f * dt;
			else
				posit_tmp -= 1000.0f * dt;

			d.PosX = posit_tmp;
			distance += 1000.0f * dt;

			if (distance >= 100.0f)
			{
				std::cout << distance << std::endl;
				Knockback = false;
				distance = 0;
			}
		}
		for (char i = 0; i < Bullet_Buffer; ++i)
		{
			if (d.GetFireball()[i].IsActive())
			{
				if (Collision_.Dy_Rect_Rect(d.GetFireball()[i].Collision_, this->GetVelocity(), d.GetFireball()[i].GetVelocity(), dt))
				{
					Decrease_HP(d.GetDamage());
					d.AddCharge();
          d.PlayImpact();
					SFX.Play(0);
					d.GetFireball()[i].Projectile::ResetDist();
					d.GetFireball()[i].SetActive(false);
				}
			}
		}

		if (d.GetMfireball().IsActive())
		{
			if (Collision_.Dy_Rect_Rect(d.GetMfireball().Collision_, this->GetVelocity(), d.GetMfireball().GetVelocity(), dt))
			{
				Decrease_HP(d.GetMDamage());
				SFX.Play(0);
				d.GetMfireball().Projectile::ResetDist();
				d.GetMfireball().SetActive(false);
			}
		}

		if (this->Get_HP() <= 0)
		{
			SetActive(false);
		}

	}
  SFX.Update();
}

void Grunt::MoveTowardPlayer(const Dragon &d, const float dt)
{
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

	PosX -= MovementX * 0.6f * dt;
	//PosY -= MovementY / 60;

	//this->Transform_.SetTranslate(PosX, PosY); //PosX += GetVelocity
	//this->Transform_.Concat();
}

void Grunt::LineOfSight(const Dragon &d)
{
	float playerDist = (d.PosX - this->PosX);
	if (playerDist <= 600.0f && playerDist >= -600.0f)  // vision range
	{
		if ((d.PosY < PosY + 150.0f) && (d.PosY > PosY - 150.0f))
		{
			PlayerSeen = true;
		}
	}
	else
	{
		PlayerSeen = false;
	}

	
}

void Grunt::AttackPlayer(const Dragon &/*d*/)
{
	//
	//std::cout << "Attacking" << std::endl;
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

		/*this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();*/
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
			/*this->Transform_.SetTranslate(PosX, PosY);
			this->Transform_.Concat();*/
		}
	}
}
