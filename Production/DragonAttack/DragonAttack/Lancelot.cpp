#include "Lancelot.h"
#include "Boss_States.h"
#include "Collision.h"
#include "Audio_Engine.h"
#include <iostream>
#include <cmath>
#include <vector>


namespace
{
	enum Lancelot_Moveset
	{
		STAB,
		SLASH,
		MAD_ENHANCEMENT,
		ARONDIGHT
	};
	
	std::vector<Boss_Attack> lancelot;       // array to store lancelot attack

	Boss_Action_State current_action = IDLE; // lancelot current action
	Audio_Engine music{ 1,					 // sound for lance lot
	[](std::vector<std::string>& s)->void
	{
		s.push_back(".//Audio/Hit_02.mp3");
	} };
	
	
	const int HEALTH           = 800;   // health that lancelot start with
	const int PHASE2_HP        = 400;   // phase 2 trigger
	const float LANCELOT_SCALE = 60.0f; // lancelot size

	const char limit = 4;				// num of lancelot attacks

	float idle_time          = 1.0f;    // idling time for lancelot
	const float ATTACK_RANGE = 150.0f;	// range of attack of lancelot
	const float ATTACK_SCALE = 20.0f;   // scale of attacks
	Lancelot_Moveset currAttk = STAB;   // currnet attack lancelot is using

	// variables for slash and arondight
	const AEVec2 ATK_START_POINT {-100.0f, -130.0f}; 
	const AEVec2 ARONDIGHT_SCALE { 11.5f, 3.0f };
	const AEVec2 STARTING_POINT { 200.0f, -250.0f };
	const AEVec2 SLASH_VELOCITY { 20.0f, 500.0f };
	float angle = 25.0f;
	float angle_offset = 2.0f;
	float charge_time;


	// Variables for stab
	const AEVec2 STAB_VELOCITY{ 20.0f, 0.0f };

	Sprite attack_sprite;

	bool Player_Facing_Me(Lancelot&, Dragon&);
}

Lancelot::Lancelot(Sprite* texture)
	: Characters(texture,
		HEALTH,  Col_Comp{STARTING_POINT.x - LANCELOT_SCALE, STARTING_POINT.y - LANCELOT_SCALE,
						  STARTING_POINT.x + LANCELOT_SCALE, STARTING_POINT.y + LANCELOT_SCALE, Rect}),
	    M_E{ false }, arondight_particle{Effects_Get(ARONDIGHT_PARTICLE)}

{
	Transform_.SetTranslate(STARTING_POINT.x, STARTING_POINT.y); // spawn lancelot at this location
	Transform_.SetScale(-1.0f, 1.0f);
	Transform_.Concat();
	SetPos(STARTING_POINT.x, STARTING_POINT.y);    // update lancelot current coordinates
	SetActive(true);					           // spawn lancelot
	Set_Direction(LEFT);				           // face left
	SetVelocity({ 200.0f, 0.0f });                 // velocity for lancelot
	Reset_Idle_Time(idle_time);
	Init();							               // initialize the attacks lancelot have
}

void Lancelot::Init()
{
	//initialize lancelot attacks here
	const char* sword = ".//Textures/arondight.png";

	lancelot.reserve(limit);

	attack_sprite = S_CreateSquare(ATTACK_SCALE, sword);
	
	Init_Stab();
	Init_Slash();
	lancelot.emplace_back(Boss_Attack()); // mad enhancement
	Init_Arondight();

	// prevent unique mechanic from activating at the start of fight
	lancelot[MAD_ENHANCEMENT].cooldown_timer = 10.0f; 
	lancelot[MAD_ENHANCEMENT].cooldown = true; 

	// arondight particle variables
	arondight_particle->Emitter_.PPS_ = 10;
	arondight_particle->Emitter_.Vol_Max = 4096;
	arondight_particle->Emitter_.Direction_ = 90.0f;
	arondight_particle->Emitter_.Particle_Rand_.Spread_ = 360;
	arondight_particle->Emitter_.Conserve_ = 0.8f;
	arondight_particle->Emitter_.Size_ = 10.0f;
	arondight_particle->Emitter_.Speed_ = 4.0f;
	arondight_particle->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
	arondight_particle->Emitter_.Lifetime_ = 3.f;
	arondight_particle->Emitter_.Particle_Rand_.Life_Rand_ = 1;
	arondight_particle->Emitter_.Pos_.Min_Max.Angle_ = PI / 2.f;

	seed_initializer(); // initializes the seed for rng purposes

}

