/* Start Header ************************************************************************/
/*!
\file       Lancelot.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file creates, set the behaviour and rules for the first boss Lancelot

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Lancelot.h"      // header file
#include "Collision.h"     // point and line collision
#include "Camera.h"        // camShake
#include <iostream>        // cout, endl ( used for debugging)
#include <cmath>           // cos, sin
#include <vector>          // std::vector


// use to store global variables
namespace
{
    // enumeration for lancelot attacks
    enum Lancelot_Moveset
    {
        STAB,
        SLASH,
        MAD_ENHANCEMENT,
        ARONDIGHT
    };

    std::vector<Boss_Attack> lancelot;  // array to store lancelot attack
    
    //------------------------------------------------------------------
    //
    //  CONST VARIABLES
    //
    //------------------------------------------------------------------
    const int    HEALTH           = 800;                // health that lancelot start with
    const int    PHASE2_HP        = 400;                // phase 2 trigger
    const float  LANCELOT_SCALE   = 60.0f;              // lancelot size
    const float  ATTACK_RANGE     = 150.0f;             // range of attack of lancelot
    const float  ATTACK_SCALE     = 20.0f;              // scale of attacks
    const char   limit            = 4;                  // num of lancelot attacks
    const AEVec2 ATK_START_POINT{ -100.0f, -140.0f };   // starting point of slash attacks
    const AEVec2 ARONDIGHT_SCALE{ 11.5f, 3.0f };        // scale of arondight
    const AEVec2 STARTING_POINT{ 200.0f, -260.0f };     // lancelot spawn location
    const AEVec2 SLASH_VELOCITY{ 20.0f, 500.0f };       // velocity of slash
    const AEVec2 STAB_VELOCITY{ 20.0f, 0.0f };          // velocity for stab

    //------------------------------------------------------------------
    //
    //  GLOBAL VARIABLES
    //
    //------------------------------------------------------------------
    float idle_time           = 1.0f;   // idling time for lancelot
    Lancelot_Moveset currAttk = STAB;   // currnet attack lancelot is using
    float  angle              = 25.0f;  // angle for rotation
    float  angle_offset       = 2.0f;   // offset of angle per frame
    float  charge_time;                 // duration lancelot use to charge his attacks
    Sprite attack_sprite;               // texture for attacks

    // checks if the player if facing lancelot
    bool Player_Facing_Me(Lancelot&, Dragon&);
}

/**************************************************************************************
//
// Converting constructor for lancelot. Takes in a pointer to its texture
// and used it to create a lancelot object
//
**************************************************************************************/
Lancelot::Lancelot(Sprite* texture)
    : Characters(texture,
        HEALTH,  Col_Comp{STARTING_POINT.x - LANCELOT_SCALE, STARTING_POINT.y - LANCELOT_SCALE,
                          STARTING_POINT.x + LANCELOT_SCALE, STARTING_POINT.y + LANCELOT_SCALE, Rect}),
    M_E{ false     }, arondight_particle{ Effects_Get(ARONDIGHT_PARTICLE) }, me_particle{Effects_Get(ME_PARTICLE)},
    phase{ PHASE_1 }, phase_particle    { Effects_Get(PHASE_PARTICLE)     }, timer{2.f}, current_action{IDLE},
    anim{ WALK_ANIM + 1, 5, 5, [](std::vector <Range>& Init) -> void {
    Init.push_back(Range{ 0.0f, 1.0f, 0.0f, 0.0f }); //Hit
    Init.push_back(Range{ 0.0f, 1.0f, 0.2f, 0.2f }); //Idle
    Init.push_back(Range{ 0.0f, 1.0f, 0.4f, 0.4f }); //Walk
    Init.push_back(Range{ 0.0f, 1.0f, 0.6f, 0.6f }); //Before dash
    Init.push_back(Range{ 0.0f, 1.0f, 0.8f, 0.8f }); //After dash
        }},
    music{ 1,
        [](std::vector<std::string>& s)->void
        {
            s.push_back(".//Audio/Hit_02.mp3");
        } }

