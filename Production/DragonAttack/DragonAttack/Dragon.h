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
#include "Projectile.h"
#include "Characters.h"
#include "Collision.h"
#include <vector>

//Indicates the type of power up
enum PowerUp {
  HP = 0, //Health Point +1
  DMG,    //Damage up
  SPD,    //Speed up
  INVUL   //Invul
};

class Dragon : public Characters{

public:
  void Input();  //Handles player input
  void Update(); //Updates data members based on input
  void Render();
  Dragon()
    :Characters{ S_CreateSquare(100.0f, 1.0f, 1.0f, "dragon_tmp.png"), 3,
    Col_Comp{-100.0f, -100.0f, 100.0f, 100.0f, Rect} },
    Attack{ false }, Pwr_Up{ false }, Damage{ 10 }, Charge{ 0 },
    Gravity{ 5.0f }, Speed{ 8.0f }, Dir{}, Buff{}, Fireball{},
    Mfireball{ S_CreateSquare(50.0f, 1.0f, 1.0f, "fireball.png"), 
    Col_Comp{ 0.0f, 0.0f, 50, Circle} }
  {
    Fireball.reserve(5);
    for (int i = 0; i < 5; ++i)
      Fireball.push_back(Projectile{ S_CreateSquare(50.0f, 1.0f, 1.0f, "fireball.png"),
                                    Col_Comp{ 0.0f, 0.0f, 50, Circle } });
    for (int i = 0; i < 5; ++i)
      Fireball[i].SetVelocity(AEVec2{ 10.0f, 0.0f }).Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    Mfireball.SetVelocity(AEVec2{ 5.0f, 0.0f }).Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  }
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
  std::vector<Projectile> Fireball; //Array of Fireball projectile
  Projectile Mfireball; //Mega Fireball projectile

  //Private Functions START//////////////////////////////////////////////////////////////
  void ApplyPowerUP();
  //Private Functions END////////////////////////////////////////////////////////////////
};
