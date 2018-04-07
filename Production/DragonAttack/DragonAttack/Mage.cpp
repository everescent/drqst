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

    const float MAGE_SCALE     = 55.0f;      // scale for mage
    const float EBALL_SCALE    = 20.0f;		 // scale for energyball

    const float LEFT_BOUNDARY  = -600.0f;    // boundaries of screen
    const float RIGHT_BOUNDARY = 600.0f;	 // boundaries of screen

}

/**************************************************************************************
//
// Constructor of Mage obj
// Takes in the a spawn position and pointer to its texture
//
**************************************************************************************/
Mage::Mage(const AEVec2& position, Sprite* texture)
    :Characters(texture,
        HEALTH, Col_Comp{ position.x - MAGE_SCALE, position.y - MAGE_SCALE , position.x + MAGE_SCALE , position.y + MAGE_SCALE, Rect }),
    current_action{ IDLE }, teleport{ true },

    energy_ball{ Get_Attack_Sprite(EBALL_SPRITE),
                 Col_Comp{ position.x - EBALL_SCALE, position.y - EBALL_SCALE,
                           position.x + EBALL_SCALE, position.y + EBALL_SCALE, Rect } },
    anim { IDLE_ANIM + 1, 2, 5, [](std::vector <Range>& Init) -> void {
                                Init.push_back(Range{ 0.0f, 1.0f, 0.0f, 0.0f }); //Hit
                                Init.push_back(Range{ 0.0f, 1.0f, 0.5f, 0.5f }); //Idle
        }}

{
   SetActive(false);                                        // don't spawn mage
   SetPos(position.x, position.y);							// starting coordinates
   Transform_.SetTranslate(position.x, position.y);			// add to matrix
   Transform_.Concat();										// add to final matrix
   Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);   // set blend mode
   Reset_Idle_Time(2.0f);									// idling time for mage
}

/**************************************************************************************
//
// Update function of mage
//
**************************************************************************************/

void Mage::Update(Dragon &d, const float dt)
{	
    // changes state to dead when hp is 0
    if (Get_HP() <= 0)
        current_action = DEAD;

    // teleport the mage to the back of the player once hp falls below a certain threshold
    if (Get_HP() < TELEPORT_FLAG && teleport && !energy_ball.ongoing_attack)
    {
        current_action = MOVING;
    }
    
    // checks for collision with fireball if mage is active
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
                    anim.SetState(HIT_ANIM);
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
                anim.SetState(HIT_ANIM);
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

    // mage default animation other than hit
    if(anim.GetComplete(HIT_ANIM))
        anim.SetState(IDLE_ANIM);

    // update the mage animation
    anim.Update(*Sprite_);
}

/**************************************************************************************
//
// render both mage and his attack on screen
//
**************************************************************************************/

void Mage::Render(void)
{
    GameObject::Render(); // render the mage


    energy_ball.Render(); //render the energyball
}

/**************************************************************************************
//
// Idle function for mage
//
**************************************************************************************/
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
/**************************************************************************************
//
// Attack function for mage
//
**************************************************************************************/
void Mage::Attack(const float dt, Dragon &d)
{
    // range limit of mage
    static const float RANGE_LIMIT = 600.0f;

    // resets the coordinates and velocity of energy ball
    // this is only called once before the firing of energy ball
    if (!energy_ball.ongoing_attack)
    {
        AEVec2 Displacement = { d.PosX - PosX, d.PosY - PosY };
        AEVec2Normalize(&Displacement, &Displacement);
        AEVec2Scale(&Displacement, &Displacement, 360.0f);
        energy_ball.SetVelocity(Displacement);
        
        // set the energy ball position to the mage current location
        energy_ball.SetPos(PosX, PosY);
        
        
        energy_ball.SetActive(true);       // set energyball to active
        energy_ball.ongoing_attack = true; // attack ongoing flag to true
    }
  
    // move the energy ball base on its velocity
    energy_ball.Projectile::Update(dt, EBALL_SCALE);

    // check for collision between energyball and player
    if (energy_ball.Collision_.Dy_Rect_Rect(d.Collision_, energy_ball.GetVelocity(), d.GetVelocity(), dt))
    {
        d.Decrease_HP();                // decrease hp if hit
        energy_ball.SetCollided(true);  // set collision flag to true
        d.SetInvul(true);               // Visual Feedback and make dragon invulnerable for awhile
    }

    // kills the energy ball if it flew past its limit or it collided with player
    if (energy_ball.GetDist() > RANGE_LIMIT || energy_ball.GetCollided())
    {
        energy_ball.End_Attack();
        current_action = IDLE;         // change behaviour
    }

}
/**************************************************************************************
//
// Move function for mage
//
**************************************************************************************/
void Mage::Move(const Dragon &d)
{
    const float location = d.PosX - PosX;       // get the distance between dragon and mage
    const float teleport_distance = 100.0f;     // how far the mage teleports away from player

    if (location > 0) // dragon is on the right of mage
    {
        PosX = d.PosX + teleport_distance > RIGHT_BOUNDARY ? RIGHT_BOUNDARY
                                                          : d.PosX + teleport_distance;
    }
    else			 // dragon is on the left of mage
    {
        PosX = d.PosX - teleport_distance < LEFT_BOUNDARY ? LEFT_BOUNDARY
                                                         : d.PosX - teleport_distance;

    }

    // update the collision box of mage
    Collision_.Update_Col_Pos(PosX - MAGE_SCALE, PosY - MAGE_SCALE,  // min point
                              PosX + MAGE_SCALE, PosY + MAGE_SCALE);	// max point

    Transform_.SetTranslate(PosX, PosY);   // update coordinates of mage
    Transform_.Concat();				   // update coordinates of mage
    teleport = false;					   // mage can only teleport once
    current_action = IDLE;				   // change behaviour

}
/**************************************************************************************
//
// Re-instantiate the mage with its new position
//
**************************************************************************************/

void Mage::Renew_Mage(const AEVec2& newPos)
{
    // updates the new position of Mage
    PosX = newPos.x;
    PosY = newPos.y;

    // update the transformation matrix
    Transform_.SetTranslate(PosX, PosY);
    Transform_.Concat();
    
    Collision_.Update_Col_Pos(PosX - MAGE_SCALE, PosY - MAGE_SCALE,  // min point
                              PosX + MAGE_SCALE, PosY + MAGE_SCALE); // max point

}

/**************************************************************************************
//
// Dead function for mage
//
**************************************************************************************/

void Mage::Dead(void)
{
    SetActive(false);                // set active to false
    energy_ball.SetActive(false);    // set energyball to false
    Set_HP(HEALTH);                  // reset the hp of mage
    teleport = true;                 // reset teleport flag
    current_action = IDLE;           // reset state
    Add_Kill_count();                // add kill count
    Add_Score(30);                   // add score
}

/**************************************************************************************
//
// Check if player is within line of sight
//
**************************************************************************************/

bool Mage::Line_Of_Sight(const Dragon &d)
{
    // distance between player and mage
    float distance = { d.PosX - this->PosX };
    
    // checks if player is in same screen as mage
    return distance > LEFT_BOUNDARY && distance < RIGHT_BOUNDARY;
}

Mage::~Mage()
{
    delete Sprite_;
}