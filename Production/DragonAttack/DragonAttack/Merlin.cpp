/* Start Header ************************************************************************/
/*!
\file       Merlin.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Merlin class member functions defined here.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Merlin.h"
#include <ctime>
#include <cstdlib>
#include <iostream>

Merlin::Merlin()
//Initialize characters class
  :Characters{ S_CreateSquare(100.0f, 1.0f, 1.0f, "merlin.png"), 100,
  Col_Comp{ Merlin_Start_X - 100.0f, Merlin_Start_Y - 100.0f,
  Merlin_Start_X + 100.0f, Merlin_Start_Y + 100.0f, Rect } },
  //Initialize data members
  Merlin_Attack{ &Merlin::Melee }, Merlin_State{ &Merlin::Idle },
  M_Curr{ IDLE }, M_Next{ IDLE }, M_Att_Curr{ MELEE },
  //All Boss Attacks to be properly initialized later
  M_Melee{ Sprite{}, Col_Comp{} },
  Eball{ Sprite{ S_CreateSquare(50.0f, 1.0f, 1.0f, "energyball.png") },
         Col_Comp{ Merlin_Start_X, Merlin_Start_Y, 50, Circle } },
  Spread_Eball{}, Arrow{}, Blink_{}, Attack_Interval{ 0 },
  MagicCircle{ S_CreateSquare(150.0f, 1.0f, 1.0f, "magic_circle.png") }
{
  //Set spawn position
  PosX = Merlin_Start_X;
  PosY = Merlin_Start_Y;
  Transform_.SetTranslate(PosX, PosY);
  Transform_.Concat();
  Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  //Initialize Magic Circle
  MagicCircle.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  //Melee has no special render so always active
  M_Melee.SetActive(true);
  //Initialize Single Energy Ball
  Eball.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  Eball.cooldown_timer = Eball_CD_Time;
  //Initialize Spread Shot
  Spread_Eball.reserve(3);
  for (int i = 0; i < 3; ++i)
    Spread_Eball.push_back(Boss_Attack{ S_CreateSquare(30.0f, 1.0f, 1.0f, "energyball.png"), 
                           Col_Comp{ Merlin_Start_X, Merlin_Start_Y, 30, Circle } });
  for (int i = 0; i < 3; ++i)
    Spread_Eball[i].Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  //Initialize Arrow Rain
  Arrow.reserve(A_Rain_Buffer);
  for (int i = 0; i < A_Rain_Buffer; ++i)
    Arrow.push_back(Boss_Attack{ S_CreateSquare(30.0f, 1.0f, 1.0f, "arrow.png"),
      Col_Comp{ Merlin_Start_X, Merlin_Start_Y, 30, Circle } });
  for (int i = 0; i < A_Rain_Buffer; ++i)
  {
    Arrow[i].Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    Arrow[i].SetVelocity(AEVec2{ 0.0f, -20.0f });
  }
}

void Merlin::Idle(const Dragon &player)
{
  UNREFERENCED_PARAMETER(player);
  /*Do some idle animation or something idk yet*/
}

void Merlin::Move(const Dragon &player)
{
  UNREFERENCED_PARAMETER(player);
  /*This will move merlin to the 3 different spots*/
  srand((unsigned int)time(nullptr));
  int RNG = rand() % 100;

  if (RNG < 33 && PosX != Blink_Pos_1.x)
  {
    PosX = Blink_Pos_1.x;
    PosY = Blink_Pos_1.y;
  }
  else if (RNG >= 33 && RNG < 66 &&
    PosX != Blink_Pos_2.x)
  {
    PosX = Blink_Pos_2.x;
    PosY = Blink_Pos_2.y;
  }
  else if (RNG >= 66 && PosX != Blink_Pos_3.x)
  {
    PosX = Blink_Pos_3.x;
    PosY = Blink_Pos_3.y;
  }
  else
  {
    PosX = Merlin_Start_X;
    PosY = Merlin_Start_Y;
  }

  Transform_.SetTranslate(PosX, PosY);
  Transform_.Concat();

  Blink_.Cooldown = true;
  Blink_.CD_Time = Blink_CD_Time;
}

void Merlin::Melee(const Dragon &player)
{
  UNREFERENCED_PARAMETER(player);
  std::cout << "Melee\n";
  M_Melee.cooldown = true;
}

void Merlin::S_Eball(const Dragon &player)
{
  std::cout << "Single Shot Energy Ball\n";
  //Get the displacement betwwen player and Merlin
  AEVec2 Displacement{ (player.PosX - PosX ),
                       (player.PosY - PosY) };
  AEVec2Normalize(&Displacement, &Displacement);
  AEVec2Scale(&Displacement, &Displacement, 6.0f);
  Eball.SetActive(true);
  //Go towards player
  Eball.SetVelocity(Displacement);
  Eball.cooldown = true;
}

