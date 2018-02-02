/* Start Header ************************************************************************/
/*!
\file       Dragon.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Dragon class member function definitions here.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Dragon.h"

void Dragon::ApplyPowerUP()
{

}

void Dragon::Input()
{
  if (AEInputCheckCurr(AEVK_A))
    Dir.L = true;
  if (AEInputCheckCurr(AEVK_D))
    Dir.R = true;
  if (AEInputCheckCurr(AEVK_SPACE))
    if(PosY == 0.0f)
      Dir.UP = true;
  if (AEInputCheckTriggered(AEVK_RETURN))
    Attack = true;
}

void Dragon::Update()
{
  Input();
  if (Dir.L)
  {
    PosX -= Speed;
    Transform_.SetScale(1.0f, 1.0f).SetTranslate(PosX, PosY);
  }
  if (Dir.R)
  {
    PosX += Speed;
    Transform_.SetScale(-1.0f, 1.0f).SetTranslate(PosX, PosY);
  }
  if (Dir.UP)
    PosY += 30.0f;
  PosY -= Gravity;
  if (PosY <= 0.0f)
    PosY = 0.0f;
  //Check if attack has been made
  if (Attack)
  {
    for (int i = 0; i < 5; ++i)
      if (Fireball[i].IsActive())
      {
        if (Fireball[i].GetDist() <= 200.f)
          break;
      }
      else
      {
        Fireball[i].SetActive(true);
        if (Dir.L)
          Fireball[i].SetDir(false);
        else
          Fireball[i].SetDir(true);
        break;
      }
  }
  //Check for any active fireballs
  for (int i = 0; i < 5; ++i)
    if (Fireball[i].IsActive())
    {
      if (Fireball[i].GetDir())
      {
        Fireball[i].PosX += Fireball[i].GetVelocity().x;
        Fireball[i].Transform_.SetTranslate(Fireball[i].PosX, Fireball[i].PosY);
        Fireball[i].Collision_.Update_Col_Pos(Fireball[i].PosX, Fireball[i].PosY);
        Fireball[i].AddDist(Fireball[i].GetVelocity().x);
      }
      else
      {
        Fireball[i].PosX -= Fireball[i].GetVelocity().x;
        Fireball[i].Transform_.SetTranslate(Fireball[i].PosX, Fireball[i].PosY);
        Fireball[i].Collision_.Update_Col_Pos(Fireball[i].PosX, Fireball[i].PosY);
        Fireball[i].AddDist(Fireball[i].GetVelocity().x);
      }
    }
    else
    {
      Fireball[i].PosX = PosX;
      Fireball[i].PosY = PosY;
    }
  //Check for distance limit
  for (int i = 0; i < 5; ++i)
    if (Fireball[i].IsActive())
    {
      if (Fireball[i].GetDist() >= 800.0f)
      {
        Fireball[i].ResetDist();
        Fireball[i].SetActive(false);
      }
    }
    else
      continue;

  Attack = false;
  Dir.L = Dir.R = false;
  if (PosY >= 150.0f)
    Dir.UP = false;
}

void Dragon::Render()
{
  for (int i = 0; i < 5; ++i)
    if(Fireball[i].IsActive())
      Fireball[i].Render();
  GameObject::Render();
}