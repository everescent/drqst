/* Start Header ************************************************************************/
/*!
\file       King_Arthur.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file creates, set the behaviour and rules for the last boss king arthur

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include <iostream>                // cout, endl (used for debugging)
#include <iomanip>                 // setprecision, setw (used for debugging)
#include "King_Arthur.h"           // header file
#include "AI_Data_Factory.h"       // to get the king arthur texture 
#include "GameStateManager.h"      // to switch to credits screen when king arthur is dead
#include "StageManager.h"          // to change state
#include "Camera.h"                // camShake
#include <vector>                  // std::vector

#define NUM_OF_SWORD 4
#define ACCEL        62.f
#define NUM_OF_MOBS  4

namespace 
{
	// enumeration for platform locations during phase2
	enum
	{
		TOP_LEFT,
		TOP_RIGHT,
		MIDDLE
	};

	struct LOCATION
	{
		AEVec2 min;
		AEVec2 max;
	};

	//------------------------------------------------------------------
	//
	//  CONST VARIABLES
	//
	//------------------------------------------------------------------
    const int HEALTH                          = 510;       // initial hp for king arthur
    const int PHASE2_HP                       = 500;       // phase 2 trigger
    const int PHASE3_HP                       = 300;       // phase 3 trigger
    const int interval                        = 80;        // interval for triple slash
    const int range_limit                     = 1250;      // range limit for slash
    const float START_POINT_X                 = 200.0f;    // spawn point of king arthur
    const float START_POINT_Y                 = -220.0f;   // spawn point of king arthur
    const float SLASH_SCALE                   = 30.0f;     // scale of slash
    const char limit                          = 5;         // num of king arthur attacks
		
    //------------------------------------------------------------------
    //
    //  GLOBAL VARIABLES
    //
    //------------------------------------------------------------------
    float left_boundary                       = -400;      // boundaries of charge attack
    float right_boundary                      = 400;       // boundaries of charge attack
	Sprite attack_sprite;                                  // texture for slash
	Sprite sword_sprite;                                   // texture for sword
	std::vector <Boss_Attack> arthur;                      // an array of boss attacks
	LOCATION teleport_location[3];                         // teleport locations of king arthur
	AEVec2 sword_pos[NUM_OF_SWORD]            = {};        // rotating sword postitions for phase 3
	AEVec2 ai_spawn_pos[NUM_OF_MOBS];                      // mob ai spawning location

	// function declarations
    bool Move_KA(const float dt, King_Arthur &ka, const Dragon &d); // move king arthur towards player
    void Set_Attack_Dir(King_Arthur &ka);                           // set the attack directions

}
/**************************************************************************************
//
// Converting constructor for lancelot. Takes in a pointer to its texture
// and used it to create a lancelot object
//
**************************************************************************************/
King_Arthur::King_Arthur(Sprite* texture)
    : Characters(texture, HEALTH,
        Col_Comp{ START_POINT_X - 30.0f, START_POINT_Y - 30.0f,
                  START_POINT_X + 30.0f, START_POINT_Y + 30.0f, Rect }),
      anim{WALK_ANIM + 1, 3, 5, [](std::vector <Range>& Init) -> void {
                                Init.push_back(Range{ 0.0f, 1.0f, 0.00f, 0.00f }); //Hit
                                Init.push_back(Range{ 0.0f, 1.0f, 0.33f, 0.33f }); //Idle
                                Init.push_back(Range{ 0.0f, 1.0f, 0.66f, 0.66f }); //Walk
    }},
    music{ 1, [](std::vector<std::string>& s) 
    {
        s.push_back(".//Audio/Hit_01.mp3");
    } },
        ka_phase{ PHASE_1 }, healing_effect{Effects_Get(KA_HEALING_PARTICLE)}, current_action{IDLE},
        sword_effect{ Effects_Get(KA_SWORD_PARTICLE) }, slash_effect{Effects_Get(KA_SLASH_PARTICLE)},
        phase_effect{ Effects_Get(PHASE_PARTICLE) }, active_mobs{ 0 }, spawn_mob {false},
        timer{ 2.f }, mob_timer{ 1.f }, behavior_swap{ 0 }, healing {false}
{
    PosX = START_POINT_X;                 // change king arthur coordinates to the location set
    PosY = START_POINT_Y;                 // change king arthur coordinates to the location set
    SetActive(true);                      // show him on screen
    Set_Direction(LEFT);                  // set king arthur to face left at the start
    SetVelocity({ 120, 0 });              // velocity for king arthur
    Transform_.SetTranslate(PosX, PosY);  // translate king arthur to its spawning location
    Transform_.SetScale(-1.0f, 1.0f);     // set king arthur to face right at the start
    Transform_.Concat();                  // spawn king arthur at the location set
    Reset_Idle_Time(1.0f);                // duration king arthur will idle
    Init_KA_Attacks();	                  // call initializer for king arthur move set
    Init_MobArray();                      // call initializer for mob array
    Init_Particle();                      // initializes the particle effects for KA

}
/**************************************************************************************
//
// Initializing king arthur attacks
//
**************************************************************************************/
void King_Arthur::Init_KA_Attacks(void)
{
    // stores the path to the texture
	const char* slash_tex = ".//Textures/KA_Slash.png";

	// initializing the sprites to with the attack textures
    attack_sprite = S_CreateSquare(SLASH_SCALE, slash_tex);
    attack_sprite.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    sword_sprite = S_CreateSquare(SLASH_SCALE, ".//Textures/Excalibur.png");
    sword_sprite.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

	arthur.reserve(limit); // reserve the amount of memory for king arthur attacks

	// add the single slash and triple slash
    for(char i = 0; i < limit-1; ++i) 
        arthur.emplace_back(&attack_sprite,
                         Col_Comp(START_POINT_X - SLASH_SCALE, START_POINT_Y - SLASH_SCALE,
                                  START_POINT_Y + SLASH_SCALE, START_POINT_Y + SLASH_SCALE, Rect));


    arthur.emplace_back(Boss_Attack()); // for dash attack since it does not need textures
    arthur.emplace_back(Boss_Attack()); // add the healing move, no textures needed

	// adds the 4 swords for the 3rd unique mechanic
    for (char i = 0; i < NUM_OF_SWORD; ++i)
    {
		// spin sword
		arthur.emplace_back(Boss_Attack(&sword_sprite, { 0.f, 0.f, Point })); 
        sword_pos[i].y = 200.f;
    }

	// initializing other variables in slash attacks
    for (char i = 0; i < limit - 1; ++i) 
    {
        arthur[i].SetVelocity(AEVec2{ 350.0f, 0.0f });                       // velocity for slash
        arthur[i].Transform_.SetScale(1.5f, 1.5f);                           // determine the size of projectile
        arthur[i].Transform_.SetTranslate( START_POINT_X, START_POINT_Y );   // set position of the slash
        arthur[i].Transform_.Concat();                                       // apply transformation
    }

	// position of the 4 swords in 3rd mechanic
    sword_pos[0].x = -200.f;
    sword_pos[1].x =  200.f;
    sword_pos[2].x =  300.f;
    sword_pos[3].x = -300.f;
}
/**************************************************************************************
//
// Initializing the basic mob ais
//
**************************************************************************************/
void King_Arthur::Init_MobArray(void)
{
    for (char i = 0; i < NUM_OF_MOBS; ++i)
    {
        if (i < 2)
        {
            // spawn the mobs at the left/right of the screen
            if (i % 2)
            {
                mobs.push_back(Create_Basic_AI(MAGE, { 356.f, 180.f }));
                mobs.push_back(Create_Basic_AI(ARCHER, { 200.f, -200 }));
            }
            else
            {
                mobs.push_back(Create_Basic_AI(MAGE, { -356.0f, 180.f }));
                mobs.push_back(Create_Basic_AI(ARCHER, { -200.f, -200.f }));
            }
        }

        ai_spawn_pos[0] = { 356.f,  180.f };
        ai_spawn_pos[1] = { 200.f, -200.f };
        ai_spawn_pos[2] = { -356.0f, 180.f };
        ai_spawn_pos[3] = { -200.f, -200.f };

        // do not render on screen yet
        mobs[i]->SetActive(false);

        // set the blend mode
        mobs[i]->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    }
}