void Merlin::Sp_Eball(const Dragon &player)
{
  UNREFERENCED_PARAMETER(player);
  std::cout << "Spread Shot Energy Ball\n";
  AEVec2 Displacement0{ (player.PosX - PosX),
                       (player.PosY - PosY) };
  AEVec2 Displacement1{ (player.PosX - PosX),
                        (player.PosY - PosY) + 100.0f};
  AEVec2 Displacement2{ (player.PosX - PosX),
                        (player.PosY - PosY)  - 100.0f};
  AEVec2Normalize(&Displacement0, &Displacement0);
  AEVec2Scale(&Displacement0, &Displacement0, 6.0f);
  AEVec2Normalize(&Displacement1, &Displacement1);
  AEVec2Scale(&Displacement1, &Displacement1, 6.0f);
  AEVec2Normalize(&Displacement2, &Displacement2);
  AEVec2Scale(&Displacement2, &Displacement2, 6.0f);
  Spread_Eball[0].SetVelocity(Displacement0);
  Spread_Eball[1].SetVelocity(Displacement1);
  Spread_Eball[2].SetVelocity(Displacement2);
  for (int i = 0; i < 3; ++i)
  {
    Spread_Eball[i].SetActive(true);
    Spread_Eball[i].cooldown = true;
  }
}

void Merlin::A_Rain(const Dragon &player)
{
  UNREFERENCED_PARAMETER(player);
  srand((unsigned int)time(nullptr));
  std::cout << "Arrow Rain\n";
  if(castime == 0)
  for (int i = 0; i < A_Rain_Buffer; ++i)
  {
    if(i % 2)
      Arrow[i].PosX += (float)(rand() % 50);
    else
      Arrow[i].PosX -= (float)(rand() % 50);
    Arrow[i].SetActive(true);
    Arrow[i].cooldown = true;
  }
}

void Merlin::Attack(const Dragon &player)
{
  /*This will execute the attacks*/
  (this->*Merlin_Attack)(player);
  Attack_Interval = Merlin_Att_Inter;
}

void Merlin::CheckState(const Dragon &player)
{
  M_Curr = M_Next;
  //Assign the respective functions to the pointer
  switch (M_Curr)
  {
    case IDLE:
      Merlin_State = &Merlin::Idle;
      break;
    case MOVING: 
      Merlin_State = &Merlin::Move;
      break;
    case ATTACK:
      Merlin_State = &Merlin::Attack;
      break;
    default:
      Merlin_State = &Merlin::Idle;
      break;
  }

  if (M_Curr == MOVING)
  {
    //Check if can attack, else idle
    if (CheckAttack(player))
      M_Next = ATTACK;
    else
      M_Next = IDLE;
  }
  else if (M_Curr == ATTACK || M_Curr == IDLE)
  {
    //Check if all are on cooldown
    if (CheckAttack(player))
      M_Next = ATTACK;
    //If none, check if can blink
    else if (Blink_.Cooldown)
      M_Next = IDLE;
    else
      M_Next = MOVING;
  }
}

bool Merlin::CheckAttack(const Dragon &player)
{
  bool CanAttack{ false };

  if (Attack_Interval > 0)
    return CanAttack;

  if (player.PosY >= PosY - 50.0f && player.PosY <= PosY + 50.0f
    && player.PosX <= PosX + 50.0f && player.PosX >= PosX - 50.0f
    && !M_Melee.cooldown)
  {
    CanAttack = true;
    M_Att_Curr = MELEE;
  }
  else if (!Arrow[A_Rain_Buffer - 1].cooldown && Get_HP() <= M_Phase2_HP)
  {
    CanAttack = true;
    M_Att_Curr = ARROW_RAIN;
  }
  else if (!Spread_Eball[0].cooldown)
  {
    CanAttack = true;
    M_Att_Curr = SPREAD_EBALL;
  }
  else if (!Eball.cooldown)
  {
    CanAttack = true;
    M_Att_Curr = SINGLE_EBALL;
  }
  else
    return CanAttack;

  switch (M_Att_Curr)
  {
    case ARROW_RAIN:
      Merlin_Attack = &Merlin::A_Rain;
      break;
    case SPREAD_EBALL:
      Merlin_Attack = &Merlin::Sp_Eball;
      break;
    case SINGLE_EBALL:
      Merlin_Attack = &Merlin::S_Eball;
      break;
    case MELEE:
      Merlin_Attack = &Merlin::Melee;
      break;
    default:
      Merlin_Attack = nullptr;
      break;
  }

  return CanAttack;
}

