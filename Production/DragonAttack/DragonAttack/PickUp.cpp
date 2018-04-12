/* Start Header ************************************************************************/
/*!
\file    PickUp.cpp
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  PickUp class member functions defined here.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "PickUp.h"
#include "Particle_Effects.h"
#include <utility> //move

//Counts the number of coins collected
int              PickUp::Coin_Counter   = 0      ;
Particle_System* PickUp::coin_particles = nullptr;

//Initialize with sprite, collision, type of power up and position
PickUp::PickUp(Sprite* p_sprite, Col_Comp && t_col, const PUT type, 
               const float posX, const float posY)
  :GameObject{ p_sprite, std::move(t_col)},
   Type_     { type                      },
   Active_   { false                     },
   Cooldown_ { 0                         }
{
  SetActive(true)                                      ;
  Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  PosX = posX                                          ;
  PosY = posY                                          ;
  Transform_.SetTranslate(PosX, PosY)                  ;
  Transform_.Concat      (          )                  ;

  PickUp::coin_particles = Effects_Get(COIN_PARTICLE)  ;

  //BEHAVIOUR FOR COIN
  coin_particles->Emitter_.PPS_                    = 150  ;
  coin_particles->Emitter_.Dist_Min_               = 10.0f;
  coin_particles->Emitter_.Vol_Max                 = 150  ;
  coin_particles->Emitter_.Direction_              = 90.0f;
  coin_particles->Emitter_.Particle_Rand_.Spread_  = 360  ;
  coin_particles->Emitter_.Conserve_               = 0.9f ;
  coin_particles->Emitter_.Size_                   = 20.0f;
  coin_particles->Emitter_.Speed_                  = 50.0f;
  coin_particles->Emitter_.Particle_Rand_.Sp_Rand_ = 3    ;
  coin_particles->Emitter_.Lifetime_               = 1.0f ;
  //Initialize cooldown
  switch (Type_)
  {
    case DMG:
      Cooldown_ = DMG_CD;
      break;
    case SPD:
      Cooldown_ = SPD_CD;
      break;
    case INVUL:
      Cooldown_ = INVUL_CD;
      break;
  }
}

//Updates collision and cooldown
void PickUp::Update(Dragon &player, const float dt)
{
  //Pick up collision check
  if (IsActive())
  {
    //Update collision position
    Collision_.Update_Col_Pos(PosX - PickUp_Scale, PosY - PickUp_Scale, 
                              PosX + PickUp_Scale, PosY + PickUp_Scale);
    //Check for collision with player
    if (player.Collision_.Dy_Rect_Rect(Collision_, GetVelocity(),
      player.GetVelocity(), dt))
    {
      //If collide, activate the power up
      player.SetPickup(Type_, true);
      //Disable render
      SetActive(false);
      if (Type_ == COIN)
      {
        //Coin behaviour and look
        coin_particles->Emitter_.Pos_.Point = {PosX, PosY};
        coin_particles->TransRamp_Exp()                   ;
        coin_particles->ColorRamp_Life()                  ;
        coin_particles->UpdateEmission()                  ;
        //Adds score if coin is picked up
        player.Add_Score(10);
        ++Coin_Counter;
      }
      else if (Type_ == HP)
      {
        //Give HP if HP is picked up
        player.SetPickup(Type_, false);
	   player.Increase_HP(1);
      }
      else
        //If not activate cooldown timer
        Active_ = true;
    }
  }
  else if (Active_)
  {
    //Update cooldown timer
    Cooldown_ -= dt;
    player.SetPickup(Type_, true);
    if (Cooldown_ <= 0.0f)
    {
      //Deactivate power up
      Cooldown_ = 0.0f;
      player.SetPickup(Type_, false);
      Active_ = false;
    }
  }
  //Update coin particles if any
  if (coin_particles->GetParticleCount())
  {
    coin_particles->Update(dt);
  }
}