{
    Transform_.SetTranslate(STARTING_POINT.x, STARTING_POINT.y); // spawn lancelot at this location
    Transform_.SetScale(-1.0f, 1.0f);                            // set texture to face left
    Transform_.Concat();                                         // concat the matrix
    SetPos(STARTING_POINT.x, STARTING_POINT.y);                  // update lancelot current coordinates
    SetActive(true);                                             // spawn lancelot
    Set_Direction(LEFT);                                         // face left
    SetVelocity({ 200.0f, 0.0f });                               // velocity for lancelot
    Reset_Idle_Time(idle_time);		                             // reset the idle duration of lancelot			                 
    Init();                                                      // initialize the attacks lancelot have
}

/**************************************************************************************
//
// Initializes the attacks and particle system for lancelot
//
**************************************************************************************/
void Lancelot::Init()
{
    //initialize the sword texture for lancelot
    const char* sword = ".//Textures/arondight.png";
    attack_sprite = S_CreateSquare(ATTACK_SCALE, sword);

    // reserve the memory used with the number of attacks lancelot have
    lancelot.reserve(limit);
    
    // initializes the various moves lancelot have
    Init_Stab();
    Init_Slash();
    lancelot.emplace_back(Boss_Attack()); // mad enhancement
    Init_Arondight();

    // prevent unique mechanic from activating at the start of fight
    lancelot[MAD_ENHANCEMENT].cooldown_timer = 5.0f; 
    lancelot[MAD_ENHANCEMENT].cooldown = true; 

    // initializes the various particle systems
    Init_Particles();
}

/**************************************************************************************
//
// Initializes the particle system variables
//
**************************************************************************************/
void Lancelot::Init_Particles(void)
{
   // mad enhancement particle variables
    me_particle->Emitter_.PPS_ = 8;
    me_particle->Emitter_.Vol_Max = 2048;
    me_particle->Emitter_.Direction_ = 90.0f;
    me_particle->Emitter_.Particle_Rand_.Spread_ = 180;
    me_particle->Emitter_.Conserve_ = 0.8f;
    me_particle->Emitter_.Size_ = 15.0f;
    me_particle->Emitter_.Speed_ = 10.0f;
    me_particle->Emitter_.Lifetime_ = 2.f;
    
    // arondight particle variables
    arondight_particle->Emitter_.PPS_ = 15;
    arondight_particle->Emitter_.Vol_Max = 4096;
    arondight_particle->Emitter_.Direction_ = 90.0f;
    arondight_particle->Emitter_.Particle_Rand_.Spread_ = 360;
    arondight_particle->Emitter_.Conserve_ = 0.8f;
    arondight_particle->Emitter_.Size_ = 10.0f;
    arondight_particle->Emitter_.Speed_ = 4.0f;
    arondight_particle->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
    arondight_particle->Emitter_.Lifetime_ = 1.f;

    // initializing the particle variables for phase transition
    phase_particle->Emitter_.PPS_ = 20;
    phase_particle->Emitter_.Dist_Min_ = 10.f;
    phase_particle->Emitter_.Vol_Max = 512;
    phase_particle->Emitter_.Direction_ = 90.0f;
    phase_particle->Emitter_.Particle_Rand_.Spread_ = 180;
    phase_particle->Emitter_.Conserve_ = 0.8f;
    phase_particle->Emitter_.Size_ = 15.0f;
    phase_particle->Emitter_.Speed_ = 4.0f;
    phase_particle->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
    phase_particle->Emitter_.Lifetime_ = 0.2f;

    // update the phsae box location
    phase_particle->Emitter_.Pos_.Point_Min_Max[0].y = Collision_.Get_MinPoint().y - 60;
    phase_particle->Emitter_.Pos_.Point_Min_Max[1].y = Collision_.Get_MaxPoint().y + 20;
    phase_particle->Emitter_.Pos_.Point_Min_Max[0].x = -40.f;
    phase_particle->Emitter_.Pos_.Point_Min_Max[1].x = 40.f;

    // update the particle color
    phase_particle->Emitter_.Color_.R = 130.f / 255.f;
    phase_particle->Emitter_.Color_.G = 224.f / 225.f;
    phase_particle->Emitter_.Color_.B = 170.f / 255.f;

}