void King_Arthur::Init_Particle(void)
{
    // intializing healing particle variables
    healing_effect->Emitter_.PPS_                    = 10;
    healing_effect->Emitter_.Dist_Min_               = 30.f;
    healing_effect->Emitter_.Vol_Max                 = 500;
    healing_effect->Emitter_.Direction_              = 270.0f;
    healing_effect->Emitter_.Particle_Rand_.Spread_  = 360;
    healing_effect->Emitter_.Conserve_               = 0.8f;
    healing_effect->Emitter_.Size_                   = 10.0f;
    healing_effect->Emitter_.Speed_                  = 5.0f;
    healing_effect->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
    healing_effect->Emitter_.Lifetime_               = 1.f;

    // initializing the sword particle for phase 3
    sword_effect->Emitter_.PPS_                      = 8;
    sword_effect->Emitter_.Dist_Min_                 = 0.f;
    sword_effect->Emitter_.Vol_Max                   = 2000;
    sword_effect->Emitter_.Direction_                = 90.0f;
    sword_effect->Emitter_.Conserve_                 = 0.8f;
    sword_effect->Emitter_.Size_                     = 10.0f;
    sword_effect->Emitter_.Speed_                    = 3.0f;
    sword_effect->Emitter_.Lifetime_                 = 4.f;

    // initializing the particle variables for phase transition
    phase_effect->Emitter_.PPS_                      = 20;
    phase_effect->Emitter_.Dist_Min_                 = 10.f;
    phase_effect->Emitter_.Vol_Max                   = 512;
    phase_effect->Emitter_.Direction_                = 90.0f;
    phase_effect->Emitter_.Particle_Rand_.Spread_    = 180;
    phase_effect->Emitter_.Conserve_                 = 0.8f;
    phase_effect->Emitter_.Size_                     = 15.0f;
    phase_effect->Emitter_.Speed_                    = 4.0f;
    phase_effect->Emitter_.Particle_Rand_.Sp_Rand_   = 3;
    phase_effect->Emitter_.Lifetime_                 = 0.2f;
    phase_effect->Emitter_.Color_.R = 0.8f;
    phase_effect->Emitter_.Color_.G = 0.0f;
    phase_effect->Emitter_.Color_.B = 0.0471f;

    // update the phsae box location
    phase_effect->Emitter_.Pos_.Point_Min_Max[0].y = Collision_.Get_MinPoint().y - 60;
    phase_effect->Emitter_.Pos_.Point_Min_Max[1].y = Collision_.Get_MaxPoint().y + 20;
    phase_effect->Emitter_.Pos_.Point_Min_Max[0].x = -40.f;
    phase_effect->Emitter_.Pos_.Point_Min_Max[1].x = 40.f;

    // update the particle color
    phase_effect->Emitter_.Color_.R = 130.f / 255.f;
    phase_effect->Emitter_.Color_.G = 224.f / 225.f;
    phase_effect->Emitter_.Color_.B = 170.f / 255.f;

    // initializing the slash particle for slash
    slash_effect[0]->Emitter_.PPS_                      = 10;
    slash_effect[0]->Emitter_.Vol_Max                   = 1000;
    slash_effect[0]->Emitter_.Direction_                = 90.0f;
    slash_effect[0]->Emitter_.Conserve_                 = 0.8f;
    slash_effect[0]->Emitter_.Size_                     = 10.0f;
    slash_effect[0]->Emitter_.Speed_                    = 3.0f;
    slash_effect[0]->Emitter_.Lifetime_                 = 0.5f;

    slash_effect[2] = new Particle_System(slash_effect[0]->Emitter_.pMesh_, {}, BOX);
    slash_effect[1] = new Particle_System(slash_effect[0]->Emitter_.pMesh_, {}, BOX);

    slash_effect[2]->Emitter_ = slash_effect[1]->Emitter_ = slash_effect[0]->Emitter_;
}

