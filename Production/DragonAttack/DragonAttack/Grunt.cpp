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
#include <iostream>

namespace // global variables just for THIS file
{
	const int grunt_hp = 20;
	const float GRUNT_SCALE = 70.0F;
	bool recent_kb = false;
}

Audio_Engine Grunt::SFX{ 1, [](std::vector<std::string> &playlist)
->void {	playlist.push_back(".//Audio/Hit_02.mp3");
} };

/* Grunt Constructor */
Grunt::Grunt(Sprite *p_Sprite, const float posX, const float posY)
	: Characters(p_Sprite, grunt_hp,
		Col_Comp{ posX - GRUNT_SCALE, posY - GRUNT_SCALE,
				  posX + GRUNT_SCALE, posY + GRUNT_SCALE, Rect }),
	current_action{ IDLE },
	anim{ WALK_ANIM + 1, 2, 5, [](std::vector <Range>& Init) -> void 
		{
			Init.push_back(Range{ 0.0f, 1.0f, 0.00f, 0.00f }); // Hit
			Init.push_back(Range{ 0.0f, 1.0f, 0.33f, 0.33f }); // Idle
			Init.push_back(Range{ 0.0f, 1.0f, 0.66f, 0.66f }); // Walk
		} 
	},
	PlayerSeen{ false }, PlayerInRange{ false },
	MovementX{ 0.5f }, MovementY{ 0.5f },
	moveSpd{ 3.0f }, EstIdleX{ 0 }
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = posX;
	PosY = posY;
	SetVelocity(AEVec2{ 160.0f, 0.0f });
	Reset_Idle_Time(1.0f);
}

/* Idle state for grunt */
void Grunt::Idle(const Dragon &d, const float dt)
{
	/*//UNREFERENCED_PARAMETER(d);
	//float IdleXPos = PosX;

	//int MaxXPos = (int)(IdleXPos + 120.0f); // max boundary
	//int MinXPos = (int)(IdleXPos - 120.0f); // min boundary
	//int EstCurrentX = (int)(this->PosX);

	//if (EstCurrentX < (EstIdleX - 121) || EstCurrentX >(EstIdleX + 120)) // if lesser than min boundary OR greater than max boundary
	//{
	//	//reason for strange values is due to how the obj will enter the bottom condition if it is close to the boundary
	//	//then re-enter this condition because of the *= -1, forever continuing the back and forth

	//	MovementX = (this->PosX) - IdleXPos;
	//	PosX -= MovementX / 120;
	//}
	//else
	//{
	//	if (PlayerSeen == false)
	//	{
	//		if ((PosX > MaxXPos) || (PosX < MinXPos))
	//		{
	//			moveSpd *= -1;
	//		}

	//		PosX += moveSpd;
	//	}
	//}*/
	
	anim.SetState(IDLE_ANIM);

	// waste time / idle on the spot <- not really working
	if (recent_kb)
	{
		if (Get_Idle_Time() < 0)
		{
			Reset_Idle_Time(1.0f);
			recent_kb = false;
			Set_Facing_Dir(d);
		}
		else
		{
			Decrease_Idle_Time(dt);
		}
	}


	if (!LineOfSight(d))
		return;
	else
	{
		Set_Facing_Dir(d);
		if (abs(d.PosX - PosX) <= 550.0f)
			current_action = MOVING;
	}
}

/* Movement State for grunt */
void Grunt::MoveTowardPlayer(const Dragon &d, const float dt)
{
	/*
	//MovementX = (this->PosX) - d.PosX;
	//MovementY = (this->PosY) - d.PosY;

	//if (MovementX < 100.0f && MovementY < 50.0f &&
	//	MovementX > -100.0f && MovementY > -50.0f) // Attack Range (change accordingly)
	//{
	//	PlayerInRange = true;
	//}
	//else
	//if (MovementX > 100.0f || MovementX < -100.0f) // not entering
	//{
	//	PlayerInRange = false;
	//}

	//PosX -= MovementX * 0.6f * dt;
	*/

	anim.SetState(WALK_ANIM);
	Set_Facing_Dir(d);	 // set the direction
						 
	if (LineOfSight(d)) // player is seen
	{
		if (Get_Direction() == RIGHT)
		{
			PosX += GetVelocity().x * dt; // move to the right
		}
		else
		{
			PosX -= GetVelocity().x * dt; // move to the left
		}

		// move to new coordinates
		Transform_.SetTranslate(PosX, PosY);
		Transform_.Concat();

		// update the collision box of grunt
		Collision_.Update_Col_Pos(PosX - GRUNT_SCALE, PosY - GRUNT_SCALE,  // min point
			PosX + GRUNT_SCALE - 60.0f, PosY + GRUNT_SCALE); // max point
	}
}

/* Checks if the player is in view */
bool Grunt::LineOfSight(const Dragon &d)
{
	/*//float playerDist = (d.PosX - this->PosX);
	//if (playerDist <= 600.0f && playerDist >= -600.0f)  // vision range
	//{
	//	if ((d.PosY < PosY + 150.0f) && (d.PosY > PosY - 150.0f))
	//	{
	//		PlayerSeen = true;
	//	}
	//}
	//else
	//{
	//	PlayerSeen = false;
	//}*/

	return abs(d.PosX - PosX) < 1000.0f;
}

