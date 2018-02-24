/* Start Header ************************************************************************/
/*!
\file       Merlin.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Merlin class member functions defined here.

Copyright (C) 2018 DigiPen Institute of Technology.
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
  :Characters{ S_CreateSquare(100.0f, "merlin.png"), Merlin_HP,
  Col_Comp{ Merlin_Start_X - 100.0f, Merlin_Start_Y - 100.0f,
  Merlin_Start_X + 100.0f, Merlin_Start_Y + 100.0f, Rect } },
  //Initialize Merlin State Machine
  Merlin_Attack{ &Merlin::Melee }, Merlin_State{ &Merlin::Idle },
  M_Curr{ IDLE }, M_Next{ IDLE }, M_Att_Curr{ MELEE },
  //Initialize Boss Attacks
  M_Melee{ Sprite{}, Col_Comp{} },
  Eball{ Sprite{ S_CreateSquare(50.0f, "energyball.png") },
         Col_Comp{ Merlin_Start_X - 50.0f, Merlin_Start_Y - 50.0f,
                   Merlin_Start_X + 50.0f, Merlin_Start_Y + 50.0f, Rect } },
  Spread_Eball{}, Arrow{}, Blink_{}, Attack_Interval{ 0 },
  MagicCircle{ S_CreateSquare(150.0f, "magic_circle.png") }
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
    Spread_Eball.push_back(Boss_Attack{ S_CreateSquare(30.0f, "energyball.png"), 
                           Col_Comp{ Merlin_Start_X - 30.0f, Merlin_Start_Y - 30.0f,
                           Merlin_Start_X + 30.0f, Merlin_Start_Y + 30.0f, Rect } });
  for (Boss_Attack& SS : Spread_Eball)
  {
    SS.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    SS.cooldown_timer = Spread_CD_Time;
  }
  //Initialize Arrow Rain
  Arrow.reserve(A_Rain_Buffer);
  for (int i = 0; i < A_Rain_Buffer; ++i)
    Arrow.push_back(Boss_Attack{ S_CreateSquare(30.0f, "arrow.png"),
                    Col_Comp{ Merlin_Start_X - 2.0f, Merlin_Start_Y - 30.0f,
                    Merlin_Start_X + 2.0f, Merlin_Start_Y + 30.0f, Rect } });
  for (int i = 0; i < A_Rain_Buffer; ++i)
  {
    Arrow[i].Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    Arrow[i].SetVelocity(AEVec2{ 0.0f, -520.0f });
    Arrow[i].cooldown_timer = A_Rain_CD_Time;
  }
}
//Clears the vector memory
Merlin::~Merlin()
{
  Arrow.clear();
  Spread_Eball.clear();
}
//STATE FUNCTIONS START//////////////////////////////////////////////////////////////////
void Merlin::Idle(Dragon &player)
{
  UNREFERENCED_PARAMETER(player);
  /*Do some idle animation or something idk yet*/
}

