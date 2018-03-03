#include "Archer.h"
#include <iostream>

using namespace ArcherMacros;

Archer::Archer(const float posX, const float posY)
  //Initialize characters class
  :Characters{ S_CreateSquare(Archer_Scale, ".//Textures/grunt.png"), Archer_HP,
             Col_Comp{ posX - Archer_Scale, posY - Archer_Scale,
             posX + Archer_Scale, posY + Archer_Scale, Rect } },
  //Initialize Arrow
  Arrow{ S_CreateSquare(Arrow_Scale, "Arrow.png"),
          Col_Comp{ posX - Arrow_Scale, posY - Arrow_Scale,
          posX + Arrow_Scale, posY + Arrow_Scale, Rect } }, 
  //Initialize other members
  Archer_State{ &Archer::Idle }, A_Curr{ IDLE }, A_Next{ IDLE },
  Dragon_Seen{ false }, Attack_{ false }, Distance{ 0.0f }, Arrow_CD{ 0.0f }
{
  SetActive(true);
  GameObject::Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  Arrow.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  //Initialize position of Archer
  PosX = posX;
  PosY = posY;
}

void Archer::Attack_Update(Dragon &/*player*/, const float dt)
{
  if (Arrow.IsActive())
  {
    if (Arrow.GetDist() >= Arrow_Death)
    {
      Arrow.ResetDist();
      Arrow.SetActive(false);
    }
  }
  else
  {
    Arrow_CD -= dt;
    if (Arrow_CD <= 0.0f)
    {
      Arrow_CD = 0.0f;
      Attack_ = false;
    }
  }
  Arrow.Update(50.0f, false, Angle);
}

void Archer::Colision_Check(Dragon &/*player*/, const float /*dt*/)
{
  
}

void Archer::Idle(Dragon &/*player*/, const float /*dt*/)
{
  //Do some idle animation
}

void Archer::Move(Dragon &player, const float dt)
{
  //Move the Archer away from player if too near
  //Archer can only move for a max distance of 100m or something
  float Displacement = player.PosX - PosX;
  if (Displacement < 0.0f)
  {
    PosX += -Archer_Speed * dt;
    Distance += Archer_Speed * dt;
  }
  else
  {
    PosX += Archer_Speed * dt;
    Distance += Archer_Speed * dt;
  }
}

void Archer::Attack(Dragon &player, const float /*dt*/)
{
  //Shoots an arrow, based on player's displacement with archer
  //The displacement vector should be a parabola 
  float X{ PosX - player.PosX };
  float Y{ PosY - player.PosY };
  AEVec2 Disp{ -X, -Y };
  Arrow.SetActive(true);
  //Adjacent / Hypotenuse
  Angle = (X / (f_sqrt(AEVec2Length(&Disp))));
  //if (X < 0.0f)
  //  Angle = -Angle;
  AEVec2Normalize(&Disp, &Disp);
  AEVec2Scale(&Disp, &Disp, 260.0f);
  Arrow.SetVelocity(Disp);
  Arrow_CD = Archer_CD_Time;
  Attack_ = true;
  Arrow.PosX = PosX;
  Arrow.PosY = PosY;
  std::cout << "ATTACK";
}

void Archer::CheckState(Dragon &player, const float /*dt*/)
{
  //Check if dragon is seen
  //IF SO, check if within Line Of Sight
  //IF SO, attack, IF NOT move towards dragon and attack once in LOS
  A_Curr = A_Next;
  switch (A_Curr)
  {
  case IDLE:
    Archer_State = &Archer::Idle;
    break;
  case MOVING:
    Archer_State = &Archer::Move;
    break;
  case ATTACK:
    Archer_State = &Archer::Attack;
    break;
  }
  if(A_Curr == ATTACK)
    A_Next = IDLE;
  //Only move if dragon was seen and my distance has not reached max
  if (abs(player.PosX - PosX) < Archer_LOS && 
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
    A_Next = IDLE;
  }
}

void Archer::Update(Dragon& player, const float dt)
{
  //Check if Dragon within Line Of Sight
  //if (player.PosX - PosX < Archer_LOS)
  //{
  //  std::cout << "hi";
    Dragon_Seen = true;
  //}
  //Assign state
  CheckState(player, dt);
  //Execute state
  (this->*Archer_State)(player, dt);
  //Update attacks
  Attack_Update(player, dt);
  //Check for collision
  Colision_Check(player, dt);
  //Concatenate all matrices
  Transform_.SetTranslate(PosX, PosY);
  Transform_.Concat();
}

//Renders Archer and attacks
void Archer::Render()
{
  
  GameObject::Render();
  Arrow.Render();
}