void Lancelot::Idle(const Dragon& d, const float dt)
{
	// change current_action to attack and idle to 0 once idling is over
	if (Get_Idle_Time() <= 0)
	{		
		current_action = abs(d.PosX - this->PosX) <= ATTACK_RANGE ? ATTACK : MOVING;
		Reset_Idle_Time(idle_time);
		Set_Face_Dir(d);
		Set_Attk_Dir();
	}
	else
		Decrease_Idle_Time(dt);
}

void Lancelot::Moving(const Dragon &d, const float dt)
{
	Set_Face_Dir(d);
	
	if (Get_Direction() == RIGHT)     
	{
		PosX += GetVelocity().x * dt; // move lancelot to the right
	}
	else if (Get_Direction() == LEFT) 
	{
		PosX -= GetVelocity().x * dt; // move lancelot to the left
	}

	Transform_.SetTranslate(PosX, PosY);
	Transform_.Concat();

	// update the collision box of lancelot
	Collision_.Update_Col_Pos(PosX - LANCELOT_SCALE, PosY - LANCELOT_SCALE,
							  PosX + LANCELOT_SCALE, PosY + LANCELOT_SCALE);
	
	//change state to attack once move_duration is 0, reset move_duration
	if (abs(d.PosX - this->PosX) <= ATTACK_RANGE)
	{
		current_action = ATTACK;
		Set_Attk_Dir();
	}
}

void Lancelot::Update(Dragon &d, float dt)
{

	if (this->Get_HP() < PHASE2_HP && phase & PHASE_1)
	{
		Lancelot_Phase2(dt); // change to phase 2
	}
	else if (this->Get_HP() <= 0)
	{
		current_action = DEAD;
	}

	if (M_E) // off mad enhancement state
	{
		if (lancelot[MAD_ENHANCEMENT].cooldown_timer < 10)
			this->Mad_Enhancement(dt);         
	}

	// checks if lancelot is alive
	if (IsActive())
	{
		// fireball hit lancelot
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
					music.Play(0);
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
				music.Play(0);
				d.PlayImpact();
			}
		}
	}
	// switch between boss states
	switch (current_action)
	{
	case IDLE: this->Idle(d, dt);
		break;

	case MOVING: this->Moving(d, dt);
		break;

	case ATTACK: this->Attack(d, dt);
		break;

	case DEAD:   this->Dead();
		break;

	default: break;
	}

	for (char i = 0; i < limit; ++i) // update the skill cooldowns
		lancelot[i].Update(dt);
}

void Lancelot::Render(void)
{
	GameObject::Render();
	lancelot[currAttk].Render();

	if(currAttk == ARONDIGHT && current_action == ATTACK)
		arondight_particle->Render();
}

