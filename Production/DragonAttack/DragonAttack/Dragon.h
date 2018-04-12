/* Start Header ************************************************************************/
/*!
\file    Dragon.h
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Dragon class declared here. 

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
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
const int   MFireball_Damage{ 25      }; //Mega fireball damage
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

/****************************************************************************************
Description:
  This is the player controlled object, otherwise known as the player's class. 
  This class handles all the input by the play and updates accordingly. 
****************************************************************************************/
class Dragon : public Characters{

public:

  /**************************************************************************************
  Description:
    This function receives the player input. 
  **************************************************************************************/
  void Input()                                                                         ;

  /**************************************************************************************
  Description:
    This updates the dragon's behavior, according to player input. 
  Dragon&: 
    This is a dummy, and not used. 
  **************************************************************************************/
  void Update(Dragon&, const float dt)                                                 ;

  /**************************************************************************************
  Description:
    Adds charge to the Mega Fireball, caps at Max_Charge. 
  **************************************************************************************/
  void AddCharge()                       { Charge == Max_Charge ? Max_Charge : ++Charge; }

  /**************************************************************************************
  Description:
    Resets Mega Fireball charge.
  **************************************************************************************/
  void ResetCharge()                     { Charge = 0                                  ; }

  /**************************************************************************************
  Description:
    Set pickup type, when a pick up is picked up. OK, this sounds obvious, but yes. 
  type:
    Pick up type.
  status:
    Whether or not pickup effects have expired. 
  **************************************************************************************/
  void SetPickup(const int  type  ,
                 const bool status  )                                                  ;

  /**************************************************************************************
  Description:
    Set Invul Flag when hit. Makes the player invulanerable for a few seconds. 
  state: 
    Whether or not to activate invulnerability. 
  **************************************************************************************/
  void SetInvul(const bool state)        { Invul_FLAG  = state                         ; }

  /**************************************************************************************
  Description:
    This tells the object whether of not to update.
  state:
    Whether or not dragon can be updated. 
  **************************************************************************************/
  void SetUpdateFlag(const bool state)   { Update_Flag = state                         ; }

  /**************************************************************************************
  Description:
    This tells the object whether or not the player is touching the ground. 
  state:
    Whether or not the object has touched the ground. 
  **************************************************************************************/
  void SetTouchBottom(const bool state)  { TouchBottom = state                         ; }

  /**************************************************************************************
  Description:
    Play this when fireball made contact.
  **************************************************************************************/
  void PlayImpact()                      { SFX_.Play(IMPACT)                           ; }

  /**************************************************************************************
  Description:
    Play this when dragon gets hit. 
  **************************************************************************************/
  void PlayHit()                         { SFX_.Play(HIT)                              ; }

  /**************************************************************************************
  Description:
    Make the player face right. 
  **************************************************************************************/
  void FaceRight()                                                                     ;

  /**************************************************************************************
  Description:
    Mutes all audio.
  **************************************************************************************/
  void Mute()                                                                          ;

  /**************************************************************************************
  Description:
    Unmutes all audio.
  **************************************************************************************/
  void Unmute()                                                                        ;

  /**************************************************************************************
  Description:
    Renders the dragon.
  **************************************************************************************/
  void Render()                                                                        ;

  /**************************************************************************************
  Description:
    Get fireball damage.
  **************************************************************************************/
  char GetDamage()         const         { return Damage                               ; }

  /**************************************************************************************
  Description:
    Get mega fireball damage. 
  **************************************************************************************/
  char GetMDamage()        const         { return M_Damage                             ; }

  /**************************************************************************************
  Description:
    Jump check.
  **************************************************************************************/
  bool GetJump()                         { return Dir.UP                               ; }

  /**************************************************************************************
  Description:
    Returns Fireballs to check for collision. 
  **************************************************************************************/
  std::vector<Projectile> &GetFireball() { return Fireball                             ; }

  /**************************************************************************************
  Description:
    Returns Mega Fireball to check for collision. 
  **************************************************************************************/
  Projectile &GetMfireball()             { return Mfireball                            ; }

  /**************************************************************************************
  Description:
    Get the direction the player is facing. 
  **************************************************************************************/
  float GetFacing()        const         { return Facing                               ; }

  /**************************************************************************************
  Description:
    Returns the Invul_FLAG value. 
  **************************************************************************************/
  bool GetInvulFlag()      const         { return Invul_FLAG                           ; }

  /**************************************************************************************
  Description:
    Returns this object. 
  **************************************************************************************/
  const Dragon &Get_this() const         { return *this                                ; }

  /**************************************************************************************
  Description:
    Update_Flag check
  **************************************************************************************/
  bool GetUpdateFlag()                   { return Update_Flag                          ; }

  /**************************************************************************************
  Description:
    Gets current charge. 
  **************************************************************************************/
  int Get_Charge()                                                                     ;

