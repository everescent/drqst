/* Start Header ************************************************************************/
/*!
\file       PickUp.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  PickUp class member functions defined here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "PickUp.h"
#include <utility>

//Counts the number of coins collected
int PickUp::Coin_Counter = 0;

PickUp::PickUp(Sprite &&t_sprite, Col_Comp && t_col, const PUT type, const float posX, const float posY)
  :GameObject{std::move(t_sprite), std::move(t_col)}, 
  Type_{ type }, Active_{ false }, Cooldown_{ 0 }
{
  SetActive(true);
  PosX = posX;
  PosY = posY;
  Transform_.SetTranslate(PosX, PosY);
  Transform_.Concat();
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
      player.GetVelocity(), 0.016f))
    {
      //If collide, activate the power up
      player.SetPickup(Type_, true);
      //Disable render
      SetActive(false);
      if (Type_ == COIN)
      {
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
}