/* Sets the grunt facing direction */
void Grunt::Set_Facing_Dir(const Dragon &d)
{
	if (d.PosX - PosX > 20.0f) // player on right side
	{
		Transform_.SetScale(1.0f, 1.0f); // flip texture to right
		Set_Direction(RIGHT);
	}
	else // player on left side
	{
		Transform_.SetScale(-1.0f, 1.0f); // flip texture to left
		Set_Direction(LEFT);
	}

	Transform_.Concat();
}

/* Update function for grunt */
void Grunt::Update(Dragon &d, const float dt)
{
	/*if (IsActive())
	{
		LineOfSight(d);

		if (PlayerSeen)
		{
			//MoveTowardPlayer(d, dt);
			current_action = MOVING;
		}
		else
		{
			Idle(d);
			current_action = IDLE;
		}

		this->Transform_.SetTranslate(this->PosX, this->PosY);
		this->Transform_.Concat();

		PosY -= 10.0f; //testing gravity
		// update the collision box of grunt
		this->Collision_.Update_Col_Pos(this->PosX - GRUNT_SCALE, this->PosY - GRUNT_SCALE,  // min point
										this->PosX + GRUNT_SCALE - 60.0f, this->PosY + GRUNT_SCALE); // max point

		//collision with player, player loses health
		if(!Knockback)
			if (Collision_.Dy_Rect_Rect(d.Collision_, GetVelocity(), d.GetVelocity(), dt))
			{
				d.Decrease_HP();
				d.PlayHit();
				Knockback = true;
				posit_tmp = d.PosX;
			}

		if (Knockback)
		{
			if (d.PosX > PosX)
				posit_tmp += 1000.0f * dt;
			else
				posit_tmp -= 1000.0f * dt;

			d.PosX = posit_tmp;

			distance += 1000.0f * dt;

			if (distance >= 20.0f)
			{
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

					anim.SetState(HIT_ANIM);
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

				anim.SetState(HIT_ANIM);
			}
		}

		if (this->Get_HP() <= 0)
		{
			SetActive(false);
		}

	}
	// default animation
	//if (anim.GetComplete(HIT_ANIM))
	//	anim.SetState(IDLE_ANIM);

	switch (current_action) // state machine for knight
	{
		case IDLE:   Idle(d);
			anim.SetState(IDLE_ANIM);
			break;
		case MOVING: MoveTowardPlayer(d, dt);
			anim.SetState(WALK_ANIM);
			break;
		//case DEAD:   Dead();
		//	break;
		default: break;
	}

	// update animation
	anim.Update(*Sprite_);

	SFX.Update();*/

	if (Get_HP() <= 0)
	{
		SetActive(false);
	}

	if (IsActive())
	{
		/* Checks for collision with fireball */
		for (char i = 0; i < Bullet_Buffer; ++i)
		{
			if (d.GetFireball()[i].IsActive())
			{
				if (Collision_.Dy_Rect_Rect(d.GetFireball()[i].Collision_, GetVelocity(), 
					d.GetFireball()[i].GetVelocity(), dt))
				{
					anim.SetState(HIT_ANIM);

					Decrease_HP(d.GetDamage());
					d.AddCharge();
					d.PlayImpact();
					SFX.Play(0);
					d.GetFireball()[i].Projectile::ResetDist();
					d.GetFireball()[i].SetActive(false);
				}
			}
		}

		/* Checks for collision with mega fireball */
		if (d.GetMfireball().IsActive())
		{
			if (Collision_.Dy_Rect_Rect(d.GetMfireball().Collision_, GetVelocity(), 
				d.GetMfireball().GetVelocity(), dt))
			{
				anim.SetState(HIT_ANIM);

				Decrease_HP(d.GetMDamage());
				SFX.Play(0);
				d.GetMfireball().Projectile::ResetDist();
				d.GetMfireball().SetActive(false);
			}
		}

		//collision with player, player loses health
		if (!Knockback)
			if (Collision_.Dy_Rect_Rect(d.Collision_, GetVelocity(), d.GetVelocity(), dt))
			{
				d.Decrease_HP();
				d.PlayHit();
				Knockback = true;
				posit_tmp = d.PosX;
			}

		if (Knockback)
		{
			if (d.PosX > PosX)
				posit_tmp += 1000.0f * dt;
			else
				posit_tmp -= 1000.0f * dt;

			d.PosX = posit_tmp;

			distance += 1000.0f * dt;

			if (distance >= 20.0f)
			{
				Knockback = false;
				distance = 0;
			}
			recent_kb = true;
			current_action = IDLE;
		}
	}

	PosY -= 10.0f; //Gravity

	// update the collision box of grunt
	Collision_.Update_Col_Pos(PosX - GRUNT_SCALE, PosY - GRUNT_SCALE,  // min point
		                      PosX + GRUNT_SCALE, PosY + GRUNT_SCALE); // max point

	Transform_.SetTranslate(PosX, PosY);
	Transform_.Concat();

	switch (current_action)
	{
		case IDLE:   Idle(d, dt);
			//anim.SetState(IDLE_ANIM);
			break;
		case MOVING: MoveTowardPlayer(d, dt);
			//anim.SetState(WALK_ANIM);
			break;
		default: break;
	}
	anim.Update(*Sprite_);
}