void Lancelot::Attack(Dragon &d, const float dt)
{
	if (!lancelot[currAttk].ongoing_attack)
	{
		if (phase & PHASE_1 && !lancelot[MAD_ENHANCEMENT].cooldown)
		{
			// teleport lancelot to the center of the screen
			this->PosX = 0.0f;
			this->Transform_.SetTranslate(PosX, PosY);
			this->Transform_.Concat();
			
			charge_time = 2.0f;				// freeze lancelot in the middle of the screen
			Set_Vulnerable(false);		    // make him immune to attacks 
			currAttk = MAD_ENHANCEMENT;     

			// attack is currently ongoing
			lancelot[MAD_ENHANCEMENT].ongoing_attack = true;

			// update the collision box of lancelot
			Collision_.Update_Col_Pos(PosX - LANCELOT_SCALE, PosY - LANCELOT_SCALE,
				PosX + LANCELOT_SCALE, PosY + LANCELOT_SCALE);
		}

		else if (phase & PHASE_2 && !lancelot[ARONDIGHT].cooldown)
		{				
			angle = 0.0f;         // used for setting range the sword can go
			angle_offset = 3.0f;  // add to the angle every frame
			charge_time = 2.0f;   // freeze lancelot for 2 seconds so player can prepare
			currAttk = ARONDIGHT;

			// teleport lancelot to the middle of the screen
			this->PosX = 0.0f;
			this->Transform_.SetTranslate(PosX, PosY);
			this->Transform_.Concat();

			lancelot[ARONDIGHT].Start_Attack(20.0f, ATK_START_POINT.y + 200); // starting position of arondight
			lancelot[ARONDIGHT].Collision_.Update_Col_Pos(0.0f, 400.0f);	  // starting point of collision point
			lancelot[ARONDIGHT].Transform_.SetRotation(-90.0f);				  // rotate texture
			lancelot[ARONDIGHT].Transform_.SetTranslate(lancelot[ARONDIGHT].PosX, lancelot[ARONDIGHT].PosY);
			lancelot[ARONDIGHT].Transform_.Concat();

			arondight_particle->Emitter_.Pos_.Point_Min_Max[1].y = 300.f;

			// update the collision box of lancelot
			Collision_.Update_Col_Pos(PosX - LANCELOT_SCALE, PosY - LANCELOT_SCALE,
				PosX + LANCELOT_SCALE, PosY + LANCELOT_SCALE);
		}
   
		else if (!lancelot[SLASH].cooldown)
		{
			currAttk = SLASH;
			charge_time = 0.5f; // charge time for player to prepare themselves

			// player is on the right hand side
			if (this->Get_Direction())
			{
				lancelot[SLASH].Start_Attack(this->PosX + -ATK_START_POINT.x, ATK_START_POINT.y);
				lancelot[SLASH].SetDir(false);
				angle = -200.0f;
				angle_offset = 2.0f;
			}
			else // player on the left hand side
			{
				lancelot[SLASH].Start_Attack(this->PosX + ATK_START_POINT.x, ATK_START_POINT.y);
				angle = 25.0f;
				angle_offset = -2.0f;
			}
			lancelot[SLASH].SetActive(false);
		}
		else
		{
			currAttk = STAB;
			lancelot[STAB].Start_Attack(this->PosX, this->PosY);

			// rotate the texture to the right direction
			if (Get_Direction() == RIGHT)
			{
				lancelot[STAB].Transform_.SetScale(-3.0f, 2.0f);
			}
			else
			{
				lancelot[STAB].Transform_.SetScale(3.0f, 2.0f);
			}
		}
	}

	// state machine for boss
	switch (currAttk)
	{
	case STAB: Stab(d, dt);
		break;
	case SLASH: Slash(d, dt);
		break;
	case MAD_ENHANCEMENT: Mad_Enhancement(dt);
		break;
	case ARONDIGHT: Arondight(d, dt);
		break;
	}

}

void Lancelot::Lancelot_Phase2(const float dt)
{
	phase = PHASE_2;

	if(M_E) // turns off mad enhancement if its activated
		Mad_Enhancement(dt); 
}

void Lancelot::Stab(Dragon& d, const float dt)
{
	lancelot[STAB].Projectile::Update(dt, ATTACK_SCALE, false, 0.0f); // move the stab projectile
	

	if (lancelot[STAB].GetDist() > 10.0f) // accelerate the stab once it past 10 pixels
	{
		const float stab_accel = 80.0f;
		AEVec2 new_velocity = lancelot[STAB].GetVelocity();
		new_velocity.x += stab_accel;
		lancelot[STAB].SetVelocity(new_velocity);

        if (!lancelot[STAB].GetCollided()) // check for collision
        {
            if (lancelot[STAB].Collision_.Dy_Rect_Rect(d.Collision_, lancelot[STAB].GetVelocity(), d.GetVelocity(), dt))
            {
                lancelot[STAB].SetCollided(true);
                d.Decrease_HP();
                d.PlayHit();
            }
        }
	}

	if (lancelot[STAB].GetDist() > 100.0f) // range of stab
	{
		lancelot[STAB].End_Attack();
		lancelot[STAB].SetVelocity(STAB_VELOCITY);
		current_action = IDLE;             // set behavior to idle
	}

		
}

