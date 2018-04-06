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

Merlin::Merlin(Sprite *Merlin_Texture, Sprite *Eball_Sprite, Sprite *Arrow_Sprite)
  //Initialize characters class
  :Characters{ Merlin_Texture, Merlin_HP,
  Col_Comp{ Merlin_Start_X - Merlin_Scale, Merlin_Start_Y - Merlin_Scale,
  Merlin_Start_X + Merlin_Scale, Merlin_Start_Y + Merlin_Scale, Rect } },
  //Initialize Merlin State Machine
  Merlin_Attack{ &Merlin::Melee }, Merlin_State{ &Merlin::Idle },
  M_Curr{ IDLE }, M_Next{ IDLE }, M_Att_Curr{ MELEE },
  //Initialize Boss Attacks
  M_Melee{ nullptr, Col_Comp{} },
  Eball{ Eball_Sprite,
         Col_Comp{ Merlin_Start_X - 50.0f, Merlin_Start_Y - 50.0f,
                   Merlin_Start_X + 50.0f, Merlin_Start_Y + 50.0f, Rect } },
  Spread_Eball{}, Arrow{}, Blink_{}, Attack_Interval{ 0 },
  MagicCircle{ S_CreateSquare(150.0f, ".//Textures/Magic_Circle.png") },
  Anim_{ ANIM_TELE_IN + 1, 4.0f, 5.0f, 
  [](std::vector <Range>& Init) ->void {
    Init.push_back(Range{ 0.0f, 1.0f, 0.00f, 0.00f }); //Hit
    Init.push_back(Range{ 0.0f, 1.0f, 0.25f, 0.25f }); //Idle
    Init.push_back(Range{ 0.0f, 1.0f, 0.50f, 0.50f }); //Tele Out
    Init.push_back(Range{ 0.0f, 1.0f, 0.75f, 0.75f }); //Tele In
  }
}
{
  //Set spawn position
  PosX = Merlin_Start_X;
  PosY = Merlin_Start_Y;
  Transform_.SetTranslate(PosX, PosY);
  Transform_.SetScale(Merlin_Scale, Merlin_Scale);
  Transform_.Concat();
  Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  //Initialize Magic Circle
  MagicCircle.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  //Melee has no special render so always active
  M_Melee.SetActive(true);
  //Initialize Single Energy Ball
  Eball.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  Eball.cooldown_timer = Eball_CD_Time;
  //Initialize Spread Shot
  Spread_Eball.reserve(3);
  for (int i = 0; i < 3; ++i)
    Spread_Eball.push_back(Boss_Attack{ Eball_Sprite,
                           Col_Comp{ Merlin_Start_X - 30.0f, Merlin_Start_Y - 30.0f,
                           Merlin_Start_X + 30.0f, Merlin_Start_Y + 30.0f, Rect } });
  for (Boss_Attack& SS : Spread_Eball)
  {
    SS.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    SS.cooldown_timer = Spread_CD_Time;
  }
  //Initialize Arrow Rain
  Arrow.reserve(A_Rain_Buffer);
  for (int i = 0; i < A_Rain_Buffer; ++i)
    Arrow.push_back(Boss_Attack{ Arrow_Sprite,
                    Col_Comp{ Merlin_Start_X - 2.0f, Merlin_Start_Y - 30.0f,
                    Merlin_Start_X + 2.0f, Merlin_Start_Y + 30.0f, Rect } });
  for (int i = 0; i < A_Rain_Buffer; ++i)
  {
    Arrow[i].Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
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
void Merlin::Idle(Dragon &player, const float dt)
{
  UNREFERENCED_PARAMETER(dt);
  UNREFERENCED_PARAMETER(player);
  Anim_.SetState(ANIM_IDLE);
}

void Merlin::Move(Dragon &player, const float dt)
{
  UNREFERENCED_PARAMETER(dt);
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

void Merlin::Attack(Dragon &player, const float dt)
{
  //This will execute the attacks
  (this->*Merlin_Attack)(player, dt);
  Attack_Interval = Merlin_Att_Inter;
}
//STATE FUNCTIONS END////////////////////////////////////////////////////////////////////

//ATTACK FUNCTIONS START/////////////////////////////////////////////////////////////////
void Merlin::Melee(Dragon &player, const float dt)
{
  UNREFERENCED_PARAMETER(player);
  UNREFERENCED_PARAMETER(dt);
  M_Melee.cooldown = true;
  M_Melee.cooldown_timer = Melee_CD_Time;
}

void Merlin::S_Eball(Dragon &player, const float dt)
{
  UNREFERENCED_PARAMETER(dt);
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

void Merlin::Sp_Eball(Dragon &player, const float dt)
{
  UNREFERENCED_PARAMETER(dt);
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

void Merlin::A_Rain(Dragon &player, const float dt)
{
  UNREFERENCED_PARAMETER(player);
  //Seed rand based on time
  srand((unsigned int)time(nullptr));
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
      Arrow[i].Projectile::Update(dt);
    }
  }
}
void Merlin::Melee_Update(const float dt)
{
  //Check if melee is used
  if (M_Melee.cooldown)
  {
    //Update cooldown
    M_Melee.cooldown_timer -= dt;
    if (M_Melee.cooldown_timer <= 0.0f)
    {
      //Reset melee 
      M_Melee.cooldown = false;
      M_Melee.cooldown_timer = Melee_CD_Time;
    }
  }
}
void Merlin::S_Eball_Update(const float dt)
{
  //Check if single shot is dead
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
      //Update cooldown
      Eball.Update(dt);
    }
  }
  //Update position and collision
  Eball.Projectile::Pos(PosX, PosY);
  Eball.Projectile::Update(dt, Eball_Scale);
}
void Merlin::Sp_Eball_Update(const float dt)
{
  //Check if any bullets for spread shot are on cooldown
  bool SS_cd    { false };
  //Check if any bullets for spread shot are active
  bool SS_active{ false };
  //Check if Spread Shot died
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
        //Then kill it if dead
        if (SS.GetDist() >= Spread_Death)
        {
          SS.ResetDist();
          SS.SetActive(false);
        }
      }
    }
    //Update Cooldown
    else for (Boss_Attack& SS : Spread_Eball)
    {
      SS.Update(dt);
    }
  }
  //Update position and collision
  for (Boss_Attack& SS : Spread_Eball)
  {
    SS.Projectile::Pos(PosX, PosY);
    SS.Projectile::Update(dt, Spread_Scale);
  }
}

