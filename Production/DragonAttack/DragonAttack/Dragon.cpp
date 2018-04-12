/* Start Header ************************************************************************/
/*!
\file    Dragon.cpp
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Dragon class member function definitions here.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Dragon.h"

//Set pickup type, when a pick up is picked up. OK, this sounds obvious, but yes.
void Dragon::SetPickup(const int type, const bool status)
{
  switch (type)
  {
    case DMG:
      Pickup_.DMG   = status;
      break                 ;
    case SPD:
      Pickup_.SPD   = status;
      break                 ;
    case INVUL:
      Pickup_.INVUL = status;
      break                 ;
  }
}

//Puts the power ups into effect, if any
void Dragon::ApplyPowerUP()
{
  if (Pickup_.DMG)
  {
    //Increase DMG
    Damage = Fireball_Damage + 10;
  }
  else
  {
    //Revert increase in damage
    Damage = Fireball_Damage     ;
  }
  if (Pickup_.SPD)
  {
    //Increase SPD
    SetVelocity(AEVec2{ Player_Speed.x * 1.5f, Player_Speed.y });
  }
  else
  {
    //Revert increase speed
    SetVelocity(Player_Speed                                   );
  }
  if (Pickup_.INVUL)
  {
    //Make Player Invulnerable
    Set_Vulnerable(false                                );
    Sprite_->SetAlphaTransBM(0.8f, 0.8f, AE_GFX_BM_BLEND);
  }
  else
  {
    //Make player vulberable
    Set_Vulnerable(true                                 );
    Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  }
}

//Makes dragon invulnerable for a short time after getting hit
void Dragon::HitInvul(const float dt)
{
  //Invulnerable time frame
  static float Invul_CD    = Invul_Time;
  //To switch between semi transparent and opaque states
  static float frame_count = 0.3f      ;
  //Always vulnerable unless invulnerability is activated
  Set_Vulnerable(true)                 ;
  //If invulnerability is activated
  if (Invul_FLAG)
  {
    //Make invulnerable
    Set_Vulnerable(false);
    Invul_CD    -= dt    ;
    frame_count -= dt    ;
    //Count the time to switch opcaity states
    if (frame_count <= 0.0f)
      Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    else if (frame_count >= 0.1f)
      Sprite_->SetAlphaTransBM(0.8f, 0.8f, AE_GFX_BM_BLEND);
    if (frame_count <= 0)
      frame_count = 0.3f                                   ;
  }
  //When invulnerability time frame is up
  if (Invul_CD <= 0.0f)
  {
    //Reset everything
    Invul_CD    = Invul_Time;
    frame_count = 0.3f      ;
    Set_Vulnerable(true)    ;
    Invul_FLAG  = false     ;
  }
}

//This function receives the player input
void Dragon::Input()
{
  //Cehck for "Right" movement key
  if (AEInputCheckCurr(AEVK_D))
  {
    Dir.R = true;
    Set_Direction(RIGHT);
    if(Anim_.GetComplete(Dragon_ANIM::HIT))
      Anim_.SetState(Dragon_ANIM::WALK);
  }
  //Check for "Left" movement key
  else if (AEInputCheckCurr(AEVK_A))
  {
    Dir.L = true;
    Set_Direction(LEFT);
    if (Anim_.GetComplete(Dragon_ANIM::HIT))
      Anim_.SetState(Dragon_ANIM::WALK);
  }
  //If not idle
  else
  {
    Anim_.SetState(Dragon_ANIM::IDLE);
  }
  //Check for "Attack" key
  if (AEInputCheckTriggered(AEVK_RETURN))
  {
    if (!SFX_.GetPlaying(0))
      SFX_.Play(SHOOT);
    Attack = true;
  }
  //Check for "Special Attack" key
  if (AEInputCheckTriggered(AEVK_RSHIFT) || 
      AEInputCheckTriggered(AEVK_LSHIFT))
  {
    if (Charge == Max_Charge)
     {
       MAttack = true;
       Mfireball.SetActive(true);
       ResetCharge();
     }
  }
  //Check for "Jump" key
  if (AEInputCheckCurr(AEVK_SPACE))
  {
    if (!Falling)
    {
      Dir.UP = true;
      TouchBottom = false;
      if (Anim_.GetComplete(Dragon_ANIM::HIT))
        Anim_.SetState(Dragon_ANIM::JUMP);
    }
  }
  //Check if truly idle
  if(!Dir.L && !Dir.R && !Dir.UP && 
      Anim_.GetComplete(Dragon_ANIM::HIT ) && 
      Anim_.GetComplete(Dragon_ANIM::JUMP))
    Anim_.SetState(Dragon_ANIM::IDLE);
}


//Mutes all audio
void Dragon::Mute()
{
  SFX_.SetVolume(HIT   , 0.0f);
  SFX_.SetVolume(IMPACT, 0.0f);
  SFX_.SetVolume(SHOOT , 0.0f);
  SFX_.SetPause (HIT   , true);
  SFX_.SetPause (IMPACT, true);
  SFX_.SetPause (SHOOT , true);
}

//Unmutes all audio
void Dragon::Unmute()
{
  SFX_.SetVolume(HIT   , 1.0f );
  SFX_.SetVolume(IMPACT, 1.0f );
  SFX_.SetVolume(SHOOT , 1.0f );
  SFX_.SetPause (HIT   , false);
  SFX_.SetPause (IMPACT, false);
  SFX_.SetPause (SHOOT , false);
}

//This updates the dragon's behavior, according to player input
void Dragon::Update(Dragon& dummy, const float dt)
{
  UNREFERENCED_PARAMETER(dummy);
  
  //Call for player input
  Input();
  //Update position based on direction
  if (Dir.L && Get_Direction() == LEFT)
  {
    PosX  -= GetVelocity().x * dt    ;
    Facing = -1.0f                   ;
    Set_Direction(LEFT)              ;
    Transform_.SetScale(Facing, 1.0f);
  }
  else if (Dir.R && Get_Direction() == RIGHT)
  {
    PosX  += GetVelocity().x * dt    ;
    Facing = 1.0f                    ;
    Set_Direction(RIGHT)             ;
    Transform_.SetScale(Facing, 1.0f);
  }
  if (Dir.UP)
  {
    PosY     += GetVelocity().y * dt ;
    Air_Dist += GetVelocity().y * dt ;
  }
  //Apply gravity
  PosY -= Gravity                    ;
  SFX_.Update()                      ;
  //Update position of player
  Transform_.SetTranslate(PosX, PosY);
  Transform_.Concat()                ;
  //Update collision box
  if(Facing < 0.0f)
    Collision_.Update_Col_Pos(PosX - Dragon_Scale       ,
                              PosY - Dragon_Scale       ,
                              PosX + Dragon_Scale * 0.9f,
                              PosY + Dragon_Scale         );
  else
    Collision_.Update_Col_Pos(PosX - Dragon_Scale * 0.9f,
                              PosY - Dragon_Scale       ,
                              PosX + Dragon_Scale       ,
                              PosY + Dragon_Scale         );
  //Apply any power ups
  ApplyPowerUP()                                           ;
  //Apply any invulnerability due to hit
  HitInvul(dt)                                             ;
  //Check if attack has been made
  if (Attack)
  {
    //Check for active bullets, less than the required interval
    for (int i = 0; i < Bullet_Buffer; ++i)
      if (Fireball[i].IsActive())
      {
        if (Fireball[i].GetDist() <= Bullet_Interval)
          break;
      }
      else
      {
        Fireball[i].SetActive(true);
        if (Facing < 0.0f)
          Fireball[i].SetDir(false);
        else
          Fireball[i].SetDir(true);
        break;
      }
  }
  //Check if special attack has been made
  if (MAttack)
  {
    if (Mfireball.IsActive())
      if (Facing < 0.0f)
        Mfireball.SetDir(false);
      else
        Mfireball.SetDir(true);
  }
  //Update fireballs
  for (int i = 0; i < Bullet_Buffer; ++i)
  {
    Fireball[i].Pos(PosX, PosY)           ;
    Fireball[i].Update(dt, Fireball_Scale);
  }
  //Update Mega Fireball
  Mfireball.Pos(PosX, PosY)               ;
  Mfireball.Update(dt, MFireball_Scale)   ;
  //Check for distance limit (Fireball)
  for (int i = 0; i < Bullet_Buffer; ++i)
    if (Fireball[i].IsActive())
    {
      if (Fireball[i].GetDist() >= Bullet_Death)
      {
        Fireball[i].ResetDist()     ;
        Fireball[i].SetActive(false);
      }
    }
    else
      continue;
  //Check for distance limit (Mega Fireball)
  if (Mfireball.IsActive())
  {
    if (Mfireball.GetDist() >= Bullet_Death)
    {
      Mfireball.ResetDist()     ;
      Mfireball.SetActive(false);
    }
  }
  //Update animation
  Anim_.Update(*Sprite_)        ;
  //Reset direction and attack
  Attack        = false;
  MAttack       = false;
  Dir.L = Dir.R = false;
  //Check for maximum jump height
  if (Air_Dist >= Jump_Height)
  {
    Falling = true ;
    Dir.UP  = false;
  }
  //Deactivate jump mode after landing
  if (Falling)
  {
    Air_Dist     -= Gravity            ;
    if (Air_Dist <= 0.0f && TouchBottom)
    {
      Falling     = false              ;
      TouchBottom = false              ;
      Anim_.SetState(Dragon_ANIM::IDLE);
    }
  }
}

//Redners the dragon
void Dragon::Render()
{
  //Render all active fireballs
  for (int i = 0; i < Bullet_Buffer; ++i)
    Fireball[i].Render();
  //Render active mega fireball
  Mfireball.Render();
  //Redner the Dragon
  GameObject::Render();
}

//Gets the charge of the mega fireball
int Dragon::Get_Charge()
{
  return Charge;
}