void Merlin::Move(Dragon &player)
{
  UNREFERENCED_PARAMETER(player);
  //This will move merlin to the 3 different spots
  srand((unsigned int)time(nullptr));
  int RNG = rand() % 100;
  //Each position has a 1/3 chance to blink to
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

void Merlin::Attack(Dragon &player)
{
  //This will execute the attacks
  (this->*Merlin_Attack)(player);
  Attack_Interval = Merlin_Att_Inter;
}
//STATE FUNCTIONS END////////////////////////////////////////////////////////////////////

//ATTACK FUNCTIONS START/////////////////////////////////////////////////////////////////
void Merlin::Melee(Dragon &player)
{
  UNREFERENCED_PARAMETER(player);
  std::cout << "Melee\n";
  M_Melee.cooldown = true;
  M_Melee.cooldown_timer = Melee_CD_Time;
}

void Merlin::S_Eball(Dragon &player)
{
  std::cout << "Single Shot Energy Ball\n";
  //Get the displacement betwwen player and Merlin
  AEVec2 Displacement{ (player.PosX - PosX ),
                       (player.PosY - PosY) };
  //Normalize and scale displacement
  AEVec2Normalize(&Displacement, &Displacement);
  AEVec2Scale(&Displacement, &Displacement, 360.0f);
  Eball.SetActive(true);
  //Go towards player
  Eball.SetVelocity(Displacement);
  Eball.cooldown = true;
  Eball.cooldown_timer = Eball_CD_Time;
}

void Merlin::Sp_Eball(Dragon &player)
{
  UNREFERENCED_PARAMETER(player);
  std::cout << "Spread Shot Energy Ball\n";
  //First vector points towards player
  AEVec2 Displacement0{ (player.PosX - PosX),
                       (player.PosY - PosY) };
  //Second vector points above player
  AEVec2 Displacement1{ (player.PosX - PosX),
                        (player.PosY - PosY) + 100.0f};
  //Third vector points below player
  AEVec2 Displacement2{ (player.PosX - PosX),
                        (player.PosY - PosY) - 100.0f};
  //Normalize and scale all vectors
  AEVec2Normalize(&Displacement0, &Displacement0);
  AEVec2Scale(&Displacement0, &Displacement0, 360.0f);
  AEVec2Normalize(&Displacement1, &Displacement1);
  AEVec2Scale(&Displacement1, &Displacement1, 360.0f);
  AEVec2Normalize(&Displacement2, &Displacement2);
  AEVec2Scale(&Displacement2, &Displacement2, 360.0f);
  //Set all displacement vectors to be the velocity
  Spread_Eball[0].SetVelocity(Displacement0);
  Spread_Eball[1].SetVelocity(Displacement1);
  Spread_Eball[2].SetVelocity(Displacement2);
  for (Boss_Attack& SS : Spread_Eball)
  {
    SS.SetActive(true);
    SS.cooldown = true;
    SS.cooldown_timer = Spread_CD_Time;
  }
}

void Merlin::A_Rain(Dragon &player)
{
  UNREFERENCED_PARAMETER(player);
  //Seed rand based on time
  srand((unsigned int)time(nullptr));
  std::cout << "Arrow Rain\n";
  //Only shoot when cast time completed
  if (castime == 0)
  {
    for (int i = 0; i < A_Rain_Buffer; ++i)
    {
      //Shoots arrows within +- 50 of the player's last known position
      if (i % 2)
        Arrow[i].PosX += (float)(rand() % 50);
      else
        Arrow[i].PosX -= (float)(rand() % 50);
      Arrow[i].SetActive(true);
      Arrow[i].cooldown = true;
      Arrow[i].cooldown_timer = A_Rain_CD_Time;
      Arrow[i].Projectile::Update();
    }
  }
}
void Merlin::Melee_Update()
{
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
void Merlin::S_Eball_Update()
{
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
      Eball.Update(0.016f);
    }
  }
  Eball.Projectile::Pos();
  Eball.Projectile::Pos(PosX, PosY);
  Eball.Projectile::Update();
  Eball.Collision_.Update_Col_Pos(Eball.PosX - 50.0f, Eball.PosY - 50.0f, 
                                  Eball.PosX + 50.0f, Eball.PosY + 50.0f);
}
void Merlin::Sp_Eball_Update()
{
  //Check if any bullets for spread shot are on cooldown
  bool SS_cd    { false };
  //Check if any bullets for spread shot are active
  bool SS_active{ false };
  for (Boss_Attack& SS : Spread_Eball)
  {
    SS_cd = SS_cd || SS.cooldown;
  }
  for (Boss_Attack& SS : Spread_Eball)
  {
    SS_active = SS_active || SS.IsActive();
  }
  if (SS_cd)
  {
    if (SS_active)
    {
      for(Boss_Attack& SS : Spread_Eball)
      {
        if (SS.GetDist() >= Spread_Death)
        {
          SS.ResetDist();
          SS.SetActive(false);
        }
      }
    }
    else for (Boss_Attack& SS : Spread_Eball)
    {
      SS.Update(0.016f);
    }
  }
  for (Boss_Attack& SS : Spread_Eball)
  {
    SS.Projectile::Pos();
    SS.Projectile::Pos(PosX, PosY);
    SS.Projectile::Update();
    SS.Collision_.Update_Col_Pos(SS.PosX - 30.0f, SS.PosY - 30.0f, 
                                 SS.PosX + 30.0f, SS.PosY + 30.0f);
  }
}

void Merlin::A_Rain_Update(Dragon &player)
{
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
      Arrow[i].Update(0.016f);
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
      Arrow[i].Projectile::Pos(player.PosX, 460.0f);
      Arrow[i].Projectile::Update();
      Arrow[i].Collision_.Update_Col_Pos
      (Arrow[i].PosX - 2.0f, Arrow[i].PosY - 30.0f,
       Arrow[i].PosX + 2.0f, Arrow[i].PosY + 30.0f);
    }
    else if (Arrow[i - 1].GetDist() < 100.0f)
    {
      Arrow[i].Projectile::Pos(player.PosX, 460.0f);
      Arrow[i].Collision_.Update_Col_Pos
      (Arrow[i].PosX - 2.0f, Arrow[i].PosY - 30.0f,
       Arrow[i].PosX + 2.0f, Arrow[i].PosY + 30.0f);
    }
    else
    {
      Arrow[i].Projectile::Pos();
      Arrow[i].Projectile::Pos(player.PosX, 460.0f);
      Arrow[i].Projectile::Update();
      Arrow[i].Collision_.Update_Col_Pos
      (Arrow[i].PosX - 2.0f, Arrow[i].PosY - 30.0f,
       Arrow[i].PosX + 2.0f, Arrow[i].PosY + 30.0f);
    }
  }
}

