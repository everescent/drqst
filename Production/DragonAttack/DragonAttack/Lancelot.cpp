#include "Lancelot.h"
#include "Boss_States.h"
#include <iostream>
#include <cmath>
#include <vector>

/*
@TODO:
slash coordinates. Swing top down, down up. Has an interval of 1 second i guess
Throw Weapon : but unavailable during mad enhancement
ARONDIGHT : phase 2 mechanic

collision with fireball and lancelot
*/


namespace
{
	enum Lancelot_Moveset
	{
		STAB,
		SLASH,
		MAD_ENHANCEMENT,
		ARONDIGHT
	};
	
	std::vector<Boss_Attack> lancelot; // array to store lancelot attack

	Boss_Action_State current_action = MOVING; // lancelot current action

	const int HEALTH = 1500; // health that lancelot start with
	const int PHASE2_HP = 750; // phase 2 trigger

	const char limit = 4; // num of lancelot attacks

	float idle_time          = 1.0f; // idling time for lancelot
	const float ATTACK_RANGE = 100.0f;
	const float ATTACK_SCALE = 20.0f;
	static Lancelot_Moveset currAttk = STAB;

	// variables for slash and arondight
	const AEVec2 ATK_START_POINT {-100.0f, -30.0f}; 
	static float angle = 25.0f;
	static float angle_offset = 2.0f;

	const AEVec2 STARTING_POINT{ 200.0f, -200.0f };
}

Lancelot::Lancelot(void)
	: Characters(S_CreateSquare(60.0f, ".//Textures/lancelot.jpg"),
	  HEALTH, { 0.0f, 0.0f, 5.0f, 5.0f, Rect }),  M_E{false}
{
	(void) this->Transform_.SetTranslate(STARTING_POINT.x, STARTING_POINT.y); // spawn lancelot at this location
	this->Transform_.Concat();
	this->SetPos( 200.0f , Start_Pos_Y);    // update lancelot current coordinates
	this->SetActive(true);					// spawn lancelot
	this->Set_Direction(LEFT);				// face left
	this->SetVelocity({ 300.0f, 0.0f });    // velocity for lancelot
	this->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	this->Reset_Idle_Time(idle_time);
	this->Init();							// initialize the attacks lancelot have
}

void Lancelot::Init()
{
	//initialize lancelot attacks here
	const char* stab = ".//Textures/arondight.png";
	const char* slash = ".//Textures/lancelot_slash.png";
	const char* tex = ".//Textures/arondight.png";

	lancelot.reserve(limit);

	lancelot.emplace_back(Boss_Attack(S_CreateSquare(ATTACK_SCALE, slash), // for stab
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect))); 

	lancelot.emplace_back(Boss_Attack(S_CreateSquare(ATTACK_SCALE, stab), // for slash
		Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect)));

	lancelot.emplace_back(Boss_Attack());                                // mad enhancement

	lancelot.emplace_back(Boss_Attack(S_CreateSquare(ATTACK_SCALE, tex), // arondight
		Col_Comp(0.0f, 400.0f, Point))); 

	Init_Stab();
	Init_Slash();
	Init_Arondight();

	lancelot[MAD_ENHANCEMENT].cooldown_timer = 10.0f;  // prevent unique mechanic from activating at the start of fight
	
	seed_initializer(); // initializes the seed for rng purposes

}

void Lancelot::Idle(const Dragon& d, const float dt)
{
	// change current_action to attack and idle to 0 once idling is over
	if (Get_Idle_Time() <= 0)
	{
		current_action = abs(d.PosX - this->PosX) <= ATTACK_RANGE ? ATTACK : MOVING;
		Reset_Idle_Time(idle_time);
		Set_Attk_Dir();
	}
	else
		Decrease_Idle_Time(dt);
}

void Lancelot::Moving(const Dragon &d, const float dt)
{
	if (Get_Direction() == RIGHT)     
	{
		PosX += GetVelocity().x * dt; // move lancelot to the right
	}
	else if (Get_Direction() == LEFT) 
	{
		PosX -= GetVelocity().x * dt; // move lancelot to the left
	}

	(void)Transform_.SetTranslate(PosX, PosY);
	Transform_.Concat();
	
	//change state to attack once move_duration is 0, reset move_duration
	if (abs(d.PosX - this->PosX) <= ATTACK_RANGE)
	{
		current_action = ATTACK;
		Set_Attk_Dir();
	}
}

