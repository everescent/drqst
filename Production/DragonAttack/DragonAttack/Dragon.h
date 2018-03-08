/* Start Header ************************************************************************/
/*!
\file       Dragon.h
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Dragon class declared here.

Copyright (C) 2018 DigiPen Institute of Technology.
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
#include "PickUp.h"
#include "Audio_Engine.h"
#include <vector>

const char  Max_Charge      { 10      }; //Maximum charge for mega fireball
const int   Bullet_Buffer   { 10      }; //Amount of fireballs at any one time
const float Bullet_Interval { 400.0f  }; //Minimum distance between each fireball
const float Bullet_Death    { 900.0f  }; //Distance when bullet dies
const float Bullet_Speed    { 1200.0f }; //How fast a bullet travels
const int   Fireball_Damage { 10      };
const int   MFireball_Damage{ 15      };

const float Jump_Height    { 400.0f  }; //Maximum height player can jump
const float Jump_Mult      { 3.2f    }; //How fast player can jump
const float Start_Pos_X    { -320.0f }; //Player stating position X
const float Start_Pos_Y    { -100.0f }; //Player starting position Y
const AEVec2 Player_Speed  { 480.0f, 480.0f * Jump_Mult };
const float Cam_Offset_X   { 320.0f  }; //Camera offset X
const float Cam_Offset_Y   { 120.0f  }; //Camera offset Y

class Dragon : public Characters{

public:
  //Handles player input
  void Input();
  //Don't need to use dt
  void Update(Dragon&, const float dt);
  //Adds charge to the Mega Fireball, caps at Max_Charge
  void AddCharge() { Charge == Max_Charge ? Max_Charge : ++Charge; }
  //Resets Mega Fireball charge
  void ResetCharge() { Charge = 0; }
  //Set pickup type
  void SetPickup(const int type, const bool status);
  //Play this when firebal made contact
  void PlayImpact() { SFX_.Play(IMPACT); }
  //Play this when dragon gets hit
  void PlayHit() { SFX_.Play(HIT); }
  //Renders the dragon
  void Render(); 
  //Get fireball damage
  char GetDamage() const { return Damage; }
  //Get mega fireball damage
  char GetMDamage() const { return M_Damage; }
  //Jump check
  bool GetJump() { return Dir.UP; }
  //Returns Fireballs to check for collision
  std::vector<Projectile> &GetFireball() { return Fireball; }
  //Returns Mega Fireball to check for collision
  Projectile &GetMfireball() { return Mfireball; }
  //Get the direction the player is facing
  float GetFacing() const { return Facing; }
  //Returns this pos
  const Dragon &Get_this() const { return *this; }
  //Gets current charge
  int Get_Charge();

  Dragon()
    //Initialize Characters class
    :Characters{ S_CreateSquare(70.0f, ".//Textures/Bob.png"), 3,
    Col_Comp{ Start_Pos_X - 70.0f, Start_Pos_Y - 70.0f,
              Start_Pos_X + 70.0f, Start_Pos_Y + 70.0f, Rect} },
    //Initialize data members
    Attack{ false }, Pwr_Up{ false }, Falling{ false }, Damage { Fireball_Damage },
    M_Damage{ MFireball_Damage }, Charge{ 0 }, Gravity{ 10.0f }, Dir{}, Pickup_{}, Fireball{},
    //Initialize Mega Fireball
    Mfireball{ S_CreateSquare(70.0f, ".//Textures/Fireball.png"), 
    Col_Comp{ Start_Pos_X - 50.0f, Start_Pos_Y - 50.0f,
    Start_Pos_X + 50.0f, Start_Pos_Y + 50.0f, Rect } }, Air_Dist{ 0.0f }, Facing{ 1.0f },
    //Initialize Audio Engine
    SFX_{ 3, [](std::vector<std::string> &playlist) ->void {
      playlist.push_back(".//Audio/Dragon_Hit.mp3");
      playlist.push_back(".//Audio/Fireball_Hit.mp3"); 
      playlist.push_back(".//Audio/Fireball.mp3");     } }
  {
    //Initialize player start location
    PosX = Start_Pos_X;
    PosY = Start_Pos_Y;
    //Set velocity
    SetVelocity(Player_Speed);
    //Reserve a bloack of memory per number of bullets 
    Fireball.reserve(Bullet_Buffer);
    //Initialize all the fireballs
    for (int i = 0; i < Bullet_Buffer; ++i)
      Fireball.push_back(Projectile{ S_CreateSquare(50.0f, ".//Textures/Fireball.png"),
                                     Col_Comp{ Start_Pos_X - 50.0f, Start_Pos_Y - 50.0f,
                                     Start_Pos_X + 50.0f, Start_Pos_Y + 50.0f, Rect } });
    for (int i = 0; i < Bullet_Buffer; ++i)
    {
      Fireball[i].SetVelocity(AEVec2{ Bullet_Speed, 0.0f });
      Fireball[i].Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    }
    //Initialize Mega Fireball
    Mfireball.SetVelocity(AEVec2{ Bullet_Speed / 2, 0.0f });
    Mfireball.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  }
  ~Dragon()
  {
    Fireball.clear();
  }
private:

  bool  Attack;    //Check if player is attacking
  bool  MAttack;   //Check if player is mega attacking
  bool  Pwr_Up;    //Check if power up is in effect
  bool  Falling;   //Check if player is falling
  char  Damage;    //Amount of damage each fireball does
  char  M_Damage;  //Amount of damage each mega fireball does
  int   Charge;    //Charge for Mega Fireball
  float Gravity;   //Gravity
  float Air_Dist;  //Distance jumped
  float Facing;    //Direction of player is facing
  //Determines direction 
  struct Direction {
    bool L;  //Check for left
    bool R;  //Check for right
    bool UP; //Check for jump
    Direction() :L{ false }, R{ false }, UP{ false } {};
  };
  //Determines what kind of pickup is activated
  struct Pickup {
    bool DMG;   //Damage
    bool SPD;   //Speed
    bool INVUL; //Invulnerability
    Pickup() :DMG{ false }, SPD{ false }, INVUL{ false } {};
  };
  enum AudioState {
    HIT,    //Dragon get hit SFX
    IMPACT, //Fireball impact SFX
    SHOOT   //Fireball shot SFX
  };
  Direction               Dir;       //Direction variable
  Pickup                  Pickup_;   //Type of power up
  std::vector<Projectile> Fireball;  //Array of Fireball projectile
  Projectile              Mfireball; //Mega Fireball projectile
  Audio_Engine            SFX_;      //Dragon Sounds
  //Private Functions START//////////////////////////////////////////////////////////////
  void ApplyPowerUP();
  //Private Functions END////////////////////////////////////////////////////////////////
};