void Merlin::Colision_Check(Dragon &player)
{
  //Arrow rain collision check
  if (Arrow[A_Rain_Buffer - 1].cooldown)
  {
    if (Arrow[A_Rain_Buffer - 1].IsActive())
    {
      for (int i = 0; i < A_Rain_Buffer - 1; ++i)
        if (Arrow[i].IsActive())
        {
          if (player.Collision_.Dy_Rect_Rect(Arrow[i].Collision_, 
              Arrow[i].GetVelocity(), player.GetVelocity(), 0.016f))
          {
            player.Decrease_HP();
            Arrow[i].SetActive(false);
          }
        }
    }
  }
  //Spread shot collision check
  if (Spread_Eball[0].cooldown)
  {
    if (Spread_Eball[0].IsActive())
    {
      for (int i = 0; i < 3; ++i)
        if (Spread_Eball[i].IsActive())
        {
          if (player.Collision_.Dy_Rect_Rect(Spread_Eball[i].Collision_,
                                             Spread_Eball[i].GetVelocity(),
                                             player.GetVelocity(), 0.016f))
          {
            player.Decrease_HP();
            Spread_Eball[i].ResetDist();
            Spread_Eball[i].SetActive(false);
          }
        }
    }
  }
  //Energy ball collision check
  if (Eball.cooldown)
  {
    if (Eball.IsActive())
    {
      if (player.Collision_.Dy_Rect_Rect(Eball.Collision_, Eball.GetVelocity(),
                                         player.GetVelocity(), 0.016f))
      {
        player.Decrease_HP();
        Eball.ResetDist();
        Eball.SetActive(false);
      }
    }
  }
}
//ATTACK FUNCTIONS END///////////////////////////////////////////////////////////////////

void Merlin::CheckState(Dragon &player)
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
  else if (M_Curr == IDLE)
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
  else if (M_Curr == ATTACK)
  {
    if (Blink_.Cooldown)
      M_Next = IDLE;
    else
      M_Next = MOVING;
  }
}

bool Merlin::CheckAttack(Dragon &player)
{
  bool CanAttack{ false };
  //Check if attack interval is over
  if (Attack_Interval > 0)
    return CanAttack;
  //Check if payer within melee range
  if (player.PosY >= PosY - 50.0f && player.PosY <= PosY + 50.0f
    && player.PosX <= PosX + 50.0f && player.PosX >= PosX - 50.0f
    && !M_Melee.cooldown)
  {
    CanAttack = true;
    M_Att_Curr = MELEE;
  }
  //Check if arrow rain is available for cast
  else if (!Arrow[0].cooldown && Get_HP() <= M_Phase2_HP)
  {
    CanAttack = true;
    M_Att_Curr = ARROW_RAIN;
  }
  //Check if Spread Shot Energy Ball is available for cast
  else if (!Spread_Eball[0].cooldown)
  {
    CanAttack = true;
    M_Att_Curr = SPREAD_EBALL;
  }
  //Check if Energy Ball is available for cast
  else if (!Eball.cooldown)
  {
    CanAttack = true;
    M_Att_Curr = SINGLE_EBALL;
  }
  else
    return CanAttack;
  //Set attack function pointer to respective attack function
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

void Merlin::Update(Dragon &player)
{
  //Determine Merlin's state
  CheckState(player);  
  //Execute state
  (this->*Merlin_State)(player);
  //Update attack interval
  Attack_Interval -= 0.016f;
  if (Attack_Interval <= 0)
    Attack_Interval = 0;
  //Update Blink
  Blink_.Update();
  //Attack updates here
  A_Rain_Update(player);
  Sp_Eball_Update();
  S_Eball_Update();
  Melee_Update();
  //Merlin collision update
  GameObject::Collision_.Update_Col_Pos(PosX - 100.0f * 0.6f, PosY - 100.0f * 0.6f, 
                                        PosX + 100.0f * 0.6f, PosY + 100.0f * 0.6f);
  //If Merlin gets hit, decrease HP
  for(char i = 0; i < Bullet_Buffer; ++i)
    if(player.GetFireball()[i].IsActive())
      if (Collision_.Dy_Rect_Rect(player.GetFireball()[i].Collision_,
                                  GetVelocity(),
                                  player.GetFireball()[i].GetVelocity(), 
                                  0.016f))//Remember to replace with dt
      {
        Decrease_HP(player.GetDamage());
        player.GetFireball()[i].SetActive(false);
      }
  //Check for attack colision with player
  Colision_Check(player);
}

void Merlin::Render()
{
  //Renders respective attack, the function checks if the attack should be rendered
  Eball.Render();
  for (Boss_Attack& SS : Spread_Eball)
  {
    SS.Render();
  }
  for (int i = 0; i < A_Rain_Buffer - 1; ++i)
  {
    if(M_Att_Curr == ARROW_RAIN)
      MagicCircle.Render_Object(MC_Pos);
    Arrow[i].Render();
  }
  //Renders Merlin
  GameObject::Render();
}
