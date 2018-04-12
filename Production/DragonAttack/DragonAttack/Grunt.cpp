/* Start Header ************************************************************************/
/*!
\file    Grunt.cpp
\project Dragon Attack
\author  Andrew Chong
\email   c.jiahaoandrew@digipen.edu
\brief
	Grunt class body definitions.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Grunt.h"
#include "Transform.h"
#include <iostream>

// Global variables
namespace 
{
	const int grunt_hp = 20;
	const float GRUNT_SCALE = 70.0F;
	bool recent_kb = false;
	const float grunt_velocity = 160.0f;
}

// Audio engine component for grunt sound effects
Audio_Engine Grunt::SFX{ 1, [](std::vector<std::string> &playlist)
->void {	playlist.push_back(".//Audio/Hit_02.mp3");
} };

// Grunt Constructor
Grunt::Grunt(Sprite *p_Sprite, const float posX, const float posY)
	: Characters(p_Sprite, grunt_hp,
		Col_Comp{ posX - GRUNT_SCALE, posY - GRUNT_SCALE,
				  posX + GRUNT_SCALE, posY + GRUNT_SCALE, Rect }), // Set collision positions
	current_action{ IDLE }, // Set default animation
	anim{ WALK_ANIM + 1, 3, 5, [](std::vector <Range>& Init) -> void  // Set animation states
		{
			Init.push_back(Range{ 0.0f, 1.0f, 0.00f, 0.00f }); // Hit
			Init.push_back(Range{ 0.0f, 1.0f, 0.33f, 0.33f }); // Idle
			Init.push_back(Range{ 0.0f, 1.0f, 0.66f, 0.66f }); // Walk
		} 
	}
{
	SetActive(true);
	Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	PosX = posX;
	PosY = posY;
	SetVelocity(AEVec2{ grunt_velocity, 0.0f });
	Reset_Idle_Time(1.0f);
	Transform_.SetTranslate(PosX, PosY);
	Transform_.Concat();
}

// Idle state for grunt
void Grunt::Idle(const Dragon &d, const float dt)
{
	anim.SetState(IDLE_ANIM);

	// waste time / idle on the spot
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

	// Unable to see player
	if (!LineOfSight(d))
		return;
	else
	{
		Set_Facing_Dir(d);
		if (abs(d.PosX - PosX) <= 550.0f && abs(d.PosX - PosX) > 80.0f)
			current_action = MOVING;
	}
}

// Movement State for grunt
void Grunt::MoveTowardPlayer(const Dragon &d, const float dt)
{
	anim.SetState(WALK_ANIM);
	Set_Facing_Dir(d);	 // set the direction
						 
	if (LineOfSight(d)) // player is seen
	{
		if (abs(PosX - d.PosX) <= 80.0f)
		{
			current_action = IDLE;
			return;
		}

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
			PosX + GRUNT_SCALE, PosY + GRUNT_SCALE); // max point
	}
	else
		anim.SetState(IDLE_ANIM); // set to idle
}

// Checks if the player is in view
bool Grunt::LineOfSight(const Dragon &d)
{
	return (abs(d.PosX - PosX) < 1000.0f) && (abs(d.PosY - PosY) < 200.0f);
}

// Sets the grunt facing direction
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

// Update function for grunt
void Grunt::Update(Dragon &d, const float dt)
{
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

					Decrease_HP(d.GetDamage()); // decrease AI health by fireball dmg
					d.AddCharge(); // adds to mega fireball charge
					d.PlayImpact(); // impact sfx
					SFX.Play(0); // plays sfx
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

				Decrease_HP(d.GetMDamage()); // decrease AI health by mega fireball dmg
				SFX.Play(0); // plays sfx
				d.GetMfireball().Projectile::ResetDist();
				d.GetMfireball().SetActive(false);
			}
		}

		// collision with player, player loses health
		if (Collision_.Dy_Rect_Rect(d.Collision_, GetVelocity(), d.GetVelocity(), dt))
		{
			d.Decrease_HP();
			d.PlayHit();
			d.SetInvul(true);
		}
	}

	PosY -= 10.0f; //Gravity

	// update the collision box of grunt
	Collision_.Update_Col_Pos(PosX - GRUNT_SCALE, PosY - GRUNT_SCALE,  // min point
		                      PosX + GRUNT_SCALE, PosY + GRUNT_SCALE); // max point

	Transform_.SetTranslate(PosX, PosY);
	Transform_.Concat();

	// switch case for determining the current state of the grunt
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

// destructor for grunt
Grunt::~Grunt()
{
    delete Sprite_;
}

// mutes the sfx for grunt
void Grunt::Mute()
{
    SFX.SetVolume(0, 0.0f);
    SFX.SetPause( 0, true);
}

// unmutes the sfx for grunt
void Grunt::Unmute()
{
    SFX.SetVolume(0, 1.0f);
    SFX.SetPause( 0, false);
}