void King_Arthur::Update(Dragon &d, const float dt)
{
    // activate phase 2 once hp drops is 50%
    if (Get_HP() < PHASE2_HP && ka_phase & PHASE_1 && ! arthur[currAttk].ongoing_attack)
    {
        King_Arthur_Phase2(dt);
        Update_Particle(dt);    // update particle effects for king arthur
        return;
    }

    // activates phase 3 once hp drops to 30%
    else if (Get_HP() < PHASE3_HP && ka_phase & PHASE_2 && !arthur[currAttk].ongoing_attack)
    {
        King_Arthur_Phase3(dt);
        Update_Particle(dt);    // update particle effects for king arthur
        return;
    }

    if (behavior_swap == 3)
    {
        current_action = MOVING;  // switch state to moving
        behavior_swap = 0;        // reset the counter
    }

    if (this->Get_HP() <= 0)
    {
        current_action = DEAD; // king arthur has died
    }

    if (spawn_mob) // update the mobs that were spawned
    {
        char i = 0, mobs_dead = 0;

        if (mob_timer < 5.f)
        {
            active_mobs = static_cast <char> (mob_timer);
            mob_timer += dt;
        }

        for (;i < active_mobs; ++i)
        {
           (mobs[i]->IsActive()) ? 	mobs[i]->Update(d, dt) : ++mobs_dead;
        }
        if (mobs_dead == i)
        {
            spawn_mob   = false;
            mob_timer   = 1.f;
            active_mobs = 0;
        }
    }

    if (IsActive())
    {
        // fireball hit KA
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

                    healing = false;
                }

        // mega fire ball hit KA
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

                healing = false;
            }
        }

    
    }
   
    // switch between the boss states
    switch (current_action)
    {
    case IDLE:     Idle(d, dt);
                   anim.SetState(IDLE_ANIM);
        break;

    case MOVING:   Moving(d,dt);
                   anim.SetState(WALK_ANIM);
        break;

    case ATTACK:   Attack(d, dt);
                   anim.SetState(IDLE_ANIM);
        break;

    case DEAD:     Dead();
        break;

    default: break;
    }

    for (unsigned char i = 0; i < arthur.size(); ++i) // update cooldowns on attacks
            arthur[i].Update(dt);

    Update_Particle(dt);    // update particle effects for king arthur
    anim.Update(*Sprite_);  // update animation effects
}

void King_Arthur::Idle(const Dragon& d, const float dt)
{
    //changes state to attack once idling is finished, reset idle
    if (Get_Idle_Time() <= 0)
    {
        current_action = ATTACK;
        Reset_Idle_Time(1.0f);

        Set_Forward_Dir(d);
        Set_Attack_Dir(*this);
    }
    else
    {
        Decrease_Idle_Time(dt);
    }
    
}