void Lancelot::Slash(Dragon& d, const float dt)
{
	static bool second_slash = false;	 // checks if its the first or second slash
	
	// freeze lancelot for player to prepare
	while (charge_time > 0)
	{
		charge_time -= dt;
		return;
	}


	lancelot[SLASH].SetActive(true);
	lancelot[SLASH].Projectile::Update(dt, ATTACK_SCALE, false, angle += angle_offset);

	// checks for collision with player. Only collide once
	if (!lancelot[SLASH].GetCollided())
	{
		if (lancelot[SLASH].Collision_.Dy_Rect_Rect(d.Collision_, lancelot[SLASH].GetVelocity(), d.GetVelocity(), dt))
		{
			lancelot[SLASH].SetCollided(true);
			d.Decrease_HP();
			d.PlayHit();
		}
	}

	if (lancelot[SLASH].GetDist() > 200.0f) // range of slash
	{			 

		if (second_slash)
		{
			lancelot[SLASH].cooldown_timer = M_E ? 2.0f : 4.0f;  // shorter cooldown when berserked.
			lancelot[SLASH].End_Attack();						 // set active to false and reset variable
			lancelot[SLASH].SetVelocity(SLASH_VELOCITY);         // reset velocity of slash
			current_action = IDLE;                               // set behavior to idle
			second_slash = false;							     // reset second slash flag
			angle_offset = -angle_offset;
		}
		else  // reverse the velocity to make slash move backwards
		{
			AEVec2 reverse = {SLASH_VELOCITY.x, SLASH_VELOCITY.y};
			reverse.x = -reverse.x;
			reverse.y = -reverse.y;
			second_slash = true;
			lancelot[SLASH].SetVelocity(reverse);
			lancelot[SLASH].ResetDist();        // reset distance travled back to 0
			angle_offset = -angle_offset;
		}
	}

}

void Lancelot::Mad_Enhancement(const float dt)
{
	while (charge_time > 0) // freeze lancelot for 2 seconds
	{
		charge_time -= dt;
		return;
	}
	
	if (M_E) // mad enhancement is activated
	{
		this->SetVelocity(AEVec2 { 200.0f, 0.0f }); // return to original velocity
		idle_time = 1.0f;							// set the idle time back
		M_E = false;								// turn mad enhancement off
		
	}
	else
	{
		this->SetVelocity(AEVec2{ 350.0f, 0.0f });               // increase velocity
		lancelot[MAD_ENHANCEMENT].cooldown_timer = 20.0f;        // start cooldown
		lancelot[MAD_ENHANCEMENT].cooldown = true;               // cooldown duration
		idle_time = 0.0f;                                        // no idle interval
		M_E = true;												 // mad enhancement flag
		lancelot[MAD_ENHANCEMENT].ongoing_attack = false;
		current_action = MOVING;
		Set_Vulnerable(true);
	}

}

void Lancelot::Arondight(Dragon& d, const float dt)
{
	// play particle effects
	arondight_particle->UpdateEmission();
	arondight_particle->Turbulence(0.5f);

	arondight_particle->TransRamp_Exp();
	arondight_particle->Update(dt);

	
	while (charge_time > 0) // freeze lancelot for 2 seconds for player to prepare
	{
		charge_time -= dt;
		arondight_particle->Gravity(0.3f);
		return;
	}

	AEVec2 new_vector = lancelot[ARONDIGHT].Collision_.Get_Point(); // get the point at the top of the sword
	AEVec2 position = { this->PosX, this->PosY }; // position of lancelot. Used for collision detection
	HalfPlane side;
	float s = 0.0f;				  // sin of angle to rotate
	float c = 0.0f;				  // cosine of angle to rotate
	float radians = 0.0f;		  // randians version of degree to rotate
	float tempX = new_vector.x;   // old value of x
	new_vector.x -= PosX;		  // rotate collision point with using lancelot as the origin
	new_vector.y -= PosY;		  // rotate collision point with using lancelot as the origin
	lancelot[ARONDIGHT].Transform_.SetRotation(AERadToDeg(atan(new_vector.y / new_vector.x)));

	

	if (Get_Direction() == RIGHT)
	{
		radians = AEDegToRad(-angle_offset); // rotate the sword by -3 degrees
		side = OUTSIDE;					     // check the outisde half plane

		// reflect the texture
		lancelot[ARONDIGHT].Transform_.SetScale(-ARONDIGHT_SCALE.x, ARONDIGHT_SCALE.y); 
	}
	else
	{
		radians = AEDegToRad(angle_offset); // rotate the sword by 3 degrees
		side = INSIDE;					    // check the inside half plane
	}

	//arondight_particle->Emitter_.Pos_.Min_Max.Angle_ = radians;

	s = sin(radians);		  
	c = cos(radians);		  

	new_vector.x = new_vector.x * c - new_vector.y * s + PosX; // collision new point after rotation
	new_vector.y = tempX * s + new_vector.y * c + PosY;		   // collision new point after rotation


	lancelot[ARONDIGHT].PosX = new_vector.x * 0.5f + PosX * 0.5f; // new position of sword
	lancelot[ARONDIGHT].PosY = new_vector.y * 0.5f + PosY * 0.5f; // new position of sword
	lancelot[ARONDIGHT].Transform_.SetTranslate(lancelot[ARONDIGHT].PosX, lancelot[ARONDIGHT].PosY);	
	lancelot[ARONDIGHT].Transform_.Concat();
	lancelot[ARONDIGHT].Collision_.Update_Col_Pos(new_vector.x, new_vector.y); // update point coordinates for collision

	angle += angle_offset;

	// checks if the attack collided with player
	if (!lancelot[ARONDIGHT].GetCollided() && Player_Facing_Me (*this, d) )
	{
		if (lancelot[ARONDIGHT].Collision_.Line_Point(lancelot[ARONDIGHT].Collision_, d.Collision_, position, side))
		{
			d.Decrease_HP();
			lancelot[ARONDIGHT].SetCollided(true);
			d.PlayHit();
		}
	}

	// stop sword from rotating more than 140 degrees
	if (angle > 140.0f)
	{
		// start cooldown of attack and set active to false
		lancelot[ARONDIGHT].End_Attack();
		lancelot[ARONDIGHT].cooldown_timer = 10.0f;
		lancelot[ARONDIGHT].cooldown = true;
		charge_time = 2.0f;
		current_action = IDLE;
		
		// reset the scale of ARONDIGHT
		lancelot[ARONDIGHT].Transform_.SetScale(ARONDIGHT_SCALE.x, ARONDIGHT_SCALE.y);
	
	}

	//arondight_particle->Newton(new_vector, 15.f);
}

