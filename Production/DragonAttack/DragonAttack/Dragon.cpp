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
    SetVelocity(AEVec2{ Player_Speed.x * 1.5f, Player_Speed.y * 1.5f });
  }
  else
  {
    SetVelocity(Player_Speed);
  }
  if (Pickup_.INVUL)
  {
    //Make Player Invulnerable
    Set_Vulnerable(false);
  }
  else
  {
    Set_Vulnerable(true);
  }
}

void Dragon::HitInvul(const float dt)
{
  static float    Invul_CD = Invul_Time;
  static unsigned frame_count = 0;
  Set_Vulnerable(false);
  if (Invul_FLAG)
  {
    Invul_CD -= dt;
    if (frame_count & 1)
      Sprite_->SetAlphaTransBM(0.5f, 0.5f, AE_GFX_BM_BLEND);
    else
      Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  }
  if (Invul_CD <= 0.0f)
  {
    Invul_CD = Invul_Time;
    frame_count = 0;
    Set_Vulnerable(true);
    Invul_FLAG = false;
  }
}

void Dragon::Input()
{
  if (AEInputCheckCurr(AEVK_D))
  {
    Dir.R = true;
  }

  if (AEInputCheckCurr(AEVK_A))
  {
    Dir.L = true;
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
    }
  }

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
  Collision_.Update_Col_Pos(PosX - Dragon_Scale * 0.5f, PosY - Dragon_Scale, 
                            PosX + Dragon_Scale * 0.5f, PosY + Dragon_Scale);
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
