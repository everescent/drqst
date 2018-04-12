/* Start Header ************************************************************************/
/*!
\file    Archer.cpp
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Archer class fucntions defined here. 

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/

#include "Archer.h"

//Use archer macros
using namespace ArcherMacros;

//Initialize audio engine
Audio_Engine Archer::Audio_{ 1, [](std::vector<std::string> &playlist) ->void {
                                  //Archer hit sound
                                  playlist.push_back(".//Audio/Hit_01.mp3");
                                } 
                           };

//Constructs the Archer object with its sprites and position
Archer::Archer(Sprite *p_Sprite, Sprite *Arrow_Sprite, 
               const float posX, const float posY      )
  //Initialize characters class
  :Characters{ p_Sprite, Archer_HP,
               Col_Comp{ posX - Archer_Scale,
                         posY - Archer_Scale,
                         posX + Archer_Scale,
                         posY + Archer_Scale,
                         Rect 
                       } 
             },
  //Initialize Arrow
  Arrow{ Arrow_Sprite,
         Col_Comp{ posX - Arrow_Scale,
                   posY - Arrow_Scale,
                   posX + Arrow_Scale,
                   posY + Arrow_Scale,
                   Rect 
                 } 
       }, 
  //Initialize other members
  Archer_State{ &Archer::Idle }, A_Curr { IDLE  }, A_Next  { IDLE },
  Dragon_Seen { false         }, Attack_{ false }, Distance{ 0.0f }, 
  Arrow_CD    { 0.0f          },
  Anim_{ WALK_ANIM + 1, 3.0f, 5.0f, [](std::vector <Range>& Init)->void {
  Init.push_back(Range{ 0.0f, 1.0f, 0.00f, 0.00f }); //Hit
  Init.push_back(Range{ 0.0f, 1.0f, 0.33f, 0.33f }); //Idle
  Init.push_back(Range{ 0.0f, 1.0f, 0.66f, 0.66f }); //Walk
  } }
{
  SetActive(true)                                                  ;
  GameObject::Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  //Initialize position of Archer
  PosX = posX                                                      ;
  PosY = posY                                                      ;
  Transform_.SetTranslate(PosX, PosY)                              ;
  Transform_.Concat()                                              ;
  Arrow.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND)      ;
  Arrow.Transform_.Concat()                                        ;
}

//Updates the arrow projectile
void Archer::Attack_Update(Dragon &/*player*/, const float dt)
{
  //Check for active arrow
  if (Arrow.IsActive())
  {
    //Kill the arrow if more than desired range
    if (Arrow.GetDist() >= Arrow_Death)
    {
      Arrow.ResetDist()     ;
      Arrow.SetActive(false);
    }
  }
  //Activate cooldown
  else
  {
    Arrow_CD -= dt    ;
    if (Arrow_CD <= 0.0f)
    {
      Arrow_CD = 0.0f ;
      Attack_  = false;
    }
  }
  //Update arrow
  Arrow.Pos(PosX, PosY)                      ;
  Arrow.Update(dt, Arrow_Scale, false, Angle);
}

//Check for collision
void Archer::Colision_Check(Dragon &player, const float dt)
{
  //Update collision box
  GameObject::Collision_.Update_Col_Pos(PosX - Archer_Scale,
                                        PosY - Archer_Scale,
                                        PosX + Archer_Scale,
                                        PosY + Archer_Scale);
  //If Archer gets hit, decrease HP
  for (char i = 0; i < Bullet_Buffer; ++i)
    if (player.GetFireball()[i].IsActive())
      if (Collision_.Dy_Rect_Rect(player.GetFireball()[i].Collision_   ,
                                  GetVelocity()                        ,
                                  player.GetFireball()[i].GetVelocity(),
                                  dt
                                 )
        )
      {
        //Decrease HP by player's damage
        Decrease_HP(player.GetDamage())                ;
        //Add mega fireball charge
        player.AddCharge()                             ;
        player.PlayImpact()                            ;
        Audio_.Play(HIT)                               ;
        //Reset the distance of the fireball and set false
        player.GetFireball()[i].Projectile::ResetDist();
        player.GetFireball()[i].SetActive(false)       ;
      }
  //Similarly, for mega fireball 
  if(player.GetMfireball().IsActive())
    if (Collision_.Dy_Rect_Rect(player.GetMfireball().Collision_   ,
                                GetVelocity()                      ,
                                player.GetMfireball().GetVelocity(),
                                dt
                               )
      )
    {
      //Decrease HP by player's damage
      Decrease_HP(player.GetMDamage())             ;
      player.PlayImpact()                          ;
      Audio_.Play(HIT)                             ;
      //Reset the distance of the fireball and set false
      player.GetMfireball().Projectile::ResetDist();
      player.GetMfireball().SetActive(false)       ;
    }
  //Arrow collision check
  if (Attack_)
  {
    if (Arrow.IsActive())
    {
      if (player.Collision_.Dy_Rect_Rect(Arrow.Collision_    ,
                                         Arrow.GetVelocity() ,
                                         player.GetVelocity(),
                                         dt
                                        )
        )
      {
        //Damage the player
        player.Decrease_HP()  ;
        player.PlayHit()      ;
        player.SetInvul(true) ;
        Arrow.ResetDist()     ;
        Arrow.SetActive(false);
      }
    }
  }
}

