#include "Mage.h"


namespace // for global variables in this file
{
	const AEVec2 SPAWN_OUTSIDE {-700.0f, -400.0f};

	const char HEALTH = 50;
	const char TELEPORT_FLAG = 25;

	const float SCALE = 30.0F;

	const float LEFT_BOUNDARY = -600.0f;
	const float RIGHT_BOUNDARY = 600.0f;
}



Mage::Mage()
	:Characters(S_CreateSquare(SCALE, ".//Textures/mage.png"),
				HEALTH, Col_Comp{ }) , current_action { IDLE }
{
	this->SetActive(false);
	this->SetPos(SPAWN_OUTSIDE.x, SPAWN_OUTSIDE.y);
	this->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

Mage::Mage(const float x, const float y)
	:Characters(S_CreateSquare(SCALE, ".//Textures/mage.png"),
				HEALTH, Col_Comp{x - SCALE, y - SCALE , x + SCALE , y + SCALE, Rect }) ,
	current_action{ IDLE }
{
	this->SetActive(false);
	this->SetPos(x, y);
	this->Transform_.SetTranslate(x, y);
	this->Transform_.Concat();
	this->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

void Mage::Update(const float dt, Dragon &d)
{
	if ( this->teleport)
	{
		this->current_action = MOVING;
	}
	
	if (this->Get_HP() < 0)
		this->current_action = DEAD;

	switch (current_action)
	{
	case IDLE: Idle(dt);
		break;
	case ATTACK: Attack(dt, d);
		break;
	case MOVING: Move(d);
		break;
	case DEAD: Dead();
		break;
    default: break;
	}
}

void Mage::Render(void)
{
	GameObject::Render();
}

void Mage::Idle(const float dt)
{
	static float timer = 1.0f;
	
	if ( ! timer )
		timer = 1.0f;

	// changes the behaviour to attack once the timer hits 0
	timer <= 0 ? timer = 0.0f, current_action = ATTACK : timer -= dt;
}

void Mage::Attack(const float dt, const Dragon &d)
{
	UNREFERENCED_PARAMETER(dt);
	UNREFERENCED_PARAMETER(d);

	current_action = IDLE;
}


void Mage::Move(const Dragon &d)
{
	// for now, teleport behind player

	float location = d.PosX - this->PosX; // get the distance between dragon and mage
	float teleport_distance = 100.0f;     // how far the mage teleports away from player

	if (location > 0) // dragon is on the right of mage
	{
		this->PosX = d.PosX + teleport_distance > RIGHT_BOUNDARY ? RIGHT_BOUNDARY 
																 : d.PosX + teleport_distance;
	}
	else			 // dragon is on the left of mage
	{
		this->PosX = d.PosX - teleport_distance > LEFT_BOUNDARY ? LEFT_BOUNDARY
			: d.PosX - teleport_distance;

	}

	this->Collision_.Update_Col_Pos(this->PosX - SCALE, this->PosY - SCALE,  // min point
									this->PosX + SCALE, this->PosY + SCALE);	// max point

	this->Transform_.SetTranslate(this->PosX, this->PosY);
	this->Transform_.Concat();
	this->teleport = false;
	this->current_action = IDLE;
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