/**************************************************************************************
//
// Idle state of lancelot
//
**************************************************************************************/
void Lancelot::Idle(const Dragon& d, const float dt)
{
    // change current_action to attack and idle to 0 once idling is over
    if (Get_Idle_Time() <= 0)
    {		
        // change current action to attack if player within attack range, else change it to moving state
        current_action = abs(d.PosX - this->PosX) <= ATTACK_RANGE ? ATTACK : MOVING;
        Reset_Idle_Time(idle_time);   // reset the idle time
        Set_Face_Dir(d);              // set direction lancelot should face
        Set_Attk_Dir();               // set attack direction
    }
    else
        Decrease_Idle_Time(dt);       // decrease the duration time
}
/**************************************************************************************
//
// Moving state of lancelot
//
**************************************************************************************/
void Lancelot::Moving(const Dragon &d, const float dt)
{
    // set the direction lancelot should face
    Set_Face_Dir(d);
    
    if (Get_Direction() == RIGHT)     
    {
        PosX += GetVelocity().x * dt; // move lancelot to the right
    }
    else if (Get_Direction() == LEFT) 
    {
        PosX -= GetVelocity().x * dt; // move lancelot to the left
    }

    // translate lancelot and apply the concatenation 
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

/**************************************************************************************
//
// Update function of lancelot
//
**************************************************************************************/
void Lancelot::Update(Dragon &d, float dt)
{
    // change lancelot to phase 2 if the hp is within the threshold and there is no attack ongoing
    if (this->Get_HP() < PHASE2_HP && phase & PHASE_1 && ! lancelot[currAttk].ongoing_attack)
    {
        Lancelot_Phase2(dt);  // change to phase 2
        Update_Particles(dt); // render the phase transition particle effects
        return;
    }
    else if (this->Get_HP() <= 0) // check if king arthur is dead
    {
        current_action = DEAD;    // set king arthur to its death state
    }

    
    if (M_E) 
    {
        // off mad enhancement state if lancelot is currently under mad enhancement
        if (lancelot[MAD_ENHANCEMENT].cooldown_timer < 10)
            this->Mad_Enhancement(dt);  

        // update emitter position
        me_particle->Emitter_.Pos_.Min_Max.Point_Max    = Collision_.Get_MaxPoint();
        me_particle->Emitter_.Pos_.Min_Max.Point_Min    = Collision_.Get_MinPoint();
        me_particle->Emitter_.Pos_.Min_Max.Point_Max.x -= 10.f;
        me_particle->Emitter_.Pos_.Min_Max.Point_Max.y -= 10.f;
        me_particle->Emitter_.Pos_.Min_Max.Point_Min.x += 10.f;
        me_particle->Emitter_.Pos_.Min_Max.Point_Min.y += 10.f;

        // emit particles
        me_particle->UpdateEmission();
    }

    // checks if lancelot is alive
    if (IsActive())
    {
        // check if fireball hit lancelot
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

        // mega fire ball is active
        if (d.GetMfireball().IsActive())
        {
            // mega fire ball hit lancelot
            if (Collision_.Dy_Rect_Rect(d.GetMfireball().Collision_, GetVelocity(),
                d.GetMfireball().GetVelocity(), dt))
            {
                // decrease lancelot hp, reset mega fireball variables
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
               anim.SetState(IDLE_ANIM);
        break;

    case MOVING: this->Moving(d, dt);
                 anim.SetState(WALK_ANIM);
        break;

    case ATTACK: this->Attack(d, dt);
                 anim.SetState(IDLE_ANIM);
        break;

    case DEAD:   this->Dead();
        break;

    default: break;
    }

    // update the skill cooldowns
    for (char i = 0; i < limit; ++i) 
        lancelot[i].Update(dt);

    Update_Particles(dt);  // update particle effects
    anim.Update(*Sprite_); // update animation
}

/**************************************************************************************
//
// Updates the particle system in lancelot ONLY IF there are active particle
//
**************************************************************************************/
void Lancelot::Update_Particles(const float dt)
{
    if (me_particle->GetParticleCount())
    {
        // adds drag if lancelot is moving
        if (current_action == MOVING)
            me_particle->Drag(0.5f);

        // particle behaviors
        me_particle->Turbulence(0.6f);
        me_particle->Force(0.8f, false, true);
        me_particle->ColorRamp_Life();
        me_particle->TransRamp_Exp();
        me_particle->Update(dt);
    }

    if (arondight_particle->GetParticleCount())
    {
        // particle behaviors
        arondight_particle->Turbulence(0.5f);
        arondight_particle->TransRamp_Exp();
        arondight_particle->Update(dt);
    }

    if (phase_particle->GetParticleCount())
    {
        // particle behaviors
        phase_particle->Turbulence(0.4f);
        phase_particle->Force(0.5f, false, true);        //Simulate an upward force
        phase_particle->Newton({ 10.f, 400.0f }, 2.8f);
        phase_particle->Update(dt);
    }
}
/**************************************************************************************
//
// Render lancelot and his attacks and particles if active
//
**************************************************************************************/
void Lancelot::Render(void)
{
    GameObject::Render();
    lancelot[currAttk].Render();

    phase_particle->Render();
    me_particle->Render();
    arondight_particle->Render();
}
/**************************************************************************************
//
// Attack state of lancelot
//
**************************************************************************************/
void Lancelot::Attack(Dragon &d, const float dt)
{
    // if attacks are not ongoing
    if ( ! lancelot[currAttk].ongoing_attack )
    {
        // currently in phase 1 and mad enhancement is not on cooldown
        if (phase & PHASE_1 && ! lancelot[MAD_ENHANCEMENT].cooldown)
        {
            // teleport lancelot to the center of the screen and make him semi transparent
            PosX = 0.0f;
            Transform_.SetTranslate(PosX, PosY);
            Transform_.Concat();
            Sprite_->SetAlphaTransBM(0.8f, 0.8f, AE_GFX_BM_BLEND);

            charge_time = 2.0f;				// freeze lancelot in the middle of the screen
            Set_Vulnerable(false);		    // make him immune to attacks 
            currAttk = MAD_ENHANCEMENT;     // set current attack lancelot is using

            // attack is currently ongoing
            lancelot[MAD_ENHANCEMENT].ongoing_attack = true;


            // update the collision box of lancelot
            Collision_.Update_Col_Pos(PosX - LANCELOT_SCALE, PosY - LANCELOT_SCALE,
                                      PosX + LANCELOT_SCALE, PosY + LANCELOT_SCALE);

        }

        // it is currently phase 2 and arondight is not on cooldown
        else if (phase & PHASE_2 && ! lancelot[ARONDIGHT].cooldown)
        {				
            angle = 0.0f;         // used for setting range the sword can go
            angle_offset = 3.0f;  // add to the angle every frame
            charge_time = 2.0f;   // freeze lancelot for 2 seconds so player can prepare
            currAttk = ARONDIGHT; // set current attack to arondight

            // teleport lancelot to the middle of the screen and make him semi transparent
            PosX = 0.0f;
            Transform_.SetTranslate(PosX, PosY);
            Transform_.Concat();
            Sprite_->SetAlphaTransBM(0.8f, 0.8f, AE_GFX_BM_BLEND);

            lancelot[ARONDIGHT].Start_Attack(20.0f, ATK_START_POINT.y + 200); // starting position of arondight
            lancelot[ARONDIGHT].Collision_.Update_Col_Pos(0.0f, 400.0f);	  // starting point of collision point
            lancelot[ARONDIGHT].Transform_.SetRotation(-90.0f);				  // rotate texture
            lancelot[ARONDIGHT].Transform_.SetTranslate(lancelot[ARONDIGHT].PosX, lancelot[ARONDIGHT].PosY);
            lancelot[ARONDIGHT].Transform_.Concat();

            // reset emiter position and angle
            arondight_particle->Emitter_.Pos_.Point_Min_Max[0].x = 12.f;
            arondight_particle->Emitter_.Pos_.Point_Min_Max[0].y = -15.f;
            arondight_particle->Emitter_.Pos_.Point_Min_Max[1].x = 18.f;
            arondight_particle->Emitter_.Pos_.Point_Min_Max[1].y = 300.f;
            arondight_particle->Emitter_.Pos_.Min_Max.Angle_ = PI * 0.5f;

            // update the collision box of lancelot
            Collision_.Update_Col_Pos(PosX - LANCELOT_SCALE, PosY - LANCELOT_SCALE,
                                      PosX + LANCELOT_SCALE, PosY + LANCELOT_SCALE);           
        }
        
        // slash is not on cooldown
        else if ( ! lancelot[SLASH].cooldown)
        {
            currAttk = SLASH;   // change current attack to slash
            charge_time = 0.5f; // charge time for player to prepare themselves

            // player is on the right hand side
            if (this->Get_Direction())
            {
                // set slash to go to the right side
                lancelot[SLASH].Start_Attack(this->PosX + -ATK_START_POINT.x, ATK_START_POINT.y);
                lancelot[SLASH].SetDir(false);
                angle = -200.0f;         // starting angle of rotation
                angle_offset = 2.0f;     // offset of angle
            }
            else // player on the left hand side
            {
                // set slash to go to the left side
                lancelot[SLASH].Start_Attack(this->PosX + ATK_START_POINT.x, ATK_START_POINT.y);
                angle = 25.0f;            // starting angle of rotation
                angle_offset = -2.0f;     // offset of angle
            }
            lancelot[SLASH].SetActive(false);
        }

        // default attack is stab
        else
        {
            // set current attack to stab
            currAttk = STAB;  
            lancelot[STAB].Start_Attack(this->PosX, this->PosY);
           
            if (Get_Direction() == RIGHT)
            {
                // rotate the texture to the right direction
                lancelot[STAB].Transform_.SetScale(-3.0f, 2.0f);
            }
            else
            {
                // rotate the texture to the left direction
                lancelot[STAB].Transform_.SetScale(3.0f, 2.0f);
            }
        }
    }

    // state machine for boss attacks
    switch (currAttk)
    {
    case STAB:            Stab(d, dt);
        break;
    case SLASH:           Slash(d, dt);
        break;
    case MAD_ENHANCEMENT: Mad_Enhancement(dt);
        break;
    case ARONDIGHT:       Arondight(d, dt);
        break;
    }

}
/**************************************************************************************
//
// lancelot phase 2 transition function
//
**************************************************************************************/
void Lancelot::Lancelot_Phase2(const float dt)
{
    if(M_E) // turns off mad enhancement if its activated
        Mad_Enhancement(dt); 

    me_particle->Off_Emitter(); // set all particle active to false

    // move lancelot to the middle of the screen
    PosX = 0.0f;
    Transform_.SetTranslate(PosX, PosY);
    Transform_.Concat();
    Collision_.Update_Col_Pos(PosX - LANCELOT_SCALE, PosY - LANCELOT_SCALE,   // min point
                              PosX + LANCELOT_SCALE, PosY + LANCELOT_SCALE);  // max point
    
    Set_Vulnerable(false);                 // make lancelot immune to attacks
    phase_particle->UpdateEmission();      // create particles

    // shake the camera for a few seconds
    if (timer > 0.f)
    {
        CamShake();
        timer -= dt;
        return;
    }

    phase = PHASE_2;            // change to phase 2
    current_action = ATTACK;    // set current state to attack
    Set_Vulnerable(true);       // set lancelot to vulnerable to attack
}
/**************************************************************************************
//
// stab attack of lancelot
//
**************************************************************************************/
void Lancelot::Stab(Dragon& d, const float dt)
{
    lancelot[STAB].Projectile::Update(dt, ATTACK_SCALE, false, 0.0f); // move the stab projectile
    
    // accelerate the stab once it past 10 pixels
    if (lancelot[STAB].GetDist() > 10.0f) 
    {
        const float stab_accel = 80.0f;                        // acceleration of stab
        AEVec2 new_velocity = lancelot[STAB].GetVelocity();    // get the current velocity of stab
        new_velocity.x += stab_accel;                          // add the acceleration of stab
        lancelot[STAB].SetVelocity(new_velocity);              // set the new velocity

        // check if stab has collided, so player only take once instance of damage
        if (!lancelot[STAB].GetCollided()) 
        {
            // stab collided with player
            if (lancelot[STAB].Collision_.Dy_Rect_Rect(d.Collision_, lancelot[STAB].GetVelocity(), d.GetVelocity(), dt))
            {
                lancelot[STAB].SetCollided(true);   // set collided to true
                d.Decrease_HP();                    // decrease dragon hp
                d.PlayHit();                        // play sound
                d.SetInvul(true);                   // Visual Feedback and make dragon invulnerable for awhile
            }
        }
    }

    if (lancelot[STAB].GetDist() > 100.0f) // range of stab
    {
        lancelot[STAB].End_Attack();                 // reset the stab variables
        lancelot[STAB].SetVelocity(STAB_VELOCITY);   // reset stab's velocity
        current_action = IDLE;                       // set behavior to idle
    }
    
}
/**************************************************************************************
//
// slash attack of lancelot
//
**************************************************************************************/
void Lancelot::Slash(Dragon& d, const float dt)
{
    static bool second_slash = false;	 // checks if its the first or second slash
    
    // freeze lancelot for player to prepare
    while (charge_time > 0)
    {
        charge_time -= dt;
        return;
    }

    // set slash active to true
    lancelot[SLASH].SetActive(true);

    // rotate the slash texture
    lancelot[SLASH].Projectile::Update(dt, ATTACK_SCALE, false, angle += angle_offset);

    // checks for collision with player. Only collide once
    if (!lancelot[SLASH].GetCollided())
    {
        // slash hit player
        if (lancelot[SLASH].Collision_.Dy_Rect_Rect(d.Collision_, lancelot[SLASH].GetVelocity(), d.GetVelocity(), dt))
        {
            lancelot[SLASH].SetCollided(true);  // set collision flag to true
            d.Decrease_HP();                    // decrease player hp
            d.PlayHit();                        // play music
            d.SetInvul(true);                   // Visual Feedback and make dragon invulnerable for awhile
        }
    }

    // range of slash
    if (lancelot[SLASH].GetDist() > 200.0f) 
    { 
        // currently in second slash
        if (second_slash)
        {
            lancelot[SLASH].cooldown_timer = M_E ? 2.0f : 4.0f;  // shorter cooldown when berserked.
            lancelot[SLASH].End_Attack();                        // set active to false and reset variable
            lancelot[SLASH].SetVelocity(SLASH_VELOCITY);         // reset velocity of slash
            current_action = IDLE;                               // set behavior to idle
            second_slash = false;                                // reset second slash flag
            angle_offset = -angle_offset;
        }
        else  // reverse the velocity to make slash move backwards for second slash
        {
            AEVec2 reverse = {SLASH_VELOCITY.x, SLASH_VELOCITY.y};    // current velocity
            reverse.x = -reverse.x;                                   // negate the velocity to move backwards
            reverse.y = -reverse.y;									  // negate the velocity to move backwards
            second_slash = true;                                      // set flag to true
            lancelot[SLASH].SetVelocity(reverse);                     // update new velocity
            lancelot[SLASH].ResetDist();                              // reset distance travled back to 0
            angle_offset = -angle_offset;                             // reverse the angle offset
        }
    }

}
/**************************************************************************************
//
// mad enhancement of lancelot
//
**************************************************************************************/
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
        lancelot[MAD_ENHANCEMENT].ongoing_attack = false;        // mad enhancement is no longer ongoing
        current_action = MOVING;                                 // change current action
        Sprite_->SetAlphaTransBM(1.f, 1.f, AE_GFX_BM_BLEND);     // set lancelot to be opaque
        Set_Vulnerable(true);                                    // lancelot is now vulnerable to attacks
    }

}
/**************************************************************************************
//
// Arondight attack of lancelot
//
**************************************************************************************/
void Lancelot::Arondight(Dragon& d, const float dt)
{
    // play particle effects
    arondight_particle->UpdateEmission();
    
    while (charge_time > 0) // freeze lancelot for 2 seconds for player to prepare
    {
        charge_time -= dt;
        arondight_particle->Gravity(0.3f);
        return;
    }

    AEVec2 new_vector = lancelot[ARONDIGHT].Collision_.Get_Point();                                // get the point at the top of the sword
    AEVec2 position = { this->PosX, this->PosY };                                                  // position of lancelot. Used for collision detection
    HalfPlane side;                                                                                // determine which side of the screen the player is at
    float s = 0.0f;				                                                                   // sin of angle to rotate
    float c = 0.0f;				                                                                   // cosine of angle to rotate
    float radians = 0.0f;		                                                                   // randians version of degree to rotate
    float tempX = new_vector.x;                                                                    // old value of x
    new_vector.x -= PosX;		                                                                   // rotate collision point with using lancelot as the origin
    new_vector.y -= PosY;		                                                                   // rotate collision point with using lancelot as the origin
    lancelot[ARONDIGHT].Transform_.SetRotation(AERadToDeg(atan(new_vector.y / new_vector.x)));     // rotate texture
    Sprite_->SetAlphaTransBM(1.f, 1.f, AE_GFX_BM_BLEND);                                           // set lancelot to be opqque
    

    if (Get_Direction() == RIGHT)
    {
        radians = AEDegToRad(-angle_offset); // rotate the sword by -3 degrees
        side = OUTSIDE;					     // check the outisde half plane

        // reflect the texture
        lancelot[ARONDIGHT].Transform_.SetScale(-ARONDIGHT_SCALE.x, ARONDIGHT_SCALE.y); 

        // moving the emiter box to the right
        arondight_particle->Emitter_.Pos_.Point_Min_Max[0].x += 10.f;
        arondight_particle->Emitter_.Pos_.Point_Min_Max[1].x += 10.f;
    }
    else
    {
        radians = AEDegToRad(angle_offset); // rotate the sword by 3 degrees
        side = INSIDE;					    // check the inside half plane

        // moving the emitter box to the left
        arondight_particle->Emitter_.Pos_.Point_Min_Max[0].x -= 10.f;
        arondight_particle->Emitter_.Pos_.Point_Min_Max[1].x -= 10.f;
    }

    // moving the y coordinates of the emiter box, updating the angle as well
    arondight_particle->Emitter_.Pos_.Min_Max.Angle_ +=  radians;
    arondight_particle->Emitter_.Pos_.Point_Min_Max[0].y -= 15.f;
    arondight_particle->Emitter_.Pos_.Point_Min_Max[1].y -= 15.f;

    s = sin(radians);   // get the sin of the angle
    c = cos(radians);	// get the cos of the angle 

    new_vector.x = new_vector.x * c - new_vector.y * s + PosX; // collision new point after rotation
    new_vector.y = tempX * s + new_vector.y * c + PosY;		   // collision new point after rotation



    lancelot[ARONDIGHT].PosX = new_vector.x * 0.5f + PosX * 0.5f;                                      // new position of sword
    lancelot[ARONDIGHT].PosY = new_vector.y * 0.5f + PosY * 0.5f;                                      // new position of sword
    lancelot[ARONDIGHT].Transform_.SetTranslate(lancelot[ARONDIGHT].PosX, lancelot[ARONDIGHT].PosY);   // update position of texture
    lancelot[ARONDIGHT].Transform_.Concat();                                                           // apply transformation
    lancelot[ARONDIGHT].Collision_.Update_Col_Pos(new_vector.x, new_vector.y);                         // update point coordinates for collision

    angle += angle_offset; // update the new angle

    // checks if the attack collided with player
    if (!lancelot[ARONDIGHT].GetCollided() && Player_Facing_Me (*this, d) )
    {
        // attack hited player
        if (lancelot[ARONDIGHT].Collision_.Line_Point(lancelot[ARONDIGHT].Collision_, d.Collision_, position, side))
        {
            d.Decrease_HP();                        // decrease player hp
            lancelot[ARONDIGHT].SetCollided(true);  // set collided flag to true
            d.PlayHit();                            // play music
            d.SetInvul(true);                       // Visual Feedback and make dragon invulnerable for awhile
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

}
/**************************************************************************************
//
// sets the texture facing direction
//
**************************************************************************************/
void Lancelot::Set_Face_Dir(const Dragon& d)
{
    // dragon is on the right of player
    if (d.PosX - this->PosX > 0)
    {
        // set direction to right and flip the texture
        this->Set_Direction(RIGHT);
        this->Transform_.SetScale(1.0f, 1.0f);
    }

    // dragon is on the left of player
    else
    {
        // set direction to left and flip the texture
        this->Set_Direction(LEFT);
        this->Transform_.SetScale(-1.0f, 1.0f);
    }

    // apply transformation
    this->Transform_.Concat();
}
/**************************************************************************************
//
// sets the attack facing direction
//
**************************************************************************************/
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
/**************************************************************************************
//
// Initialize the stab variables
//
**************************************************************************************/
void Lancelot::Init_Stab(void)
{
    // push back a boss_attack object
    lancelot.emplace_back(&attack_sprite, Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect));
    
    lancelot[STAB].SetVelocity(STAB_VELOCITY);                              // velocity for slash
    lancelot[STAB].Transform_.SetScale(3.0f, 2.0f);                         // determine the size of projectile
    lancelot[STAB].Transform_.Concat();                                     // apply transformation
    lancelot[STAB].Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);   // set it to blend mode
}
/**************************************************************************************
//
// Initialize the slash variables
//
**************************************************************************************/
void Lancelot::Init_Slash(void)
{
    // push back a boss_attack object
    lancelot.emplace_back(&attack_sprite, Col_Comp(0.0f, 0.0f, 5.0f, 5.0f, Rect));
   
    lancelot[SLASH].SetVelocity(SLASH_VELOCITY);                             // velocity for slash
    lancelot[SLASH].Transform_.SetScale(3.0f, 2.0f);                         // scale the slash
    lancelot[SLASH].Transform_.Concat();                                     // apply transformation
    lancelot[SLASH].Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);   // set it to blend mode
}
/**************************************************************************************
//
// Initialize the arondight variables
//
**************************************************************************************/
void Lancelot::Init_Arondight(void)
{
    // push back a boss_attack object
    lancelot.emplace_back(&attack_sprite, Col_Comp(0.0f, 400.0f, Point));
    
    lancelot[ARONDIGHT].SetVelocity(AEVec2{ 600.0f, 600.0f });                        // velocity for slash
    lancelot[ARONDIGHT].Transform_.SetScale(ARONDIGHT_SCALE.x, ARONDIGHT_SCALE.y);    // determine the size of projectile
    lancelot[ARONDIGHT].Transform_.Concat();                                          // apply transformation
    lancelot[ARONDIGHT].Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);        // set to blend mode
}
/**************************************************************************************
//
// Dead state of lancelot
//
**************************************************************************************/
void Lancelot::Dead(void)
{
    // set all active attacks to false
    for (Boss_Attack& i : lancelot)
    {
        i.SetActive(false);
    }

    // set lancelot active to false
    SetActive(false);
}

void Lancelot::Mute(void)
{
    music.SetVolume(0, 0.0f);
    music.SetPause(0, true);
}

void Lancelot::Unmute(void)
{
    music.SetVolume(0, 1.0f);
    music.SetPause(0, false);
}
/**************************************************************************************
//
// lancelot destructor
//
**************************************************************************************/
Lancelot::~Lancelot()
{
    // empty the vector of attacks    
    lancelot.clear();

    // remove particles from screens
    arondight_particle->Off_Emitter();
    me_particle->Off_Emitter();
    phase_particle->Off_Emitter();


    attack_sprite.~Sprite(); // destroy the mesh and texture allcoated 
}

namespace
{

/**************************************************************************************
//
// check if the player is facing lancelot ( used for arondight attack)
//
**************************************************************************************/
bool Player_Facing_Me(Lancelot& l, Dragon& d)
{
    // get the direction lancelot is facing
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