//Idle state
void Archer::Idle(Dragon &/*player*/, const float /*dt*/)
{
  //Do some idle animation
  Distance = 0.0f          ;
  Anim_.SetState(IDLE_ANIM);
}

//Moving state
void Archer::Move(Dragon &player, const float dt)
{
  //Move the Archer away from player if too near
  //Archer can only move for a max distance
  Anim_.SetState(WALK_ANIM)              ;
  float Displacement = player.PosX - PosX;
  //Check for direction of displacement
  if (Displacement < 0.0f)
  {
    PosX     += -Archer_Speed * dt       ;
    Distance +=  Archer_Speed * dt       ;
  }
  else
  {
    PosX     += Archer_Speed * dt        ;
    Distance += Archer_Speed * dt        ;
  }
}

//Attack state
void Archer::Attack(Dragon &player, const float /*dt*/)
{
  //Shoots an arrow, based on player's displacement with archer
  //The displacement vector should be a parabola 
  float X{ PosX - player.PosX }    ;
  float Y{ PosY - player.PosY }    ;
  AEVec2 Disp{ -X, -Y }            ;
  Arrow.SetActive(true)            ;
  //Opposite / Adjacent
  Angle = atan(Y / X)              ;
  //Convert to degrees
  Angle *= (180.0f / PI)           ;
  //Directional check
  if (X < 0.0f)
    Angle += 180.0f                ;
  //Normalize and scale
  AEVec2Normalize(&Disp, &Disp)    ;
  AEVec2Scale(&Disp, &Disp, 420.0f);
  Arrow.SetVelocity(Disp)          ;
  Arrow_CD = Archer_CD_Time        ;
  Attack_  = true                  ;
  Distance = 0.0f                  ;
}

//Dead state
void Archer::Dead()
{
  //Add score only once
  if (IsActive())
  {
    Characters::Add_Kill_count();
    Characters::Add_Score(5)    ;
  }
  //Kill the archer
  SetActive(false)      ;
  Arrow.SetActive(false);
  Set_HP(Archer_HP)     ;
  A_Curr = IDLE         ;
}

//Check for which state to go into
void Archer::CheckState(Dragon &player, const float /*dt*/)
{
  //Check if dragon is seen
  //IF SO, check if within Line Of Sight
  //IF SO, attack, IF NOT move towards dragon and attack once in LOS
  A_Curr = A_Next;
  switch (A_Curr)
  {
  case IDLE:
    Archer_State = &Archer::Idle  ;
    break                         ;
  case MOVING:
    Archer_State = &Archer::Move  ;
    break                         ;
  case ATTACK:
    Archer_State = &Archer::Attack;
    break                         ;
  }
  if(A_Curr == ATTACK)
    A_Next = IDLE  ;
  //Only move if dragon was seen and my distance has not reached max
  if (abs(player.PosX - PosX) > Archer_LOS &&
      Distance < Archer_Max_Dist && Dragon_Seen)
  {
    A_Next = MOVING;
  }
  //Attack only if dragon is seen
  else if (!Attack_ && Dragon_Seen)
  {
    A_Next = ATTACK;
  }
  //Default is Idle
  else
  {
    A_Next = IDLE  ;
  }
}

//Mutes all audio
void Archer::Mute()
{
  Audio_.SetVolume(HIT , 0.0f);
  Audio_.SetPause (HIT , true);
}

//Unmutes all audio
void Archer::Unmute()
{
  Audio_.SetVolume(HIT , 1.0f );
  Audio_.SetPause (HIT , false);
}

//Updates archer
void Archer::Update(Dragon& player, const float dt)
{
  //Kill the archer if 0 or less health
  if (Get_HP() <= 0)
  {
    Dead()                             ;
  }
  else
  {
    //Check if Dragon within Line Of Sight
    if (abs(player.PosX - PosX) < Archer_LOS && 
        abs(player.PosY - PosY) < Archer_LOS * 0.2f)
    {
      Dragon_Seen = true               ;
    }
    if (player.PosX < PosX)
    {
      Transform_.SetScale(-1.0f, 1.0f) ;
    }
    else
    {
      Transform_.SetScale(1.0f, 1.0f)  ;
    }
    PosY -= Gravity                    ;
    //Assign state
    CheckState(player, dt)             ;
    //Execute state
    (this->*Archer_State)(player, dt)  ;
    //Update attacks
    Attack_Update(player, dt)          ;
    //Check for collision
    Colision_Check(player, dt)         ;
    Audio_.Update()                    ;
    //Concatenate all matrices
    Transform_.SetTranslate(PosX, PosY);
    Transform_.Concat()                ;
  }
  //Update animation
  Anim_.Update(*Sprite_)               ;
}

//Renders Archer and attacks
void Archer::Render()
{
  //Render Archer
  GameObject::Render();
  //Render arrow
  Arrow.Render()      ;
}

//Deletes the sprite
Archer::~Archer()
{
  delete Sprite_;
}