void Lancelot::Update(Dragon &d, float dt)
{
	
	if (this->Get_HP() < PHASE2_HP && phase == PHASE_1)
	{
		Lancelot_Phase2(); // change to phase 2
	}
	else if (this->Get_HP() <= 0)
	{
		current_action = DEAD;
	}

	if (d.PosX - this->PosX > 0)
	{
		this->Set_Direction(RIGHT);
	}
	else
	{
		this->Set_Direction(LEFT);
	}

	if (M_E) // off mad enhancement state
	{
		if (lancelot[MAD_ENHANCEMENT].cooldown_timer < 10)
			this->Mad_Enhancement();         
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
}

void Lancelot::Attack(Dragon &d, const float dt)
{
	if (!lancelot[currAttk].ongoing_attack)
	{
	/*	if(phase == PHASE_1 && ! lancelot[MAD_ENHANCEMENT].cooldown)
			currAttk = MAD_ENHANCEMENT;

		else  if (phase == PHASE_2 && !lancelot[ARONDIGHT].cooldown)
		{
			currAttk = ARONDIGHT;
			//lancelot[ARONDIGHT].SetDir(true);
			angle = -90.0f;
			angle_offset = 2.0f;
		}
   
		else*/ if (!lancelot[SLASH].cooldown)
		{
			currAttk = SLASH;

			if (this->Get_Direction())
			{
				lancelot[SLASH].Start_Attack(this->PosX + -ATK_START_POINT.x, ATK_START_POINT.y);
				lancelot[SLASH].SetDir(false);
				angle = -200.0f;
				angle_offset = 2.0f;
			}
			else
			{
				lancelot[SLASH].Start_Attack(this->PosX + ATK_START_POINT.x, ATK_START_POINT.y);
				angle = 25.0f;
				angle_offset = -2.0f;
			}
		}
		else
		{
			currAttk = STAB;
			lancelot[STAB].Start_Attack(this->PosX, this->PosY);
		}
	}

	switch (currAttk)
	{
	case STAB: Stab(d, dt);
		break;
	case SLASH: Slash(d, dt);
		break;
	case MAD_ENHANCEMENT: Mad_Enhancement();
		break;
	case ARONDIGHT: Arondight(d, dt);
		break;
	}

}

void Lancelot::Lancelot_Phase2(void)
{
	phase = PHASE_2;
	M_E = false;
}

void Lancelot::Stab(Dragon& d, const float dt)
{
	UNREFERENCED_PARAMETER(dt);

	lancelot[STAB].Projectile::Update(ATTACK_SCALE);

	if (!lancelot[STAB].GetCollided())
	{
		if (lancelot[STAB].Collision_.Dy_Rect_Rect(d.Collision_, lancelot[STAB].GetVelocity(), d.GetVelocity(), dt))
		{
			lancelot[STAB].SetCollided(true);
			d.Decrease_HP();
		}
	}

	if (lancelot[STAB].GetDist() > 100.0f) // range of stab
	{
		lancelot[STAB].End_Attack();
		current_action = IDLE;             // set behavior to idle
	}

		
}

void Lancelot::Slash(Dragon& d, const float dt)
{
	UNREFERENCED_PARAMETER(dt);
	static float slash_interval = 0.5f;
	static bool second_slash = false;

	lancelot[SLASH].Projectile::Update(ATTACK_SCALE,false , angle += angle_offset);

	if (!lancelot[SLASH].GetCollided())
	{
		if (lancelot[SLASH].Collision_.Dy_Rect_Rect(d.Collision_, lancelot[SLASH].GetVelocity(), d.GetVelocity(), dt))
		{
			lancelot[SLASH].SetCollided(true);
			d.Decrease_HP();
		}
	}

	if (lancelot[SLASH].GetDist() > 200.0f) // range of slash
	{			 
		AEVec2 reverse = lancelot[SLASH].GetVelocity();
		if (second_slash)
		{
			lancelot[SLASH].cooldown_timer = M_E ? 2.0f : .0f; // shorter cooldown when berserked.
			lancelot[SLASH].End_Attack();
			reverse.x = -reverse.x;
			reverse.y = -reverse.y;
			lancelot[SLASH].SetVelocity(reverse);

			current_action = IDLE;          // set behavior to idle
			second_slash = false;
			angle_offset = -angle_offset;
		}
		else
		{
			reverse.x = -reverse.x;
			reverse.y = -reverse.y;
			second_slash = true;
			lancelot[SLASH].SetVelocity(reverse);
			lancelot[SLASH].ResetDist();        // reset distance travled back to 0
			angle_offset = -angle_offset;
		}
	}

}

void Lancelot::Mad_Enhancement()
{
	if (M_E) // mad enhancement is activated
	{
		this->SetVelocity(AEVec2 { 120.0f, 0.0f }); // return to original velocity
		idle_time = 1.0f;							// set the idle time back
		M_E = false;								// turn mad enhancement off
		std::cout << "end\n";
	}
	else
	{
		this->SetVelocity(AEVec2{ 350.0f, 0.0f });               // increase velocity
		lancelot[MAD_ENHANCEMENT].cooldown_timer = 20.0f;        // start cooldown
		lancelot[MAD_ENHANCEMENT].cooldown = true;               // cooldown duration
		idle_time = 0.0f;                                        // no idle interval
		M_E = true;												 // mad enhancement flag
		std::cout << "start\n";
	}

}

void Lancelot::Arondight(Dragon& d, const float dt)
{
	UNREFERENCED_PARAMETER(d);

	static float charge_time;


	if ( ! lancelot[ARONDIGHT].ongoing_attack)
	{
		this->PosX = 0.0f;
		this->Transform_.SetTranslate(PosX, PosY);
		this->Transform_.Concat();

		charge_time = 2.0f;
		angle = -90.0f;
		lancelot[ARONDIGHT].Collision_.Update_Col_Pos(400.0f, 0.0f);
		lancelot[ARONDIGHT].SetPos(0.0f, ATK_START_POINT.y +200);
		lancelot[ARONDIGHT].Transform_.SetTranslate(lancelot[ARONDIGHT].PosX, lancelot[ARONDIGHT].PosY);
		lancelot[ARONDIGHT].Transform_.SetRotation(-90.0f);
		lancelot[ARONDIGHT].Transform_.Concat();
		lancelot[ARONDIGHT].SetActive(true);
		lancelot[ARONDIGHT].ongoing_attack = true;

	}



	while (charge_time > 0)
	{
		charge_time -= dt;
		return;
	}

	lancelot[ARONDIGHT].Projectile::Update(ATTACK_SCALE);
	lancelot[ARONDIGHT].Transform_.SetRotation(angle += angle_offset);
	lancelot[ARONDIGHT].Transform_.Concat();
	AEVec2 position = { this->PosX, this->PosY };


	if ( ! lancelot[ARONDIGHT].GetCollided() && lancelot[ARONDIGHT].Collision_.Line_Point(lancelot[ARONDIGHT].Collision_, d.Collision_, position))
	{
		d.Decrease_HP();
		lancelot[ARONDIGHT].SetCollided(true);
	}

	AEVec2 new_point = lancelot[ARONDIGHT].Collision_.Get_Point();

	if (lancelot[ARONDIGHT].GetDir())
	{
		new_point.x += lancelot[ARONDIGHT].GetVelocity().x * dt;
		new_point.y += lancelot[ARONDIGHT].GetVelocity().y * dt;
	}
	else
	{
		float radians = (float)(20 * 3.14159265 / 180.0);

		float s = sin(radians);
		float c = cos(radians);
		
		new_point.x = new_point.x * c - new_point.y * s;
		new_point.y = new_point.x * s + new_point.y * c;
		
	}

	lancelot[ARONDIGHT].Collision_.Update_Col_Pos(new_point.x, new_point.y);

	if (lancelot[ARONDIGHT].GetDist() > 600.0f)
	{
		lancelot[ARONDIGHT].End_Attack();
		lancelot[ARONDIGHT].cooldown_timer = 0.0f;
	}


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
	
	lancelot[STAB].PosY = STARTING_POINT.y;
	lancelot[STAB].SetVelocity(AEVec2{ 500.0f, 0.0f }); // velocity for slash
	lancelot[STAB].Transform_.SetScale(3.0f, 2.0f);     // determine the size of projectile
	lancelot[STAB].Transform_.Concat();
	lancelot[STAB].Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

void Lancelot::Init_Slash(void)
{
	lancelot[SLASH].PosY = ATK_START_POINT.y;
	lancelot[SLASH].SetVelocity(AEVec2{ 20.0f, 500.0f }); // velocity for slash
	lancelot[SLASH].Transform_.SetScale(3.0f, 2.0f);       // determine the size of projectile
	lancelot[SLASH].Transform_.Concat();
	lancelot[SLASH].Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

void Lancelot::Init_Arondight(void)
{
	lancelot[ARONDIGHT].SetVelocity(AEVec2{ 600.0f, 600.0f }); // velocity for slash
	lancelot[ARONDIGHT].Transform_.SetScale(11.5f, 3.0f);       // determine the size of projectile
	

	lancelot[ARONDIGHT].Transform_.Concat();
	lancelot[ARONDIGHT].Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

void Lancelot::Dead(void)
{
	//change next state to next stage

	for (Boss_Attack& i : lancelot)
	{
		i.SetActive(false);
	}
}

Lancelot::~Lancelot()
{
	lancelot.clear();
}