  /**************************************************************************************
  Description:
    Constructs the Dragon.
  D_Sprite:
    The dragon's sprite. 
  F_Sprite:
    The fireball's srpite. 
  Pos_:
    Spawn position of the dragon. 
  **************************************************************************************/
  Dragon(Sprite* D_Sprite, Sprite *F_Sprite, AEVec2 Pos_)
    //Initialize Characters class
    :Characters{ D_Sprite,                     5,
                 Col_Comp{ Pos_.x - Dragon_Scale,
                           Pos_.y - Dragon_Scale,
                           Pos_.x + Dragon_Scale,
                           Pos_.y + Dragon_Scale,
                           Rect
                         } 
               },
    //Initialize data members
    Attack     { false            }, Pwr_Up    { false }, Falling { false           },
    TouchBottom{ true             }, Invul_FLAG{ false }, Damage  { Fireball_Damage },
    M_Damage   { MFireball_Damage }, Charge    { 0     }, Gravity { 10.0f           },
    Dir        {                  }, Pickup_   {       }, Fireball{                 },
    Update_Flag{ true             },
    //Initialize Mega Fireball
    Mfireball  { F_Sprite, 
                 Col_Comp{ Pos_.x - MFireball_Scale,
                           Pos_.y - MFireball_Scale,
                           Pos_.x + MFireball_Scale,
                           Pos_.y + MFireball_Scale, 
                           Rect 
                         } 
               }, 
    Air_Dist   { 0.0f             }, Facing    { 1.0f  },
    //Initialize Audio Engine
    SFX_       { 3, [](std::vector<std::string> &playlist) ->void {
                      playlist.push_back(".//Audio/Dragon_Hit.mp3"  ); //Dragon hit sound
                      playlist.push_back(".//Audio/Fireball_Hit.mp3"); //Fireball on impact sound
                      playlist.push_back(".//Audio/Fireball.mp3"    ); //Fireball launch sound
                    } 
               }, 
    Anim_      { Dragon_ANIM::WALK + 1, 4, 5,
                 [](std::vector <Range>& Init) -> void {
                   Init.push_back(Range{ 0.0f, 1.0f, 0.00f, 0.00f }); //Hit
                   Init.push_back(Range{ 0.0f, 1.0f, 0.25f, 0.25f }); //Idle
                   Init.push_back(Range{ 0.0f, 1.0f, 0.50f, 0.50f }); //Jump
                   Init.push_back(Range{ 0.0f, 1.0f, 0.75f, 0.75f }); //Walk
                 } 
               }
  {
    SetActive(true)                    ;
    //Initialize player start location
    PosX = Pos_.x                      ;
    PosY = Pos_.y                      ;
    //Update position of player
    Transform_.SetTranslate(PosX, PosY);
    Transform_.Concat()                ;
    //Set velocity
    SetVelocity(Player_Speed)          ;
    //Reserve a block of memory per number of bullets 
    Fireball.reserve(Bullet_Buffer)    ;
    //Initialize all the fireballs
    for (int i = 0; i < Bullet_Buffer; ++i)
      Fireball.push_back(Projectile{ F_Sprite                         ,
                                     Col_Comp{ Pos_.x - Fireball_Scale,
                                               Pos_.y - Fireball_Scale,
                                               Pos_.x + Fireball_Scale,
                                               Pos_.y + Fireball_Scale,
                                               Rect 
                                             } 
                                   }
                        );
    //Initialize fireball
    for (int i = 0; i < Bullet_Buffer; ++i)
    {
      Fireball[i].SetVelocity(AEVec2{ Bullet_Speed, 0.0f })            ;
      Fireball[i].Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    }
    //Initialize Mega Fireball
    Mfireball.SetVelocity(AEVec2{ Bullet_Speed / 2, 0.0f })        ;
    Mfireball.Transform_.SetScale(1.5f, 1.5f)                      ;
    Mfireball.Transform_.Concat()                                  ;
    Mfireball.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  }

  /**************************************************************************************
  Description:
    Deconstructs the Dragon. Clears the vector memory. 
  **************************************************************************************/
  ~Dragon()
  {
    Fireball.clear();
  }
private:

  bool  Attack     ; //Check if player is attacking
  bool  MAttack    ; //Check if player is mega attacking
  bool  Pwr_Up     ; //Check if power up is in effect
  bool  Falling    ; //Check if player is falling
  bool  TouchBottom; //Check if player has touched bottom
  bool  Invul_FLAG ; //Check if invul for getting hit needs to be activated
  bool  Update_Flag; //Checks if u can update the dragon
  char  Damage     ; //Amount of damage each fireball does
  char  M_Damage   ; //Amount of damage each mega fireball does
  int   Charge     ; //Charge for Mega Fireball
  float Gravity    ; //Gravity
  float Air_Dist   ; //Distance jumped
  float Facing     ; //Direction of player is facing

  //Determines direction 
  struct Direction {
    bool L ; //Check for left
    bool R ; //Check for right
    bool UP; //Check for jump
    Direction() :L{ false }, R{ false }, UP{ false } {}
  };

  //Determines what kind of pickup is activated
  struct Pickup {
    bool DMG  ; //Damage
    bool SPD  ; //Speed
    bool INVUL; //Invulnerability
    Pickup() :DMG{ false }, SPD{ false }, INVUL{ false } {}
  };

  //Audio enums
  enum AudioState {
    HIT   , //Dragon get hit SFX
    IMPACT, //Fireball impact SFX
    SHOOT   //Fireball shot SFX
  };

  Direction               Dir      ; //Direction variable
  Pickup                  Pickup_  ; //Type of power up
  std::vector<Projectile> Fireball ; //Array of Fireball projectile
  Projectile              Mfireball; //Mega Fireball projectile
  Audio_Engine            SFX_     ; //Dragon Sounds
  Animation               Anim_    ; //For Dragon animation

  //Private Functions START//////////////////////////////////////////////////////////////
  /**************************************************************************************
  Description:
    Puts the power ups into effect, if any. 
  **************************************************************************************/
  void ApplyPowerUP()          ;

  /**************************************************************************************
  Description:
    Makes dragon invulnerable for a short time after getting hit. 
  **************************************************************************************/
  void HitInvul(const float dt);
  //Private Functions END////////////////////////////////////////////////////////////////
};