void King_Arthur::Moving(const Dragon &d, const float dt)
{	
    static float move_duration = 3.0f; // duration KA moves

    Set_Forward_Dir(d);    // set KA to face player
    Move_KA(dt, *this, d); // move king arthur

    //change state to idle if move_duration is 0 or player reached, reset move_duration
    move_duration <= 0 || this->Get_Direction() == STAY ? 
    current_action = IDLE, move_duration = 3.0f : move_duration -= dt;

    Set_Attack_Dir(*this); // set attack directions
}

void King_Arthur::Attack(Dragon &d, const float dt)
{		
    //unique mechanic has the highest priority 
    if (! arthur[currAttk].ongoing_attack)
    {
        if (!(arthur[DASH].cooldown) && ka_phase & PHASE_1)
        {
            currAttk = DASH;
        }
        else if (!(arthur[HEAL].cooldown) && ka_phase & PHASE_2)
        {
            currAttk = HEAL;
        }

        else if (!(arthur[SPIN_SWORD].cooldown) && ka_phase & PHASE_3)
        {
            for (char i = 0; i < NUM_OF_SWORD; ++i)
            {
                arthur[SPIN_SWORD + i].Start_Attack(sword_pos[i].x, sword_pos[i].y);
                arthur[SPIN_SWORD + i].Transform_.SetRotation(0.0f);
                arthur[SPIN_SWORD + i].Transform_.SetTranslate(arthur[SPIN_SWORD + i].PosX, arthur[SPIN_SWORD + i].PosY);
                arthur[SPIN_SWORD + i].Transform_.Concat();
                arthur[SPIN_SWORD + i].ongoing_attack = false;

                PosX = 0.0f;
                PosY = sword_pos[0].y;
                Transform_.SetTranslate(PosX, PosY);
                Transform_.Concat();
                Collision_.Update_Col_Pos(PosX - 30.0f, PosY - 30.0f,  // min point
                                          PosX + 30.0f, PosY + 30.0f);	// max point

            }
            
            currAttk = SPIN_SWORD;
        }
        //followed by triple slash
        else if (!(arthur[TRIPLE_SLASH].cooldown))
            currAttk = TRIPLE_SLASH;
        else
        {
            currAttk = SINGLE_SLASH; //default attack for king arthur
            arthur[SINGLE_SLASH].Start_Attack(this->PosX, this->PosY);
        }

    }
    switch (currAttk)
    {
    case SINGLE_SLASH: Single_Slash(d, dt);
        break;
    case TRIPLE_SLASH: Triple_Slash(d, dt);
        break;
    case DASH: Dash_Attack(d, dt);
        break;
    case HEAL: Heal_and_Spawn(d, dt);
        break;
    case SPIN_SWORD: Spinning_Blades(d, dt);
        break;
    default: break;
    }


}

void King_Arthur::King_Arthur_Phase2(const float dt)
{
    Set_Vulnerable(false);

    // move king arthur to the middle of the screen
    PosX = 0.0f;
    PosY = START_POINT_Y;
    Transform_.SetTranslate(0.0f, START_POINT_Y);
    Transform_.Concat();
    Collision_.Update_Col_Pos(PosX - 30.0f, PosY - 30.0f,  // min point
        PosX + 30.0f, PosY + 30.0f);	// max point

    Off_Particles(); // render particles off screen
    Sprite_->SetAlphaTransBM(0.8f, 0.8f, AE_GFX_BM_BLEND);

    phase_effect->UpdateEmission();

    if (timer > 0)
    {
        timer -= dt;
        CamShake();
        return;
    }
    
    ka_phase = PHASE_2; // change to phase 2
    Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    

    // platform coordinates to teleport to
    teleport_location[TOP_RIGHT].max.x =  256.0f;
    teleport_location[TOP_RIGHT].max.y =  200.0f;
    
    teleport_location[MIDDLE].max.x    = -32.0f;
    teleport_location[MIDDLE].max.y    =  20.0f;

    teleport_location[TOP_LEFT].max.x  = -344.0f;
    teleport_location[TOP_LEFT].max.y  =  200.0f;

    current_action = ATTACK;
    Set_Vulnerable(true);
    timer = 2.f;
}

void King_Arthur::King_Arthur_Phase3(const float dt)
{
    Set_Vulnerable(false);

    // move king arthur to the middle of the screen
    PosX = 0.0f;
    PosY = START_POINT_Y;
    Transform_.SetTranslate(0.0f, START_POINT_Y);
    Transform_.Concat();
    Collision_.Update_Col_Pos(PosX - 30.0f, PosY - 30.0f,   // min point
                              PosX + 30.0f, PosY + 30.0f);	// max point

    Off_Particles(); // render particles off screen
    phase_effect->UpdateEmission();
    Sprite_->SetAlphaTransBM(0.8f, 0.8f, AE_GFX_BM_BLEND); // set ka to fade out

    if (timer > 0)
    {
        timer -= dt;
        CamShake();
        return;
    }

    ka_phase = PHASE_3;
    current_action = ATTACK;
    Set_Vulnerable(true);
    Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND); // set ka to original opaque

    // resettings the boundaries of king arthur
    left_boundary = -400; 
    right_boundary = 400; 

    //despawns all the mob
    for (auto& elem : mobs)
        elem->Set_HP(0);
}

