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
#include "Animation.h"
#include "PickUp.h"
#include "Audio_Engine.h"
#include <vector>

const char  Max_Charge      { 10      }; //Maximum charge for mega fireball
const int   Bullet_Buffer   { 10      }; //Amount of fireballs at any one time
const float Bullet_Interval { 600.0f  }; //Minimum distance between each fireball
const float Bullet_Death    { 900.0f  }; //Distance when bullet dies
const float Bullet_Speed    { 1200.0f }; //How fast a bullet travels
const int   Fireball_Damage { 10      }; //Fireball damage
const int   MFireball_Damage{ 15      }; //Mega fireball damage
const float Fireball_Scale  { 50.0f   }; //Scale of Fireball
const float MFireball_Scale { 75.0f   }; //Scale of Mega Firaball

const float Invul_Time      { 1.5f    }; //Time dragon is invul after being hit
const float Dragon_Scale    { 70.0f   }; //Dragon scale
const float Jump_Height     { 400.0f  }; //Maximum height player can jump
const float Jump_Mult       { 3.2f    }; //How fast player can jump
//Player velocity
const AEVec2 Player_Speed   { 480.0f, 480.0f * Jump_Mult };

//Animation state identifier
namespace Dragon_ANIM{
  typedef enum Dragon_Animation_States {
    HIT , //0
    IDLE, //1
    JUMP, //2
    WALK  //3
  }DAS;
}

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
  //Set Invul Flag when hit
  void SetInvul(const bool state) { Invul_FLAG = state; }
  void SetTouchBottom(const bool state) { TouchBottom = state; }
  //Play this when firebal made contact
  void PlayImpact() { SFX_.Play(IMPACT); }
  //Play this when dragon gets hit
  void PlayHit() { SFX_.Play(HIT); }
  //Mutes all audio
  void Mute();
  //Unmutes all audio
  void Unmute();
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
  //Returns the Invul_FLAG value
  bool GetInvulFlag() const { return Invul_FLAG; }
  //Returns this pos
  const Dragon &Get_this() const { return *this; }
  //Gets current charge
  int Get_Charge();

  Dragon(Sprite* D_Sprite, Sprite *F_Sprite, AEVec2 Pos_)
    //Initialize Characters class
    :Characters{ D_Sprite, 3,
    Col_Comp{ Pos_.x - Dragon_Scale, Pos_.y - Dragon_Scale,
    Pos_.x + Dragon_Scale, Pos_.y + Dragon_Scale, Rect} },
    //Initialize data members
    Attack{ false }, Pwr_Up{ false }, Falling{ false }, TouchBottom{ true }, Invul_FLAG{ false },
    Damage { Fireball_Damage }, M_Damage{ MFireball_Damage }, Charge{ 0 }, Gravity{ 10.0f }, 
    Dir{}, Pickup_{}, Fireball{},
    //Initialize Mega Fireball
    Mfireball{ F_Sprite, Col_Comp{ Pos_.x - MFireball_Scale, Pos_.y - MFireball_Scale,
    Pos_.x + MFireball_Scale, Pos_.y + MFireball_Scale, Rect } }, Air_Dist{ 0.0f }, Facing{ 1.0f },
    //Initialize Audio Engine
    SFX_{ 3, [](std::vector<std::string> &playlist) ->void {
      playlist.push_back(".//Audio/Dragon_Hit.mp3");
      playlist.push_back(".//Audio/Fireball_Hit.mp3"); 
      playlist.push_back(".//Audio/Fireball.mp3");     } }, 
    Anim_{ Dragon_ANIM::WALK + 1, 4, 5, [](std::vector <Range>& Init) -> void {
      Init.push_back(Range{ 0.0f, 1.0f, 0.00f, 0.00f }); //Hit
      Init.push_back(Range{ 0.0f, 1.0f, 0.25f, 0.25f }); //Idle
      Init.push_back(Range{ 0.0f, 1.0f, 0.50f, 0.50f }); //Jump
      Init.push_back(Range{ 0.0f, 1.0f, 0.75f, 0.75f }); //Walk
      } }
  {
    SetActive(true);
    //Initialize player start location
    PosX = Pos_.x;
    PosY = Pos_.y;
    //Update position of player
    Transform_.SetTranslate(PosX, PosY);
    Transform_.Concat();
    //Set velocity
    SetVelocity(Player_Speed);
    //Reserve a block of memory per number of bullets 
    Fireball.reserve(Bullet_Buffer);
    //Initialize all the fireballs
    for (int i = 0; i < Bullet_Buffer; ++i)
      Fireball.push_back(Projectile{ F_Sprite,
                                     Col_Comp{ Pos_.x - Fireball_Scale, Pos_.y - Fireball_Scale,
        Pos_.x + Fireball_Scale, Pos_.y + Fireball_Scale, Rect } });
    for (int i = 0; i < Bullet_Buffer; ++i)
    {
      Fireball[i].SetVelocity(AEVec2{ Bullet_Speed, 0.0f });
      Fireball[i].Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    }
    //Initialize Mega Fireball
    Mfireball.SetVelocity(AEVec2{ Bullet_Speed / 2, 0.0f });
    Mfireball.Transform_.SetScale(1.5f, 1.5f);
    Mfireball.Transform_.Concat();
    Mfireball.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
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
  bool TouchBottom;
  bool  Invul_FLAG;
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
  Animation               Anim_;     //For Dragon animation
  //Private Functions START//////////////////////////////////////////////////////////////
  void ApplyPowerUP();
  //Makes dragon invulnerable for a short time after getting hit
  void HitInvul(const float dt);
  //Private Functions END////////////////////////////////////////////////////////////////
};
