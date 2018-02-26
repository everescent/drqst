#include "Mage.h"

namespace // for global variables in this file
{
	const AEVec2 SPAWN_OUTSIDE {-700.0f, -400.0f};

	const char HEALTH          = 50;
	const char TELEPORT_FLAG   = 25;

	const float MAGE_SCALE     = 30.0F;
	const float EBALL_SCALE    = 20.0f;

	const float LEFT_BOUNDARY  = -600.0f;
	const float RIGHT_BOUNDARY = 600.0f;

}



Mage::Mage()
	:Characters(S_CreateSquare(MAGE_SCALE, ".//Textures/mage.png"),
				HEALTH, Col_Comp{ }) , current_action { IDLE },

	energy_ball{ Sprite{S_CreateSquare(EBALL_SCALE, "energyball.png")},
				 Col_Comp{SPAWN_OUTSIDE.x - EBALL_SCALE, SPAWN_OUTSIDE.y - EBALL_SCALE,
						  SPAWN_OUTSIDE.x + EBALL_SCALE, SPAWN_OUTSIDE.y + EBALL_SCALE, Rect} }


	, ani{
	4, 900, 900, 9, 9,
	[](std::vector <Range> &A_State)
{
	A_State.push_back(Range{ 0, 0, 0, 9 });
	A_State.push_back(Range{ 1, 1, 1, 9 });
	A_State.push_back(Range{ 2, 2, 2, 9 });
	A_State.push_back(Range{ 3, 3, 3, 9 });
}

}





{
	this->SetActive(false);
	this->SetPos(SPAWN_OUTSIDE.x, SPAWN_OUTSIDE.y);
	this->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	energy_ball.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

Mage::Mage(const AEVec2 & pos)
	:Characters(S_CreateSquare(MAGE_SCALE, ".//Textures/mage.png"),
				HEALTH, Col_Comp{pos.x - MAGE_SCALE, pos.y - MAGE_SCALE , pos.x + MAGE_SCALE , pos.y + MAGE_SCALE, Rect }) ,
	current_action{ IDLE },

	energy_ball{ Sprite{ S_CreateSquare(EBALL_SCALE, "energyball.png") },
				 Col_Comp{ pos.x - EBALL_SCALE, pos.y - EBALL_SCALE,
						   pos.x + EBALL_SCALE, pos.y + EBALL_SCALE, Rect } }


	,ani {
	4, 900, 900, 9, 9,
		[](std::vector <Range> &A_State)
	{
		A_State.push_back(Range{ 0, 9, 1, 2 });
		A_State.push_back(Range{ 0, 9, 3, 4 });
		A_State.push_back(Range{ 0, 9, 5, 6 });
		A_State.push_back(Range{ 0, 9, 7, 8 });
	}
}




{
	this->SetActive(false);
	this->SetPos(pos.x, pos.y);
	this->Transform_.SetTranslate(pos.x,pos.y);
	this->Transform_.Concat();
	this->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	energy_ball.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

void Mage::Update(const float dt, Dragon &d)
{	
	if (this->Get_HP() <= 0 && ! energy_ball.ongoing_attack)
		this->current_action = DEAD;

	if (this->IsActive())
	{
		for (char i = 0; i < Bullet_Buffer; ++i)
			if (d.GetFireball()[i].IsActive())
				if (Collision_.Dy_Rect_Rect(d.GetFireball()[i].Collision_, GetVelocity(),
					d.GetFireball()[i].GetVelocity(), dt))
				{
					//Decrease HP by d's damage
					Decrease_HP(d.GetDamage());
					//Reset the distance of the fireball and set false
					d.GetFireball()[i].Projectile::ResetDist();
					d.GetFireball()[i].SetActive(false);
				}
	}

	switch (current_action)
	{
	case IDLE: Idle(dt, d);
		break;
	case ATTACK: Attack(dt, d);
		break;
	case MOVING: Move(d);
		break;
	case DEAD: Dead();
		break;
    default: break;
	}





	ani.Update(this->Sprite_, current_action);







}

void Mage::Render(void)
{
	GameObject::Render();


	energy_ball.Render();
}

void Mage::Idle(const float dt, const Dragon &d)
{
	static float timer = 1.0f;
	
	if ( ! timer )
		timer = 1.0f;

	// changes the behaviour to attack once the timer hits 0
	if (timer <= 0)
	{
		timer = 0.0f;

		//  change behavior to attack if player is within sight
		if (Line_Of_Sight(d))
			ani.Update(this->Sprite_, current_action, true),
			
			
			
			
			current_action = ATTACK;
	}
	else
	{
		timer -= dt;
	}
}

void Mage::Attack(const float dt, Dragon &d)
{
	UNREFERENCED_PARAMETER(dt);
	UNREFERENCED_PARAMETER(d);

	static const float RANGE_LIMIT = 500.0f;

	if (!energy_ball.ongoing_attack)
	{
		AEVec2 Displacement = { d.PosX - this->PosX, d.PosY - this->PosY };
		AEVec2Normalize(&Displacement, &Displacement);
		AEVec2Scale(&Displacement, &Displacement, 360.0f);
		energy_ball.SetVelocity(Displacement);
		
		energy_ball.SetPos(this->PosX, this->PosY);
		energy_ball.SetActive(true);
		energy_ball.ongoing_attack = true;
	}

	energy_ball.Projectile::Update(EBALL_SCALE);

	if (!energy_ball.GetCollided())
	{
		if (energy_ball.Collision_.Dy_Rect_Rect(d.Collision_, energy_ball.GetVelocity(), d.GetVelocity(), dt))
		{
			d.Decrease_HP();
			energy_ball.SetCollided(true);
		}
	}

	if (energy_ball.GetDist() > RANGE_LIMIT || energy_ball.GetCollided())
	{
		energy_ball.SetActive(false);
		energy_ball.ResetDist();
		energy_ball.ongoing_attack = false;
		energy_ball.SetCollided(false);

		if (this->Get_HP() < TELEPORT_FLAG && this->teleport)
		{
			this->current_action = MOVING; ani.Update(this->Sprite_, current_action, true);
		}
		else
		{
			this->current_action = IDLE; ani.Update(this->Sprite_, current_action, true);
		}
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

	this->Collision_.Update_Col_Pos(this->PosX - MAGE_SCALE, this->PosY - MAGE_SCALE,  // min point
									this->PosX + MAGE_SCALE, this->PosY + MAGE_SCALE);	// max point

	this->Transform_.SetTranslate(this->PosX, this->PosY);
	this->Transform_.Concat();
	this->teleport = false;
	this->current_action = IDLE;


	ani.Update(this->Sprite_, current_action, true);
}

void Mage::Dead(void)
{
	SetActive(false);
	SetPos(SPAWN_OUTSIDE.x, SPAWN_OUTSIDE.y);
	Set_HP(HEALTH);
}

bool Mage::Line_Of_Sight(const Dragon &d)
{
	float distance = { d.PosX - this->PosX };
	
	return distance > LEFT_BOUNDARY && distance < RIGHT_BOUNDARY;
}