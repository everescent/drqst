#include "Knight.h"
#include "cmath"

namespace  // global variables for KNIGHT
{
	const int HEALTH = 100;
	const float GRAVITY = 10.0f;

	const float ATTACK_RANGE = 120.0f;
	const float STAB_ACCELRATOR = 100.0f;

	const float KNIGHT_SCALE = 70.0f;
	const float STAB_SCALE = 40.0f;
	const AEVec2 STAB_VELOCITY {20.0f, 0.0f};

}

Knight::Knight(const AEVec2 & spawn_location, Sprite* texture)
	: Characters(texture, HEALTH, 
		Col_Comp{ spawn_location.x - KNIGHT_SCALE, spawn_location.y - KNIGHT_SCALE , 
			      spawn_location.x + KNIGHT_SCALE , spawn_location.y + KNIGHT_SCALE, Rect }),
	  current_action{ IDLE }, time_traveled{0},

	stab{ Get_Attack_Sprite(STAB_SPRITE),
		  Col_Comp { spawn_location.x - STAB_SCALE, spawn_location.y - STAB_SCALE,
					 spawn_location.x + STAB_SCALE, spawn_location.y + STAB_SCALE, Rect}}
{
	SetPos(spawn_location.x, spawn_location.y);						 // spawn location of knight
	SetActive(false);												 // don't render on screen yet
	Transform_.SetTranslate(spawn_location.x, spawn_location.y);	 // moving him to screen location
	Transform_.Concat();
	Reset_Idle_Time(2.0f);											 // duration of idle time fot knight
	SetVelocity(AEVec2{ 50.0f, 0.0f });								 // velocity of knight
	
	stab.SetVelocity(STAB_VELOCITY);								 // velocity for stab
}

void Knight::Idle(const Dragon& d, const float dt)
{
	if (!Line_Of_Sight(d))
		return;

	if (Get_Idle_Time() < 0) // finish idling
	{
		Reset_Idle_Time(2.0f); // reset idle duration
		
		// if knight is within attack range, attack player, else continue moving
		abs(d.PosX - PosX) <= ATTACK_RANGE ? current_action = ATTACK : current_action = MOVING;
		Set_Forward_Dir(d); // set direction to face
	}
	else
	{
		Decrease_Idle_Time(dt); // reduce idle time
	}
}

void Knight::Moving(const Dragon &d, const float dt)
{
	Set_Forward_Dir(d);	 // set the direction the knight suppose to go

	if (Line_Of_Sight(d))
	{
		if (Get_Direction() == RIGHT)
		{
			PosX += GetVelocity().x * dt; // move the knight to the right
		}
		else
		{
			PosX -= GetVelocity().x * dt; // move the knight to the left
		}

		Transform_.SetTranslate(PosX, PosY); // move him to the new coordinates
		Transform_.Concat();

		// update the collision box of knight
		Collision_.Update_Col_Pos(PosX - KNIGHT_SCALE, PosY - KNIGHT_SCALE,
							      PosX + KNIGHT_SCALE, PosY + KNIGHT_SCALE);
	}
		
		if (abs(d.PosX - PosX) <= ATTACK_RANGE) // player within attack range
		{
			current_action = ATTACK;
			time_traveled = 2.0f;
			return;
		}
		else if (time_traveled > 2.0f)    // player not within range
		{
			current_action = IDLE;
			time_traveled = 0.0f;
			return;
		}
	

	time_traveled += dt; // reduce timer
	 
}

void Knight::Attack(Dragon& d, const float dt)
{
	UNREFERENCED_PARAMETER(dt);

	if ( ! stab.ongoing_attack) // place stab position to knight location
	{
		stab.Start_Attack(PosX, PosY);
	}
	
	// update positon	
	if (Get_Direction() == RIGHT)
		stab.Projectile::Update(dt, STAB_SCALE, false, 180);    // rotate texture to look right
	else
		stab.Projectile::Update(dt, STAB_SCALE, false, 0);      // rotate texture to look left
	
	
	stab.Transform_.Concat();			 // apply rotation onto stab 

	if ( ! stab.GetCollided()) // check if attack collided with player
	{
		if (stab.Collision_.Dy_Rect_Rect(d.Collision_, stab.GetVelocity(), d.GetVelocity(), dt))
		{
			d.Decrease_HP();
			stab.SetCollided(true);
		}
	}

	if (stab.GetDist() > 10.0f)  // once distance past 10 spaces, accelerate the stab attack
	{
		AEVec2 new_velocity = stab.GetVelocity();
		new_velocity.x += STAB_ACCELRATOR;
		stab.SetVelocity(new_velocity);
	}

	if (stab.GetDist() > 100.0f) // end of stab attack
	{
		stab.End_Attack();				 // reset stab variables
		current_action = IDLE;			 // change behaviour to idle
		stab.SetVelocity(STAB_VELOCITY); // reset the veloctiy of stab
	}

}

void Knight::Dead(void)
{
	SetActive(false);	    // set active back to false
	Set_HP(HEALTH);			// set HP to original state for future usage
	stab.SetActive(false);  // set stab to false also
}

bool Knight::Line_Of_Sight(const Dragon& d)
{
	return abs(d.PosX - PosX) < 1280.0f; // check if knight can see player on screen
}

void Knight::Set_Forward_Dir(const Dragon& d)
{
	if (d.PosX - PosX > 0)   // dragom on the right of player
	{
		Transform_.SetScale(1.0f, 1.0f);
		Set_Direction(RIGHT);
		stab.SetDir(true);
	}
	else                     // dragon on the left of player
	{
		Transform_.SetScale(-1.0f, 1.0f);
		Set_Direction(LEFT);
		stab.SetDir(false);
	}

	Transform_.Concat();
}

void Knight::Update(Dragon &d, const float dt)
{
	if (Get_HP() < 1) // knight has died
	{
		current_action = DEAD;
	}
	
	
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

	PosY -= GRAVITY;
	Collision_.Update_Col_Pos(PosX - KNIGHT_SCALE, PosY - KNIGHT_SCALE,
		PosX + KNIGHT_SCALE, PosY + KNIGHT_SCALE);
	Transform_.SetTranslate(PosX, PosY);
	Transform_.Concat();

	switch (current_action) // state machine for knight
	{
	case IDLE:   Idle(d, dt);
		break;
	case MOVING: Moving(d, dt);
		break;
	case ATTACK: Attack(d, dt);
		break;
	case DEAD:   Dead();
		break;
	default: break;
	}
}

void Knight::Render() // render both knight and his attack on screen
{
	GameObject::Render();
	stab.Render();
}