void Lancelot::Set_Face_Dir(const Dragon& d)
{
	if (d.PosX - this->PosX > 0)
	{
		this->Set_Direction(RIGHT);
		this->Transform_.SetScale(1.0f, 1.0f);
	}
	else
	{
		this->Set_Direction(LEFT);
		this->Transform_.SetScale(-1.0f, 1.0f);
	}
	this->Transform_.Concat();
}

void Lancelot::Set_Attk_Dir()
{
	if (Get_Direction() == RIGHT)            // set all attacks to go right
	{
		for (Boss_Attack& i : lancelot)
		{
			i.SetDir(true);
		}
	}
	else                                     // set all attacks to go left
	{
		for (Boss_Attack& i : lancelot)
		{
			i.SetDir(false);
		}
	}
}

void Lancelot::Init_Stab(void)
{
	lancelot.emplace_back(&attack_sprite, Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect));
	
	lancelot[STAB].SetVelocity(STAB_VELOCITY);       // velocity for slash
	lancelot[STAB].Transform_.SetScale(3.0f, 2.0f);  // determine the size of projectile
	lancelot[STAB].Transform_.Concat();
	lancelot[STAB].Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

void Lancelot::Init_Slash(void)
{
	lancelot.emplace_back(&attack_sprite, Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect));
	lancelot[SLASH].SetVelocity(SLASH_VELOCITY); // velocity for slash
	lancelot[SLASH].Transform_.SetScale(3.0f, 2.0f);       // determine the size of projectile
	lancelot[SLASH].Transform_.Concat();
	lancelot[SLASH].Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

void Lancelot::Init_Arondight(void)
{
	lancelot.emplace_back(&attack_sprite, Col_Comp(0.0f, 400.0f, Point));
	
	lancelot[ARONDIGHT].SetVelocity(AEVec2{ 600.0f, 600.0f });                        // velocity for slash
	lancelot[ARONDIGHT].Transform_.SetScale(ARONDIGHT_SCALE.x, ARONDIGHT_SCALE.y);    // determine the size of projectile
	lancelot[ARONDIGHT].Transform_.Concat();
	lancelot[ARONDIGHT].Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

void Lancelot::Dead(void)
{
	//change next state to next stage

	for (Boss_Attack& i : lancelot)
	{
		i.SetActive(false);
	}
	SetActive(false);
}

Lancelot::~Lancelot()
{
	lancelot.clear();
}

namespace
{

bool Player_Facing_Me(Lancelot& l, Dragon& d)
{
	Direction boss_face = l.Get_Direction();

	switch (boss_face)
	{
	case LEFT: return d.PosX - l.PosX < 0 ;
		break;
	case RIGHT: return d.PosX - l.PosX > 0;
		break;
	default: return false;
	}
}
}