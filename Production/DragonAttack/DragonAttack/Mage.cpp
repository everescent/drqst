/* Start Header ************************************************************************/
/*!
\file       Mage.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
			This file updates the mage behaviour

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Mage.h"

namespace // for global variables in this file
{ 
	const char HEALTH          = 50;         // starting health of mage
	const char TELEPORT_FLAG   = 25;		 // hp threshold for mage to teleport

	const float MAGE_SCALE     = 30.0F;      // scale for mage
	const float EBALL_SCALE    = 20.0f;		 // scale for energyball

	const float LEFT_BOUNDARY  = -600.0f;    // boundaries of screen
	const float RIGHT_BOUNDARY = 600.0f;	 // boundaries of screen

}

// constructor for mage, that sets its position to the parameters that were given
Mage::Mage(const AEVec2& position, const Sprite* texture)
	:Characters(texture,
				HEALTH, Col_Comp{position.x - MAGE_SCALE, position.y - MAGE_SCALE , position.x + MAGE_SCALE , position.y + MAGE_SCALE, Rect }) ,
	current_action{ IDLE },

	energy_ball{ Sprite{ S_CreateSquare(EBALL_SCALE, "energyball.png") },
				 Col_Comp{ position.x - EBALL_SCALE, position.y - EBALL_SCALE,
						   position.x + EBALL_SCALE, position.y + EBALL_SCALE, Rect } }

{
	this->SetActive(false);                                                // don't spawn mage
	this->SetPos(position.x, position.y);								   // starting coordinates
	this->Transform_.SetTranslate(position.x, position.y);				   // add to matrix
	this->Transform_.Concat();											   // add to final matrix
	//this->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);			   // rendering mode of mage
	this->Reset_Idle_Time(2.0f);										   // idling time for mage
	energy_ball.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);	   // rendering mode of energyball
}

// update function for mage, performs the behaviour of mage at current frame
void Mage::Update(Dragon &d, const float dt)
{	
	// changes state to dead when hp is 0 and there is no ongoing attack at the moment
	if (this->Get_HP() <= 0 && ! energy_ball.ongoing_attack)
		this->current_action = DEAD;

	// teleport the mage to the back of the player once hp falls below a certain threshold
	if (this->Get_HP() < TELEPORT_FLAG && this->teleport && !energy_ball.ongoing_attack)
	{
		this->current_action = MOVING;
	}
	
	// checks for collision with fireball if mage is active
	if (this->IsActive())
	{
		for (char i = 0; i < Bullet_Buffer; ++i)
			if (d.GetFireball()[i].IsActive())
				if (Collision_.Dy_Rect_Rect(d.GetFireball()[i].Collision_, GetVelocity(),
					d.GetFireball()[i].GetVelocity(), dt))
				{
					//Decrease HP by player's damage
					Decrease_HP(d.GetDamage());
					//Add mega fireball charge
					d.AddCharge();
					d.PlayImpact();
					//Reset the distance of the fireball and set false
					d.GetFireball()[i].Projectile::ResetDist();
					d.GetFireball()[i].SetActive(false);
				}

		// mega fire ball hit lancelot
		if (d.GetMfireball().IsActive())
		{
			if (Collision_.Dy_Rect_Rect(d.GetMfireball().Collision_, GetVelocity(),
				d.GetMfireball().GetVelocity(), dt))
			{
				Decrease_HP(d.GetMDamage());
				d.GetMfireball().Projectile::ResetDist();
				d.GetMfireball().SetActive(false);
			}
		}
	}

	// calls the behaviour function depending on current behaviour
	switch (current_action)
	{
	case IDLE:    Idle(dt, d);
		break;
	case ATTACK:  Attack(dt, d);
		break;
	case MOVING:  Move(d);
		break;
	case DEAD:    Dead();
		break;
    default: break;
	}

}

// render function for mage
void Mage::Render(void)
{
	GameObject::Render(); // render the mage


	energy_ball.Render(); //render the energyball
}

// idle function of mage
void Mage::Idle(const float dt, const Dragon &d)
{
	// changes the behaviour to attack once the timer hits 0
	if (Get_Idle_Time() <= 0)
	{
		Reset_Idle_Time(2.0f);

		//  change behavior to attack if player is within sight
		if (Line_Of_Sight(d))			
			current_action = ATTACK;
	}
	else
	{
		// reduce timer by delta time
		Decrease_Idle_Time(dt);
	}
}

// attack function of mage
// shoots energy balls at player
void Mage::Attack(const float dt, Dragon &d)
{
	UNREFERENCED_PARAMETER(dt);

	// range limit of mage
	static const float RANGE_LIMIT = 600.0f;

	// resets the coordinates and velocity of energy ball
	// this is only called once before the firing of energy ball
	if (!energy_ball.ongoing_attack)
	{
		AEVec2 Displacement = { d.PosX - this->PosX, d.PosY - this->PosY };
		AEVec2Normalize(&Displacement, &Displacement);
		AEVec2Scale(&Displacement, &Displacement, 360.0f);
		energy_ball.SetVelocity(Displacement);
		
		// set the energy ball position to the mage current location
		energy_ball.SetPos(this->PosX, this->PosY);
		
		
		energy_ball.SetActive(true);       // set energyball to active
		energy_ball.ongoing_attack = true; // attack ongoing flag to true
	}

	// move the energy ball base on its velocity
	energy_ball.Projectile::Update(EBALL_SCALE);

	// check for collision between energyball and player
	if (energy_ball.Collision_.Dy_Rect_Rect(d.Collision_, energy_ball.GetVelocity(), d.GetVelocity(), dt))
	{
		d.Decrease_HP();                // decrease hp if hit
		energy_ball.SetCollided(true);  // set collision flag to true
	}

	// kills the energy ball if it flew past its limit or it collided with player
	if (energy_ball.GetDist() > RANGE_LIMIT || energy_ball.GetCollided())
	{
		energy_ball.End_Attack();
		this->current_action = IDLE;         // change behaviour
	}

}


void Mage::Move(const Dragon &d)
{
	// for now, teleport behind player

	const float location = d.PosX - this->PosX; // get the distance between dragon and mage
	const float teleport_distance = 100.0f;     // how far the mage teleports away from player

	if (location > 0) // dragon is on the right of mage
	{
		this->PosX = d.PosX + teleport_distance > RIGHT_BOUNDARY ? RIGHT_BOUNDARY
														  : d.PosX + teleport_distance;
	}
	else			 // dragon is on the left of mage
	{
		this->PosX = d.PosX - teleport_distance < LEFT_BOUNDARY ? LEFT_BOUNDARY
			                                             : d.PosX - teleport_distance;

	}

	// update the collision box of mage
	this->Collision_.Update_Col_Pos(this->PosX - MAGE_SCALE, this->PosY - MAGE_SCALE,  // min point
									this->PosX + MAGE_SCALE, this->PosY + MAGE_SCALE);	// max point

	this->Transform_.SetTranslate(this->PosX, this->PosY);   // update coordinates of mage
	this->Transform_.Concat();								 // update coordinates of mage
	this->teleport = false;									 // mage can only teleport once
	this->current_action = IDLE;							 // change behaviour

}

// dead function of mage
void Mage::Dead(void)
{
	SetActive(false); // set active to false
	Set_HP(HEALTH);   // reset the hp of mage
}

// checks if the player is within sight
bool Mage::Line_Of_Sight(const Dragon &d)
{
	// distance between player and mage
	float distance = { d.PosX - this->PosX };
	
	// checks if player is in same screen as mage
	return distance > LEFT_BOUNDARY && distance < RIGHT_BOUNDARY;
}