/* Start Header ************************************************************************/
/*!
\file       Dragon.h
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Dragon class declared here.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "GameObject.h"
/*#include "Projectile.h"*/
#include "Characters.h"
#include "Collision.h"

//Indicates the type of power up
enum PowerUp {
  HP = 0, //Health Point +1
  DMG,    //Damage up
  SPD,    //Speed up
  INVUL   //Invul
};

class Dragon : public Characters/*, public Projectile */{

public:
  void Input();  //Handles player input
  void Update(); //Updates data members based on input
  Dragon()
    :Characters{ S_CreateSquare(100.0f, 1.0f, 1.0f, "dragon_tmp.png"), 3,
    Col_Comp{-100.0f, -100.0f, 100.0f, 100.0f, Rect} },
    Attack{ false }, Pwr_Up{ false }, Damage{ 10 }, Charge{ 0 },
    Gravity{ 5.0f }, Speed{ 8.0f }, Dir{}, Buff{}
  {}
private:

  bool Attack;   //Check if player is attacking
  bool Pwr_Up;   //Check if power up is in effect
  int Damage;    //Amount of damage each attack does
  int Charge;    //Charge for Mega Fireball
  float Gravity; //Gravity
  float Speed;   //Speed of the dragon
  //Determines direction 
  struct Direction {
    bool L { false };//Check for left
    bool R { false };//Check for right
    bool UP{ false };//Check for jump
    Direction() = default;
  };
  Direction Dir; //Direction variable
  PowerUp Buff;  //Type of power up

  //Private Functions START//////////////////////////////////////////////////////////////
  void ApplyPowerUP();
  //Private Functions END////////////////////////////////////////////////////////////////
};
