/* Start Header ************************************************************************/
/*!
\file       Knight.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Body file for Knight class

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Knight.h"
#include "cmath"

namespace  // global variables for KNIGHT
{
    #define HEALTH  100
    #define GRAVITY 10.0f

    #define ATTACK_RANGE 120.0f
    #define STAB_ACCELRATOR  100.0f

    #define KNIGHT_SCALE 70.0f
    #define STAB_SCALE  40.0f
    const AEVec2 STAB_VELOCITY {20.0f, 0.0f};

}

/**************************************************************************************
//
// Constructor of knight obj
// Takes in the a spawn position and pointer to its texture
//
**************************************************************************************/
Knight::Knight(const AEVec2 & spawn_location, Sprite* texture)
    : Characters(texture, HEALTH,
        Col_Comp{ spawn_location.x - KNIGHT_SCALE, spawn_location.y - KNIGHT_SCALE ,
                  spawn_location.x + KNIGHT_SCALE , spawn_location.y + KNIGHT_SCALE, Rect }),
      current_action{ IDLE }, time_traveled{ 0 },
      
      stab{ Get_Attack_Sprite(STAB_SPRITE),
          Col_Comp { spawn_location.x - STAB_SCALE, spawn_location.y - STAB_SCALE,
                     spawn_location.x + STAB_SCALE, spawn_location.y + STAB_SCALE, Rect} },
      
      anim{ WALK_ANIM + 1, 3, 5, [](std::vector <Range>& Init) -> void {
                                 Init.push_back(Range{ 0.0f, 1.0f, 0.00f, 0.00f }); //Hit
                                 Init.push_back(Range{ 0.0f, 1.0f, 0.33f, 0.33f }); //Idle
                                 Init.push_back(Range{ 0.0f, 1.0f, 0.66f, 0.66f }); //Walk
           } }
{
    SetPos(spawn_location.x, spawn_location.y);						 // spawn location of knight
    SetActive(false);												 // don't render on screen yet
    Transform_.SetTranslate(spawn_location.x, spawn_location.y);	 // moving him to screen location
    Transform_.Concat();
    Reset_Idle_Time(1.0f);											 // duration of idle time for knight
    SetVelocity(AEVec2{ 50.0f, 0.0f });								 // velocity of knight
    Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);           // set blend mode
    stab.SetVelocity(STAB_VELOCITY);								 // velocity for stab
}
/**************************************************************************************
//
// Idle function for knight
//
**************************************************************************************/
void Knight::Idle(const Dragon& d, const float dt)
{
    // return if player not within line of sight
    if (!Line_Of_Sight(d))
        return;

    if (Get_Idle_Time() < 0) // finish idling
    {
        Reset_Idle_Time(1.0f); // reset idle duration
        
        // if knight is within attack range, attack player, else continue moving
        abs(d.PosX - PosX) <= ATTACK_RANGE ? current_action = ATTACK : current_action = MOVING;
        
        Set_Forward_Dir(d); // set direction to face
    }
    else
    {
        Decrease_Idle_Time(dt); // reduce idle time
    }
}
/**************************************************************************************
//
// Moving function for knight
//
**************************************************************************************/
void Knight::Moving(const Dragon &d, const float dt)
{
    Set_Forward_Dir(d);	 // set the direction the knight suppose to go

    // player is seen
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

        // move him to the new coordinates
        Transform_.SetTranslate(PosX, PosY); 
        Transform_.Concat();

        // update the collision box of knight
        Collision_.Update_Col_Pos(PosX - KNIGHT_SCALE, PosY - KNIGHT_SCALE,
                                  PosX + KNIGHT_SCALE, PosY + KNIGHT_SCALE);
    }
        
        if (abs(d.PosX - PosX) <= ATTACK_RANGE) // player within attack range
        {
            current_action = ATTACK; // change state to attack
            time_traveled = 0.0f;
            return;
        }
        else if (time_traveled > 3.0f)    // player not within range
        {
            current_action = IDLE;       // change state to idle
            time_traveled = 0.0f;
            return;
        }
    

    time_traveled += dt; // reduce timer
     
}
/**************************************************************************************
//
// Attack function for knight
//
**************************************************************************************/
void Knight::Attack(Dragon& d, const float dt)
{
    if ( ! stab.ongoing_attack) // place stab position to knight location
    {
        stab.Start_Attack(PosX, PosY);
    }
    
    // update positon	
    if (Get_Direction() == RIGHT)
        stab.Projectile::Update(dt, STAB_SCALE, false, 180);    // rotate texture to look right
    else
        stab.Projectile::Update(dt, STAB_SCALE, false, 0);      // rotate texture to look left

    if ( ! stab.GetCollided()) // check if attack collided with player
    {
        // attack can only collide with player once
        if (stab.Collision_.Dy_Rect_Rect(d.Collision_, stab.GetVelocity(), d.GetVelocity(), dt))
        {
            d.Decrease_HP();        // decrease dragon hp
            stab.SetCollided(true);	// set collided to true so it cannot collide again
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
/**************************************************************************************
//
// Dead function for knight
//
**************************************************************************************/
void Knight::Dead(void)
{
    SetActive(false);	    // set active back to false
    Set_HP(HEALTH);			// set HP to original state for future usage
    stab.SetActive(false);  // set stab to false also
    Add_Kill_count();       // add kill count
    Add_Score(30);          // add score

}
/**************************************************************************************
//
// check if player is within line of sight
//
**************************************************************************************/
bool Knight::Line_Of_Sight(const Dragon& d)
{
    return abs(d.PosX - PosX) < 1280.0f; // check if knight can see player on screen
}

/**************************************************************************************
//
// Set the facing direction of knight
//
**************************************************************************************/
void Knight::Set_Forward_Dir(const Dragon& d)
{
    if (d.PosX - PosX > 20)   // dragom on the right of player
    {
        Transform_.SetScale(1.0f, 1.0f); // reflect texture
        Set_Direction(RIGHT);			 // set direction
        stab.SetDir(true);				 // set stab attack to go to the right
    }
    else                     // dragon on the left of player
    {
        Transform_.SetScale(-1.0f, 1.0f); // reflect texture
        Set_Direction(LEFT);			  // set direction
        stab.SetDir(false);				  // set stab attack to go left
    }

    // apply transformations
    Transform_.Concat();
}

/**************************************************************************************
//
// Update function of knight
//
**************************************************************************************/
void Knight::Update(Dragon &d, const float dt)
{
    if (Get_HP() < 1) // knight has died
    {
        current_action = DEAD;
    }
    
    // if knight is active, check for collision against fireball
    if (IsActive())
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
                    // Visual Feedback and make dragon invulnerable for awhile
                    d.SetInvul(true);
                }

        // mega fire ball hit lancelot
        if (d.GetMfireball().IsActive())
        {
            if (Collision_.Dy_Rect_Rect(d.GetMfireball().Collision_, GetVelocity(),
                d.GetMfireball().GetVelocity(), dt))
            {
                // decrease knight hp and reset mega fireball variables
                Decrease_HP(d.GetMDamage());
                d.GetMfireball().Projectile::ResetDist();
                d.GetMfireball().SetActive(false);
                // Visual Feedback and make dragon invulnerable for awhile
                d.SetInvul(true);
            }
        }
    }

    PosY -= GRAVITY; // apply gravity on knight

    // update AABB box
    Collision_.Update_Col_Pos(PosX - KNIGHT_SCALE, PosY - KNIGHT_SCALE,
                              PosX + KNIGHT_SCALE, PosY + KNIGHT_SCALE);

    // update latest position of knight
    Transform_.SetTranslate(PosX, PosY);
    Transform_.Concat();

    switch (current_action) // state machine for knight
    {
    case IDLE:   Idle(d, dt);
                 anim.SetState(IDLE_ANIM);
        break;
    case MOVING: Moving(d, dt);
                 anim.SetState(WALK_ANIM);
        break;
    case ATTACK: Attack(d, dt);
                 anim.SetState(IDLE_ANIM);
        break;
    case DEAD:   Dead();
        break;
    default: break;
    }

    // update animation
    anim.Update(*Sprite_);
}

/**************************************************************************************
//
// render both knight and his attack on screen
//
**************************************************************************************/
void Knight::Render() 
{
    GameObject::Render(); // render knight
    stab.Render();		  // render knight's attack
}

/**************************************************************************************
//
// Destructor of knight to delete the sprite that was new for it
//
**************************************************************************************/
Knight::~Knight()
{
    delete Sprite_;
}

