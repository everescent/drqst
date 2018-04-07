/* Start Header ************************************************************************/
/*!
\file       Dragon.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Dragon class member function definitions here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Dragon.h"

void Dragon::SetPickup(const int type, const bool status)
{
  switch (type)
  {
    case DMG:
      Pickup_.DMG = status;
      break;
    case SPD:
      Pickup_.SPD = status;
      break;
    case INVUL:
      Pickup_.INVUL = status;
      break;
  }
}

void Dragon::ApplyPowerUP()
{
  if (Pickup_.DMG)
  {
    //Increase DMG
    Damage = Fireball_Damage + 10;
  }
  else
  {
    Damage = Fireball_Damage;
  }
  if (Pickup_.SPD)
  {
    //Increase SPD
    SetVelocity(AEVec2{ Player_Speed.x * 1.5f, Player_Speed.y });
  }
  else
  {
    SetVelocity(Player_Speed);
  }
  if (Pickup_.INVUL)
  {
    //Make Player Invulnerable
    Set_Vulnerable(false);
    Sprite_->SetAlphaTransBM(0.8f, 0.8f, AE_GFX_BM_BLEND);
  }
  else
  {
    Set_Vulnerable(true);
    Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  }
}

void Dragon::HitInvul(const float dt)
{
  static float    Invul_CD = Invul_Time;
  static float    frame_count = 0.3f;
  Set_Vulnerable(true);
  if (Invul_FLAG)
  {
    Set_Vulnerable(false);
    Invul_CD -= dt;
    frame_count -= dt;
    if (frame_count <= 0.0f)
      Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    else if (frame_count >= 0.1f)
      Sprite_->SetAlphaTransBM(0.8f, 0.8f, AE_GFX_BM_BLEND);
    if (frame_count <= 0)
      frame_count = 0.3f;
  }
  if (Invul_CD <= 0.0f)
  {
    Invul_CD = Invul_Time;
    frame_count = 0.3f;
    Set_Vulnerable(true);
    Invul_FLAG = false;
  }
}

void Dragon::Input()
{
  if (AEInputCheckCurr(AEVK_D))
  {
    Dir.R = true;
    if(Anim_.GetComplete(Dragon_ANIM::HIT))
      Anim_.SetState(Dragon_ANIM::WALK);
  }
  else if (AEInputCheckCurr(AEVK_A))
  {
    Dir.L = true;
    if (Anim_.GetComplete(Dragon_ANIM::HIT))
      Anim_.SetState(Dragon_ANIM::WALK);
  }
  else
  {
    Anim_.SetState(Dragon_ANIM::IDLE);
  }

  if (AEInputCheckTriggered(AEVK_RETURN))
  {
    if (!SFX_.GetPlaying(0))
      SFX_.Play(SHOOT);
    Attack = true;
  }

  if (AEInputCheckTriggered(AEVK_P))
  {
    if (Charge == Max_Charge)
     {
       MAttack = true;
       Mfireball.SetActive(true);
       ResetCharge();
     }
  }

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
  if(!Dir.L && !Dir.R && !Dir.UP && Anim_.GetComplete(Dragon_ANIM::HIT) && 
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


void Dragon::Update(Dragon& dummy, const float dt)
{
  UNREFERENCED_PARAMETER(dummy);
  //Call for player input
  Input();
  //Update position based on direction
  if (Dir.L)
  {
    PosX -= GetVelocity().x * dt;
    Facing = -1.0f;
    Set_Direction(LEFT);
    Transform_.SetScale(Facing, 1.0f);
  }
  else if (Dir.R)
  {
    PosX += GetVelocity().x * dt;
    Facing = 1.0f;
    Set_Direction(RIGHT);
    Transform_.SetScale(Facing, 1.0f);
  }
  if (Dir.UP)
  {
    PosY += GetVelocity().y * dt;
    Air_Dist += GetVelocity().y * dt;
  }
  PosY -= Gravity;
  SFX_.Update();
  //Update position of player
  Transform_.SetTranslate(PosX, PosY);
  Transform_.Concat();
  if(Facing < 0.0f)
    Collision_.Update_Col_Pos(PosX - Dragon_Scale, PosY - Dragon_Scale,
                              PosX + Dragon_Scale * 0.9f, PosY + Dragon_Scale);
  else
    Collision_.Update_Col_Pos(PosX - Dragon_Scale * 0.9f, PosY - Dragon_Scale,
                              PosX + Dragon_Scale, PosY + Dragon_Scale);
  ApplyPowerUP();
  HitInvul(dt);
  //Check if attack has been made
  if (Attack)
  {
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
    Fireball[i].Pos(PosX, PosY);
    Fireball[i].Update(dt, Fireball_Scale);
  }
  //Update Mega Fireball
  Mfireball.Pos(PosX, PosY);
  Mfireball.Update(dt, MFireball_Scale);
  //Check for distance limit (Fireball)
  for (int i = 0; i < Bullet_Buffer; ++i)
    if (Fireball[i].IsActive())
    {
      if (Fireball[i].GetDist() >= Bullet_Death)
      {
        Fireball[i].ResetDist();
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
      Mfireball.ResetDist();
      Mfireball.SetActive(false);
    }
  }
  Anim_.Update(*Sprite_);
  //Reset direction and attack
  Attack = false;
  MAttack = false;
  Dir.L = Dir.R = false;
  if (Air_Dist >= Jump_Height)
  {
    Falling = true;
    Dir.UP = false;
  }
  if (Falling)
  {
    Air_Dist -= Gravity;
    if (Air_Dist <= 0.0f && TouchBottom)
    {
      Falling = false;
      TouchBottom = false;
      Anim_.SetState(Dragon_ANIM::IDLE);
    }
  }
}

void Dragon::Render()
{
  for (int i = 0; i < Bullet_Buffer; ++i)
    Fireball[i].Render();
  Mfireball.Render();
  GameObject::Render();
}

int Dragon::Get_Charge()
{
  return Charge;
}