void King_Arthur::Dash_Attack(Dragon &d, const float dt)
{	
    static Direction dash;           // direction KA should dash in

    if (!(arthur[DASH].ongoing_attack)) // called once to fix dash direction
    {
        dash = this->Get_Direction();
        arthur[DASH].ongoing_attack = true;
    }

    this->Set_Direction(dash);       // change back the dash direction

    this->SetVelocity({ 800, 0 }); // change king arthur's velocity to dash
    bool hit = Move_KA(dt, *this, d);         // move king arthur towards player

    // need check for collision against dragon
    if(this->Get_Direction() == STAY)
    {
        current_action = IDLE;           // switch current state to idle
        arthur[DASH].cooldown = true;       // start the cooldown of dash attk
        this->SetVelocity({ 120, 0 });   // reset the velocity of king arthur
        ++behavior_swap;    
        arthur[DASH].ongoing_attack = false;
        arthur[DASH].cooldown_timer = 10.0f; // cooldown of jump attack

        if(hit)
            d.Decrease_HP();
    }

    this->Transform_.Concat();


}

void King_Arthur::Single_Slash(Dragon &d, const float dt)
{
    slash_effect[0]->Emitter_.Pos_.Min_Max.Point_Max.x = arthur[SINGLE_SLASH].PosX + 8.f;
    slash_effect[0]->Emitter_.Pos_.Min_Max.Point_Min.x = arthur[SINGLE_SLASH].PosX - 8.f;
    slash_effect[0]->Emitter_.Pos_.Min_Max.Point_Max.y = arthur[SINGLE_SLASH].PosY + 35.f;
    slash_effect[0]->Emitter_.Pos_.Min_Max.Point_Min.y = arthur[SINGLE_SLASH].PosY - 35.f;
    slash_effect[0]->UpdateEmission();

    arthur[SINGLE_SLASH].Projectile::Update(dt, SLASH_SCALE, false, 0.f); // move the slash

    if(! arthur[SINGLE_SLASH].GetCollided()) // check for collision
    {
        if (arthur[SINGLE_SLASH].Collision_.Dy_Rect_Rect(d.Collision_, arthur[SINGLE_SLASH].GetVelocity(), d.GetVelocity(), dt))
        {
            d.Decrease_HP();
            arthur[SINGLE_SLASH].SetCollided(true);
            d.SetInvul(true);                   // Visual Feedback and make dragon invulnerable for awhile
        }
    }

    //disappears once it flys 500 unit away
    if (arthur[SINGLE_SLASH].GetDist() > range_limit || arthur[SINGLE_SLASH].PosX < -600 ||
        arthur[SINGLE_SLASH].PosX > 600)      
    {
        arthur[SINGLE_SLASH].End_Attack();

        current_action = IDLE;                       // set the behaviour to idle
        ++behavior_swap;
    }
    

}

void King_Arthur::Triple_Slash(Dragon &d, const float dt)
{
    const char ts_limit = limit - 1; // limit the loop to just the triple slashes
    
    if (!(arthur[TRIPLE_SLASH].ongoing_attack)) // sets the attack to start from KA
    {
        for (char i = 1; i < ts_limit; ++i)
        {
            arthur[i].SetPos(this->PosX, this->PosY);
        }
        arthur[TRIPLE_SLASH].SetActive(true);         // set the first attack to show
        arthur[TRIPLE_SLASH].ongoing_attack = true;   // attack is currently ongoing
    }


    //slashes 3 times at the player with an interval inbetween
    for (char i = 1; i < ts_limit; ++i)
    {		
        // release the 2nd and third projectile after the one in front travels a certain distance
        if (i > 1 && arthur[i-1].GetDist() > interval)
        {
            arthur[i].SetActive(true);
        }

        arthur[i].Projectile::Update(dt, SLASH_SCALE, false, 0.f); // update the pos of the slash

        // create particles if slash is active
        if (arthur[i].IsActive())
        {
            slash_effect[i - 1]->Emitter_.Pos_.Min_Max.Point_Max.x = arthur[i].PosX + 8.f;
            slash_effect[i - 1]->Emitter_.Pos_.Min_Max.Point_Min.x = arthur[i].PosX - 8.f;
            slash_effect[i - 1]->Emitter_.Pos_.Min_Max.Point_Max.y = arthur[i].PosY + 35.f;
            slash_effect[i - 1]->Emitter_.Pos_.Min_Max.Point_Min.y = arthur[i].PosY - 35.f;
            slash_effect[i - 1]->UpdateEmission();
        }
        
        // checks if it has collided with the player
        if (!arthur[i].GetCollided())
        {
            if (arthur[i].Collision_.Dy_Rect_Rect(d.Collision_, arthur[i].GetVelocity(), d.GetVelocity(), dt))
            {
                d.Decrease_HP();
                d.SetInvul(true);                   // Visual Feedback and make dragon invulnerable for awhile
                for (char j = 1; j < ts_limit; ++j)
                    arthur[j].SetCollided(true);    // set collided flag to true
            }
        }

        // makes projectiles disappear
        if (arthur[i].GetDist() > range_limit || arthur[i].PosX < -600 ||
            arthur[i].PosX > 600) 
        {
            arthur[i].SetActive(false); // disappears from screen
            arthur[i].ResetDist();      // reset the projectile distance traveled
            arthur[i].PosX = 0.0f;      // remove flicker

            if (i == 3) // last slash has completed
            {
                current_action                      = IDLE;     // change state to idle
                arthur[TRIPLE_SLASH].cooldown       = true;     // start cooldown		
                arthur[TRIPLE_SLASH].ongoing_attack = false;    // attack has complete
                arthur[TRIPLE_SLASH].cooldown_timer = 5.0f;     // cooldown duration
                ++behavior_swap;								// behavior counter

                for (char j = 1; j < ts_limit; ++j)
                    arthur[j].SetCollided(false);
            }
        }
    }

}


