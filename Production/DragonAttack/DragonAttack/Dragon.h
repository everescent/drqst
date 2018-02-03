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
#include <utility>

//Indicates the type of power up
enum PowerUp {
  HP = 0, //Health Point +1
  DMG,    //Damage up
  SPD,    //Speed up
  INVUL   //Invul
};

const int   Bullet_Buffer  { 10      }; //Amount of fireballs at any one time
const float Bullet_Interval{ 600.0f  }; //Minimum distance between each fireball
const float Bullet_Death   { 900.0f  }; //Distance when bullet dies
const float Bullet_Speed   { 20.0f   }; //How fast a bullet travels

const float Jump_Height    { 200.0f  }; //Maximum height player can jump
const float Jump_Mult      { 1.2f    }; //How fast player can jump
const float Start_Pos_X    { -320.0f }; //Player stating position X
const float Start_Pos_Y    { -120.0f }; //Player starting position Y

static float Cam_Offset_X  { 320.0f  };
static float Cam_Offset_Y  { 120.0f  };

class Dragon : public Characters{

public:
  void Input();  //Handles player input
  void Update(); //Updates data members based on input
  void Update(float dt) { UNREFERENCED_PARAMETER(dt); }
  void Render();
  const std::vector<Projectile> &&GetFireball() const { return std::move(Fireball); }
  const Projectile &&GetMfireball() const { return std::move(Mfireball); }
  float GetFacing() const { return Facing; }
  Dragon()
    :Characters{ S_CreateSquare(100.0f, 1.0f, 1.0f, "dragon_tmp.png"), 3,
    Col_Comp{-100.0f, -100.0f, 100.0f, 100.0f, Rect} },
    Attack{ false }, Pwr_Up{ false }, Damage{ 10 }, Charge{ 0 },
    Gravity{ 10.0f }, Speed{ 8.0f }, Dir{}, Buff{}, Fireball{},
    Mfireball{ S_CreateSquare(50.0f, 1.0f, 1.0f, "fireball.png"), 
    Col_Comp{ Start_Pos_X, Start_Pos_Y, 50, Circle} }, Air_Dist{ 0.0f }, Facing{ 1.0f }
  {
    PosX = Start_Pos_X;
    PosY = Start_Pos_Y;
    Fireball.reserve(Bullet_Buffer);
    for (int i = 0; i < Bullet_Buffer; ++i)
      Fireball.push_back(Projectile{ S_CreateSquare(50.0f, 1.0f, 1.0f, "fireball.png"),
                                    Col_Comp{ Start_Pos_X, Start_Pos_Y, 50, Circle } });
    for (int i = 0; i < Bullet_Buffer; ++i)
    {
      Fireball[i].SetVelocity(AEVec2{ Bullet_Speed, 0.0f });
      Fireball[i].Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    }
    Mfireball.SetVelocity(AEVec2{ Bullet_Speed / 2, 0.0f });
    Mfireball.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  }
private:

  bool Attack;    //Check if player is attacking
  bool Pwr_Up;    //Check if power up is in effect
  int Damage;     //Amount of damage each attack does
  int Charge;     //Charge for Mega Fireball
  float Gravity;  //Gravity
  float Speed;    //Speed of the dragon
  float Air_Dist; //Distance jumped
  float Facing;   //Direction of player is facing
  //Determines direction 
  struct Direction {
    bool L { false }; //Check for left
    bool R { false }; //Check for right
    bool UP{ false }; //Check for jump
    Direction() = default;
  };
  Direction Dir;                    //Direction variable
  PowerUp Buff;                     //Type of power up
  std::vector<Projectile> Fireball; //Array of Fireball projectile
  Projectile Mfireball;             //Mega Fireball projectile

  //Private Functions START//////////////////////////////////////////////////////////////
  void ApplyPowerUP();
  //Private Functions END////////////////////////////////////////////////////////////////
};
