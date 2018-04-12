/* Start Header ************************************************************************/
/*!
\file    Merlin.h
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Merlin class declared here. 

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include "Characters.h"
#include "Boss_States.h"
#include "Dragon.h"
#include "Animation.h"
#include <vector>

//Attack state of Merlin
enum Merlin_Attack_State {
  MELEE = 0,    //Melee attack
  SINGLE_EBALL, //Single Shot Energy Ball
  SPREAD_EBALL, //Spread Shot Energy Ball
  ARROW_RAIN    //Arrow Rain
};

//Alias for Merlin Attack State
using MAS = Merlin_Attack_State;

//Global Read-Only Variables START///////////////////////////////////////////////////////
const int    Merlin_HP       { 500   };   //Merlin's HP
const int    M_Phase2_HP     { 250   };   //Merlin's phase 2 HP
const float  Merlin_Scale    { 70.0f };   //Cooldown time for Blink
const float  Blink_CD_Time   { 10.0f };   //Cooldown time for Blink
const float  Merlin_Att_Inter{ 2.0f  };   //Merlin's Attack Interval

const float  Eball_Scale     { 50.0f   }; //Energy Ball cooldown time
const float  Eball_CD_Time   { 2.0f    }; //Energy Ball cooldown time
const float  Eball_Death     { 1000.0f }; //How far the energy ball travels

const float  Spread_Scale    { 30.0f   }; //Spread shot cooldown time
const float  Spread_CD_Time  { 8.0f    }; //Spread shot cooldown time
const float  Spread_Death    { 1000.0f }; //How far the spread shot travels

const int    A_Rain_Buffer   { 30      }; //How many arrows shot
const float  A_Rain_Scale    { 30.0f   }; //Arrow death
const float  A_Rain_CD_Time  { 30.0f   }; //Arrow rain cooldown time
const float  A_Rain_Death    { 1000.0f }; //Arrow death

const float  Melee_CD_Time   { 10.0f };   //Melee cooldown time

const AEVec2 Blink_Pos_1     { -300.0f, 100.0f}; //(Point) 1st blink postion
const AEVec2 Blink_Pos_2     { 300.0f, 100.0f }; //(Point) 2nd blink postion
const AEVec2 Blink_Pos_3     { 0.0f, 180.0f   }; //(Point) 3rd blink postion

const float  Merlin_Start_X  { 320.0f  }; //Merlin default position X
const float  Merlin_Start_Y  { -120.0f }; //Merlin default position Y
//Global Read-Only Variables END/////////////////////////////////////////////////////////

/****************************************************************************************
Description:
  This is the Merlin Boss A.I. 
****************************************************************************************/
class Merlin : public Characters {

public:

  /**************************************************************************************
  Description:
    Constructs Merlin. Only make one of this at any one time. 
  Merlin_Texture:
    Merlin's sprite. 
  Eball_Sprite:
    Sprite for the energy balls. 
  Arrow_Sprite:
    Sprite for the arrow rain attack. 
  **************************************************************************************/
  Merlin(Sprite *Merlin_Texture, Sprite *Eball_Sprite, Sprite *Arrow_Sprite) ;

  /**************************************************************************************
  Description:
    Updates Merlin.
  player:
    The player. 
  dt:
    Delta time. 
  **************************************************************************************/
  void Update(Dragon &player, const float dt)                                ;

  /**************************************************************************************
  Description:
    Renders Merlin and attacks. 
  **************************************************************************************/
  void Render()                                                              ;

  /**************************************************************************************
  Description:
    Returns boss state, use this to check if boss is attacking. 
  **************************************************************************************/
  Boss_Action_State GetState()                    const { return M_Curr      ; }

  /**************************************************************************************
  Description:
    Returns the type of attack. 
  **************************************************************************************/
  MAS GetAttack()                                 const { return M_Att_Curr  ; }

  /**************************************************************************************
  Description:
    To override mute function. 
  **************************************************************************************/
  void Mute()                                                                ;

  /**************************************************************************************
  Description:
    To override unmute function. 
  **************************************************************************************/
  void Unmute()                                                              ;

  /**************************************************************************************
  Description:
    Returns Spread Eball to check for collision. 
  **************************************************************************************/
  const std::vector<Boss_Attack> &GetSpreadBall() const { return Spread_Eball; }

  /**************************************************************************************
  Description:
    Returns arrow to check for collision. 
  **************************************************************************************/
  const std::vector<Boss_Attack> &GetArrow()      const { return Arrow       ; }

  /**************************************************************************************
  Description:
    Returns Eball to check for collision. 
  **************************************************************************************/
  const Projectile &GetEball()                    const { return Eball       ; }