void King_Arthur::Heal_and_Spawn(Dragon &d, const float dt)
{
    UNREFERENCED_PARAMETER(d);
    //teleport to a platform, heal and spawn mobs

    if (!arthur[HEAL].ongoing_attack) // teleport once
    {
        //get a random number and teleport to the coresponding platform
        switch (Get_Random_Num(2))
        {
        case TOP_RIGHT: PosX            = teleport_location[TOP_RIGHT].max.x;
                        PosY            = teleport_location[TOP_RIGHT].max.y;
                        
                        left_boundary   = teleport_location[TOP_RIGHT].max.x - 90.0f;
                        right_boundary  = teleport_location[TOP_RIGHT].max.x + 140.0f;
            break;

        case MIDDLE:    PosX            = teleport_location[MIDDLE].max.x;
                        PosY            = teleport_location[MIDDLE].max.y;
                                        
                        left_boundary   = teleport_location[MIDDLE].max.x - 130.0f;
                        right_boundary  = teleport_location[MIDDLE].max.x + 130.0f;
            break;

        case TOP_LEFT: PosX             = teleport_location[TOP_LEFT].max.x;
                       PosY             = teleport_location[TOP_LEFT].max.y;
                                       
                       left_boundary    = teleport_location[TOP_LEFT].max.x - 40.0f;
                       right_boundary   = teleport_location[TOP_LEFT].max.x + 110.0f;
            break;

        default: break;
        }
        Transform_.SetTranslate(PosX, PosY);
        Transform_.Concat();
        Collision_.Update_Col_Pos(PosX - 30.0f, PosY - 30.0f,  // min point
                                  PosX + 30.0f, PosY + 30.0f);	// max point
        healing = true;
        arthur[HEAL].ongoing_attack = true;
        spawn_mob = true;

        healing_effect->Emitter_.Pos_.Min_Max.Point_Min.x = PosX - 50.f;
        healing_effect->Emitter_.Pos_.Min_Max.Point_Min.y = PosY - 50.f;
        healing_effect->Emitter_.Pos_.Min_Max.Point_Max.x = PosX + 50.f;
        healing_effect->Emitter_.Pos_.Min_Max.Point_Max.y = PosY + 50.f;

        for (char i = 0; i < NUM_OF_MOBS; ++i)
        {
            mobs[i]->SetActive(true);

            if (! (i %2))
            {
                auto* temp = static_cast <Mage*> (mobs[i]);
                temp->Renew_Mage({ai_spawn_pos[i].x, ai_spawn_pos[i].y});
            }
            else
            {
                mobs[i]->PosX = ai_spawn_pos[i].x;
                mobs[i]->PosY = ai_spawn_pos[i].y;
            }
        }
    }
    
    if (healing && Get_HP() <= HEALTH)
    {
        // heals every 0.5 seconds
        static float heal_timer = 0.5f;
        heal_timer <= 0 ? Set_HP(Get_HP() + 10), heal_timer = 0.5f : heal_timer -= dt;
    
        healing_effect->UpdateEmission();
    }
    else
    {
        arthur[HEAL].cooldown       = true;
        arthur[HEAL].ongoing_attack = false;		
        arthur[HEAL].cooldown_timer = 20.0f;
        ++behavior_swap;
    }

}