void Merlin::A_Rain_Update(Dragon &player, const float dt)
{
  //Check if last arrow died
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
    //Ifn o update cooldown
    else for (int i = 0; i < A_Rain_Buffer; ++i)
    {
      Arrow[i].Update(dt);
    }
  }
  //Activate magic circle
  if (!Arrow[A_Rain_Buffer - 1].IsActive())
  {
    MC_Pos.SetTranslate(player.PosX, 260.0f);
    MC_Pos.Concat();
  }
  if (M_Att_Curr == ARROW_RAIN)
    --castime;
  if (castime <= 0)
    castime = 0;
  //Arrow rain position and collision updates
  for (int i = 0; i < A_Rain_Buffer; ++i)
  {
    if (i == 0)
    {
      Arrow[i].Projectile::Pos(player.PosX, 460.0f);
      Arrow[i].Projectile::Update(dt, A_Rain_Scale);
    }
    else if (Arrow[i - 1].GetDist() < 100.0f)
    {
      Arrow[i].Projectile::Pos(player.PosX, 460.0f);
      Arrow[i].Collision_.Update_Col_Pos
      (Arrow[i].PosX - A_Rain_Scale, Arrow[i].PosY - A_Rain_Scale,
       Arrow[i].PosX + A_Rain_Scale, Arrow[i].PosY + A_Rain_Scale);
    }
    else
    {
      Arrow[i].Projectile::Pos(player.PosX, 460.0f);
      Arrow[i].Projectile::Update(dt, A_Rain_Scale);
    }
  }
}

void Merlin::Colision_Check(Dragon &player, const float dt)
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
              Arrow[i].GetVelocity(), player.GetVelocity(), dt))
          {
            player.SetInvul(true);
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
                                             player.GetVelocity(), dt))
          {
            player.SetInvul(true);
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
                                         player.GetVelocity(), dt))
      {
        player.SetInvul(true);
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
  if (player.PosY >= PosY - Merlin_Scale && player.PosY <= PosY + Merlin_Scale
    && player.PosX <= PosX + Merlin_Scale && player.PosX >= PosX - Merlin_Scale
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

void Merlin::Update(Dragon &player, const float dt)
{
  //Check if Merlin is dead
  if (Get_HP() > 0)
  {
    //Determine Merlin's state
    CheckState(player);
    //Execute state
    (this->*Merlin_State)(player, dt);
    //Update attack interval
    Attack_Interval -= dt;
    if (Attack_Interval <= 0)
      Attack_Interval = 0;
    //Update Blink
    Blink_.Update();
    //Attack updates here
    A_Rain_Update(player, dt);
    Sp_Eball_Update(dt);
    S_Eball_Update(dt);
    Melee_Update(dt);
    //Merlin collision update
    GameObject::Collision_.Update_Col_Pos(PosX - Merlin_Scale * 0.6f, PosY - Merlin_Scale * 0.6f,
      PosX + Merlin_Scale * 0.6f, PosY + Merlin_Scale * 0.6f);
    //If Merlin gets hit, decrease HP
    for (char i = 0; i < Bullet_Buffer; ++i)
      if (player.GetFireball()[i].IsActive())
        if (Collision_.Dy_Rect_Rect(player.GetFireball()[i].Collision_,
          GetVelocity(),
          player.GetFireball()[i].GetVelocity(), dt))
        {
          //Decrease HP by player's damage
          Decrease_HP(player.GetDamage());
          //Reset the distance of the fireball and set false
          player.GetFireball()[i].Projectile::ResetDist();
          player.GetFireball()[i].SetActive(false);
        }
    if (player.GetMfireball().IsActive())
      if (Collision_.Dy_Rect_Rect(player.GetMfireball().Collision_, GetVelocity(),
        player.GetMfireball().GetVelocity(), dt))
      {
        //Decrease HP by player's damage
        Decrease_HP(player.GetMDamage());
        player.PlayImpact();
        //Reset the distance of the fireball and set false
        player.GetMfireball().Projectile::ResetDist();
        player.GetMfireball().SetActive(false);
      }
    //Check for attack colision with player
    Colision_Check(player, dt);
  }
  Anim_.Update(*Sprite_);
}

void Merlin::Render()
{
  //Check if Merlin is dead
  if (Get_HP() > 0)
  {
    //Renders respective attack, the function checks if the attack should be rendered
    Eball.Render();
    for (Boss_Attack& SS : Spread_Eball)
    {
      SS.Render();
    }
    for (int i = 0; i < A_Rain_Buffer - 1; ++i)
    {
      if (M_Att_Curr == ARROW_RAIN)
        MagicCircle.Render_Object(MC_Pos);
      Arrow[i].Render();
    }
    //Renders Merlin
    GameObject::Render();
  }
}