  /**************************************************************************************
  Description:
    Deconstructs Merlin. 
  **************************************************************************************/
  ~Merlin()                                                                  ;

private:
  void(Merlin::*Merlin_Attack)(Dragon &player, const float dt) ; //Pointer to current attack function
  void(Merlin::*Merlin_State)(Dragon &player , const float dt) ; //Pointer to current state function

  /**************************************************************************************
  Description:
    Sets current state to next, checks for next state.
  player:
    The player.
  **************************************************************************************/
  void CheckState(Dragon &player)                              ;

  /**************************************************************************************
  Description:
    Check if can attack, and sets the appropriate function for it.
    Returns true if can attack, else false. 
  player:
    The player.
  **************************************************************************************/
  bool CheckAttack(Dragon &player)                             ;

  int castime{ 100 }      ; //Arrow rain cast time
  float Attack_Interval   ; //Time between attacks
  Boss_Action_State M_Curr; //Current state
  Boss_Action_State M_Next; //Next state
  MAS M_Att_Curr          ; //Current attack state

  Boss_Attack M_Melee                  ; //Melee attack
  Boss_Attack Eball                    ; //Projectile for engery ball
  std::vector<Boss_Attack> Spread_Eball; //Projectiles for spread shot energy ball
  std::vector<Boss_Attack> Arrow       ; //Projectiles for arrow rain

  //Blink attack struct
  struct Blink {
    float CD_Time ; //Cooldown time
    bool  Cooldown; //Check if cooldown is active
    Blink() :CD_Time{ Blink_CD_Time }, Cooldown{ true } {}
    //Update Blink
    void Update()
    {
      if (Cooldown)
      {
        CD_Time -= 0.016f;
        if (CD_Time <= 0)
        {
          Cooldown = false;
          CD_Time = Blink_CD_Time;
        }
      }
    }
  };

  Blink     Blink_     ; //Blink utilities
  Sprite    MagicCircle; //Magic circle sprite
  Transform MC_Pos     ; //Magic circle transform

  enum Anim_State {
    ANIM_HIT     ,
    ANIM_IDLE    ,
    ANIM_TELE_OUT,
    ANIM_TELE_IN
  };

  Animation Anim_;       //Animation

  //Attack Functions START///////////////////////////////////////////////////////////////
  /**************************************************************************************
  Description:
    Melee attack function.
  player:
    The player.
  dt:
    Delta time.
  **************************************************************************************/
  void Melee(Dragon &player, const float dt)         ;

  /**************************************************************************************
  Description:
    Single shot energy ball function.
  player:
    The player.
  dt:
    Delta time.
  **************************************************************************************/
  void S_Eball(Dragon &player, const float dt)       ;

  /**************************************************************************************
  Description:
    Spread shot energy ball function.
  player:
    The player.
  dt:
    Delta time.
  **************************************************************************************/
  void Sp_Eball(Dragon &player, const float dt)      ;

  /**************************************************************************************
  Description:
    Arrow rain function.
  player:
    The player.
  dt:
    Delta time.
  **************************************************************************************/
  void A_Rain(Dragon &player, const float dt)        ;

  /**************************************************************************************
  Description:
    Melee Update.
  dt:
    Delta time.
  **************************************************************************************/
  void Melee_Update(const float dt)                  ;

  /**************************************************************************************
  Description:
    Single shot update.
  dt:
    Delta time.
  **************************************************************************************/
  void S_Eball_Update(const float dt)                ;

  /**************************************************************************************
  Description:
    Spread shot update.
  dt:
    Delta time.
  **************************************************************************************/
  void Sp_Eball_Update(const float dt)               ;

  /**************************************************************************************
  Description:
    Arrow rain update.
  player:
    The player.
  dt:
    Delta time.
  **************************************************************************************/
  void A_Rain_Update(Dragon &player, const float dt) ;

  /**************************************************************************************
  Description:
    Checks if attacks hit.
  player:
    The player.
  dt:
    Delta time.
  **************************************************************************************/
  void Colision_Check(Dragon &player, const float dt);
  //Attack Functions END/////////////////////////////////////////////////////////////////

  //State Functions START///////////////////////////////////////////////////////////////
  /**************************************************************************************
  Description:
    Idle state function.
  player:
    The player.
  dt:
    Delta time.
  **************************************************************************************/
  void Idle(Dragon &player, const float dt)  ;

  /**************************************************************************************
  Description:
    Move state function.
  player:
    The player.
  dt:
    Delta time.
  **************************************************************************************/
  void Move(Dragon &player, const float dt)  ;

  /**************************************************************************************
  Description:
    Attack state function.
  player:
    The player.
  dt:
    Delta time.
  **************************************************************************************/
  void Attack(Dragon &player, const float dt);
  //State Functions END/////////////////////////////////////////////////////////////////
};