void King_Arthur::Spinning_Blades(Dragon &d, const float dt)
{    
    int i = 0;
    //float x, y;
    static float angle, angle_end;

    for (; i < NUM_OF_SWORD; ++i)
    {
        if (arthur[SPIN_SWORD + i].IsActive())
        {   //rotate the sword until it is facing the player
            if (!arthur[SPIN_SWORD + i].ongoing_attack)
            {
                AEVec2 pos{ d.PosX - arthur[SPIN_SWORD + i].PosX, d.PosY - arthur[SPIN_SWORD + i].PosY };
                AEVec2 nrm;
                AEVec2Normalize(&nrm, &pos);

                // rotate the sword till the tip faces the player
                angle_end = std::asin(nrm.x);
                angle_end = AERadToDeg(angle_end);
                angle_end = angle_end + 180.0f;

                arthur[SPIN_SWORD + i].ongoing_attack = true;
                angle = 0;
            }

            // update the particle effects and emiter position
            sword_effect->Emitter_.Pos_.Point.x = arthur[SPIN_SWORD + i].PosX;
            sword_effect->Emitter_.Pos_.Point.y = arthur[SPIN_SWORD + i].PosY;
            sword_effect->UpdateEmission();

            if (angle != angle_end)
            {
                // offset
                if (angle_end - angle < 10)
                {
                    angle = angle_end;
                    arthur[SPIN_SWORD + i].SetVelocity({ d.PosX - arthur[SPIN_SWORD + i].PosX, d.PosY - arthur[SPIN_SWORD + i].PosY });
                    arthur[SPIN_SWORD + i].Transform_.SetRotation(angle);
                    arthur[SPIN_SWORD + i].Transform_.Concat();
                }
                else
                {
                    arthur[SPIN_SWORD + i].Transform_.SetRotation(angle += 10);
                    arthur[SPIN_SWORD + i].Transform_.Concat();
                    return;
                }
            }
            AEVec2 new_velocity = arthur[SPIN_SWORD + i].GetVelocity();

            arthur[SPIN_SWORD + i].PosX += arthur[SPIN_SWORD + i].GetVelocity().x * dt;
            arthur[SPIN_SWORD + i].PosY += arthur[SPIN_SWORD + i].GetVelocity().y * dt;
            AEVec2Scale(&new_velocity, &new_velocity, ACCEL);
            AEVec2Scale(&new_velocity, &new_velocity, dt);
            arthur[SPIN_SWORD + i].SetVelocity(new_velocity);
            arthur[SPIN_SWORD + i].Transform_.SetTranslate(arthur[SPIN_SWORD + i].PosX, arthur[SPIN_SWORD + i].PosY);
            arthur[SPIN_SWORD + i].Transform_.Concat();
            arthur[SPIN_SWORD + i].Collision_.Update_Col_Pos(arthur[SPIN_SWORD + i].PosX, arthur[SPIN_SWORD + i].PosY);

            if (!arthur[SPIN_SWORD + i].GetCollided() && arthur[SPIN_SWORD + i].Collision_.Point_Rect(d.Collision_, arthur[SPIN_SWORD + i].Collision_))
            {
                d.Set_Vulnerable(true);
                d.Decrease_HP();
                arthur[SPIN_SWORD + i].SetCollided(true);
                d.SetInvul(true);                            // Visual Feedback and make dragon invulnerable for awhile
            }

            // checks if the sword reaches the ground, or fly out of screen
            if (arthur[SPIN_SWORD + i].PosY < -250.f || arthur[SPIN_SWORD + i].PosX < -600 ||
                arthur[SPIN_SWORD + i].PosX > 600)
            {
                arthur[SPIN_SWORD + i].SetActive(false);       // make stab disappaer
                arthur[SPIN_SWORD + i].ResetDist();            // reset distance traveled back to 0
                arthur[SPIN_SWORD + i].SetCollided(false);     // reset collided flag

                if (i == NUM_OF_SWORD - 1)
                {
                    current_action = IDLE;
                    arthur[SPIN_SWORD].cooldown_timer = 10.0f;
                    arthur[SPIN_SWORD].cooldown = true;        // start cooldown
                    arthur[SPIN_SWORD].ongoing_attack = false; // attack animation has concluded
                    arthur[SPIN_SWORD + 1].ongoing_attack = false; // attack animation has concluded

                    PosX = 0.0f;
                    PosY = START_POINT_Y;
                    Transform_.SetTranslate(PosX, PosY);
                    Transform_.Concat();
                    Collision_.Update_Col_Pos(PosX - 30.0f, PosY - 30.0f,  // min point
                                              PosX + 30.0f, PosY + 30.0f);	// max point
                    ++behavior_swap;

                }
            }

            return;
        }
    }
}

void King_Arthur::Render(void)
{
    for (unsigned char i = 0; i < arthur.size(); ++i)
    {
        if (arthur[i].IsActive())
            arthur[i].Render();
    }
    for (char i = 0; i < active_mobs; ++i)
    {
        if (mobs[i]->IsActive())
            mobs[i]->Render();
    }

    GameObject::Render(); // render king arthur on screen

    phase_effect->Render();

    for (auto& i : slash_effect)
    {
        i->Render();
    }
    
    // rendering the particle systems
    switch (ka_phase)
    {
    case PHASE_2: healing_effect->Render();
        break;
    case PHASE_3: sword_effect->Render();
        break;
    default: break;
    }
}