void Merlin::Update(const Dragon &player)
{
  //Check if I am hit here
  //Update all on going attacks first, aka the currently active ones
  CheckState(player);
  (this->*Merlin_State)(player);
  --Attack_Interval;
  if (Attack_Interval <= 0)
    Attack_Interval = 0;
  if (Blink_.Cooldown)
  {
    --Blink_.CD_Time;
    if (Blink_.CD_Time <= 0)
    {
      Blink_.Cooldown = false;
      Blink_.CD_Time = Blink_CD_Time;
    }
  }
  //Attack cooldown updates here
  //Single Shot
  if (Eball.cooldown)
  {
    if (Eball.IsActive())
    {
      if (Eball.GetDist() >= Eball_Death)
      {
        Eball.ResetDist();
        Eball.SetActive(false);
      }
    }
    else
    {
      --Eball.cooldown_timer;
      if (Eball.cooldown_timer <= 0.0f)
      {
        Eball.cooldown = false;
        Eball.cooldown_timer = Eball_CD_Time;
      }
    }
  }
  Eball.Projectile::Pos();
  Eball.Projectile::Pos(PosX, PosY);
  Eball.Projectile::Update();
  //Spread Shot
  if (Spread_Eball[0].cooldown)
  {
    if (Spread_Eball[0].IsActive())
    {
      if (Spread_Eball[0].GetDist() >= Spread_Death)
      {
        for (int i = 0; i < 3; ++i)
        {
          Spread_Eball[i].ResetDist();
          Spread_Eball[i].SetActive(false);
        }
      }
    }
    else for (int i = 0; i < 3; ++i)
    {
      --Spread_Eball[i].cooldown_timer;
      if (Spread_Eball[i].cooldown_timer <= 0.0f)
      {
        Spread_Eball[i].cooldown = false;
        Spread_Eball[i].cooldown_timer = Spread_CD_Time;
      }
    }
  }
  for (int i = 0; i < 3; ++i)
  {
    Spread_Eball[i].Projectile::Pos();
    Spread_Eball[i].Projectile::Pos(PosX, PosY);
    Spread_Eball[i].Projectile::Update();
  }
  //Arrow Rain
  if (Arrow[A_Rain_Buffer - 1].cooldown)
  {
    if (Arrow[A_Rain_Buffer - 1].IsActive())
    {
      if (Arrow[A_Rain_Buffer - 1].GetDist() >= A_Rain_Death)
      {
        for (int i = 0; i < A_Rain_Buffer; ++i)
        {
          Arrow[i].ResetDist();
          Arrow[i].SetActive(false);
          castime = 100;
        }
      }
    }
    else for (int i = 0; i < A_Rain_Buffer; ++i)
    {
      --Arrow[i].cooldown_timer;
      if (Arrow[i].cooldown_timer <= 0.0f)
      {
        Arrow[i].cooldown = false;
        Arrow[i].cooldown_timer = A_Rain_CD_Time;
      }
    }
  }
  if (!Arrow[A_Rain_Buffer - 1].IsActive())
  {
    MC_Pos.SetTranslate(player.PosX, 260.0f);
    MC_Pos.Concat();
  }
  if (M_Att_Curr == ARROW_RAIN)
    --castime;
  if (castime <= 0)
    castime = 0;
  for (int i = 0; i < A_Rain_Buffer; ++i)
  {
    if (i == 0)
    {
      Arrow[i].Projectile::Pos();
      Arrow[i].Projectile::Pos(player.PosX, 360.0f);
      Arrow[i].Projectile::Update();
      continue;
    }
    if (Arrow[i - 1].GetDist() < 100.0f)
    {
      Arrow[i].Projectile::Pos(player.PosX, 360.0f);
    }
    else
    {
      Arrow[i].Projectile::Pos();
      Arrow[i].Projectile::Pos(player.PosX, 260.0f);
      Arrow[i].Projectile::Update();
    }
  }
  //Melee
  if (M_Melee.cooldown)
  {
    --M_Melee.cooldown_timer;
    if (M_Melee.cooldown_timer <= 0.0f)
    {
      M_Melee.cooldown = false;
      M_Melee.cooldown_timer = Melee_CD_Time;
    }
  }
}

void Merlin::Render()
{
  //Renders respective attack, the function checks if the attack should be rendered
  Eball.Render();
  for (int i = 0; i < 3; ++i)
  {
    Spread_Eball[i].Render();
  }
  for (int i = 0; i < A_Rain_Buffer; ++i)
  {
    if(M_Att_Curr == ARROW_RAIN)
      MagicCircle.Render_Object(MC_Pos);
    Arrow[i].Render();
  }

  //Renders Merlin
  GameObject::Render();
}