void King_Arthur::Update_Particle(const float dt)
{
    // update particle behaviour
    for (auto& i : slash_effect)
    {
        // only update if there are active particles
        if (i->GetParticleCount())
        {
            i->Turbulence(0.4f);
            i->Drag(0.5f);
            i->ColorRamp_Life();
            i->TransRamp_Exp();
            i->Update(dt);
        }
    }

    if (phase_effect->GetParticleCount())
    {
        phase_effect->Turbulence(0.4f);
        phase_effect->Force(0.5f, false, true);        //Simulate an upward force
        phase_effect->Newton({ 10.f, 400.0f }, 2.8f);
        phase_effect->Update(dt);
    }

    // updates healing effects if there are active particles
    if (healing_effect->GetParticleCount())
    {
        healing_effect->Turbulence(0.4f);
        sword_effect->ColorRamp_Life();
        healing_effect->TransRamp_Exp();
        healing_effect->Newton({ PosX, PosY }, 0.3f);
        healing_effect->Update(dt);
    }

    // update rotating sword effects if there are active particles
    if (sword_effect->GetParticleCount())
    {
        sword_effect->Turbulence(0.4f);
        sword_effect->Force(0.1f, false, true);
        sword_effect->ColorRamp_Life();
        sword_effect->TransRamp_Exp();
        sword_effect->Update(dt);
    }
}

void King_Arthur::Dead(void)
{
    SetActive(false);
    // play some animation. Camera shake?

    for (char i = 0; i < active_mobs; ++i)
        mobs[i]->SetActive(false);

    for (auto& elem : arthur)
    {
        elem.SetActive(false);
    }
}

void King_Arthur::Set_Forward_Dir(const Dragon& d)
{
    if ((d.PosX - this->PosX) > 0)
    {
        this->Set_Direction(RIGHT);
        this->Transform_.SetScale(1.0f, 1.0f); // reflect the texture to face player
    }
    else
    {
        this->Set_Direction(LEFT);
        this->Transform_.SetScale(-1.0f, 1.0f); // reflect the texture to face player
    }
    this->Transform_.Concat();
}

void King_Arthur::Off_Particles(void)
{
    // remove every slash effect particle from screen
    for (auto &elem : slash_effect)
    {
        elem->Off_Emitter();
    }

    // remove every healing effect particle from screen
    healing_effect->Off_Emitter();

    // remove every sword effect particle from screen
    sword_effect->Off_Emitter();
}

// return the current phase
BOSS_PHASE King_Arthur::Get_Phase(void) const
{
    return ka_phase;
}

std::vector <Characters*>& King_Arthur::Get_Mobs(void)
{
    return mobs;
}

void King_Arthur::Mute(void)
{
    music.SetVolume(0, 0.0f);
    music.SetPause(0, true);
}

void King_Arthur::Unmute(void)
{
    music.SetVolume(0, 1.0f);
    music.SetPause(0, false);
}

King_Arthur::~King_Arthur(void)
{
    // delete the mobs that was used 
    for (auto& elem : mobs)
        delete elem;
    
    // clear the mob and arthur attack vectors for future usage
    mobs.clear();
    arthur.clear();

    attack_sprite.~Sprite();  // free texture for slash
    sword_sprite.~Sprite() ;  // free texture for sword
    
    Off_Particles();              // remove particles from screen
    phase_effect->Off_Emitter(); // render all the phase particle effects from the screen

    left_boundary = -400.0f;
    right_boundary = 400.0f;

    // delete the slash that were new 
    delete slash_effect[1];
    delete slash_effect[2];
}


namespace
{
    bool Move_KA(const float dt, King_Arthur &ka, const Dragon &d)
    {
        if (ka.PosX < right_boundary && ka.PosX > left_boundary)
        {
            if (ka.Get_Direction() == RIGHT) 
            {
                ka.PosX += ka.GetVelocity().x * dt; // move ka to the right
            }
            else if (ka.Get_Direction() == LEFT) 
            {
                ka.PosX -= ka.GetVelocity().x * dt; // move KA to the left			
            }

            (void)ka.Transform_.SetTranslate(ka.PosX, ka.PosY);
            ka.Collision_.Update_Col_Pos(ka.PosX - 30.0f, ka.PosY - 30.0f,  // min point
                                         ka.PosX + 30.0f, ka.PosY + 30.0f);	// max point
        }

        else
        {
            ka.Set_Direction(STAY);

            // snap king arthur
            ka.PosX > right_boundary ? ka.PosX -= 5.0f : ka.PosX += 5.0f;
        }

        ka.Transform_.Concat();

        if (ka.Collision_.Dy_Rect_Rect(d.Collision_, d.GetVelocity(), ka.GetVelocity(), dt))
        {
            ka.Set_Direction(STAY);
            return true;
        }

        return false;
    }

    void Set_Attack_Dir(King_Arthur & ka)
    {
        
        if (ka.Get_Direction() == RIGHT) // set all attacks to go right
        {
            for (int i = 0; i < limit; ++i)
            {
                arthur[i].SetDir(true);
                arthur[i].Transform_.SetScale(-1.f, 1.f);
                arthur[i].Transform_.Concat();
            }
        }
        else if (ka.Get_Direction() == LEFT) // set all attacks to go left
        {
            for (int i = 0; i < limit; ++i)
            {
                arthur[i].SetDir(false);
                arthur[i].Transform_.SetScale(1.f, 1.f);
                arthur[i].Transform_.Concat();
            }
        }
    }
}

