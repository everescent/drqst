/* Start Header ************************************************************************/
/*!
\file       Archer.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Archer class member functions defined here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Archer.h"

using namespace ArcherMacros;
Sprite *Archer::Arrow_Sprite{ nullptr };

Audio_Engine Archer::Audio_{ 1, [](std::vector<std::string> &playlist) ->void {
  playlist.push_back(".//Audio/Hit_01.mp3");
  //playlist.push_back(".//Audio/Arrow_Swoosh.mp3"); 
} };

Archer::Archer(Sprite *p_Sprite, const float posX, const float posY)
  //Initialize characters class
  :Characters{ p_Sprite, Archer_HP,
             Col_Comp{ posX - Archer_Scale, posY - Archer_Scale,
             posX + Archer_Scale, posY + Archer_Scale, Rect } },
  //Initialize Arrow
  Arrow{ Arrow_Sprite,
          Col_Comp{ posX - Arrow_Scale, posY - Arrow_Scale,
          posX + Arrow_Scale, posY + Arrow_Scale, Rect } }, 
  //Initialize other members
  Archer_State{ &Archer::Idle }, A_Curr{ IDLE }, A_Next{ IDLE },
  Dragon_Seen{ false }, Attack_{ false }, Distance{ 0.0f }, Arrow_CD{ 0.0f }
{
  SetActive(true);
  GameObject::Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  Transform_.SetScale(Archer_Scale, Archer_Scale);
  //Initialize position of Archer
  PosX = posX;
  PosY = posY;
  Transform_.SetTranslate(PosX, PosY);
  Transform_.Concat();
  //Only create new sprite if Arrow_Sprite is not already created
  if(Arrow_Sprite == nullptr)
   Arrow_Sprite = new Sprite{ S_CreateSquare(0.5f, ".//Textures/Arrow.png") };
  Arrow.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
  Arrow.Transform_.SetScale(Arrow_Scale, Arrow_Scale);
  Arrow.Transform_.Concat();
}

Archer::~Archer()
{
  //Only delete the sprite once
  if (Arrow_Sprite != nullptr)
  {
    delete Arrow_Sprite;
    Arrow_Sprite = nullptr;
  }
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
  Arrow.Pos(PosX, PosY);
  Arrow.Update(dt, Arrow_Scale, false, Angle);
}

void Archer::Colision_Check(Dragon &player, const float dt)
{
  GameObject::Collision_.Update_Col_Pos(PosX - Archer_Scale, PosY - Archer_Scale,
                                        PosX + Archer_Scale, PosY + Archer_Scale);
  //If Archer gets hit, decrease HP
  for (char i = 0; i < Bullet_Buffer; ++i)
    if (player.GetFireball()[i].IsActive())
      if (Collision_.Dy_Rect_Rect(player.GetFireball()[i].Collision_, GetVelocity(),
          player.GetFireball()[i].GetVelocity(), dt))
      {
        //Decrease HP by player's damage
        Decrease_HP(player.GetDamage());
        //Add mega fireball charge
        player.AddCharge();
        player.PlayImpact();
        Audio_.Play(HIT);
        //Reset the distance of the fireball and set false
        player.GetFireball()[i].Projectile::ResetDist();
        player.GetFireball()[i].SetActive(false);
      }
  if(player.GetMfireball().IsActive())
	  if (Collision_.Dy_Rect_Rect(player.GetMfireball().Collision_, GetVelocity(),
		  player.GetMfireball().GetVelocity(), dt))
	  {
		  //Decrease HP by player's damage
		  Decrease_HP(player.GetMDamage());
		  player.PlayImpact();
		  Audio_.Play(HIT);
		  //Reset the distance of the fireball and set false
		  player.GetMfireball().Projectile::ResetDist();
		  player.GetMfireball().SetActive(false);
	  }
  //Arrow collision check
  if (Attack_)
  {
    if (Arrow.IsActive())
    {
      if (player.Collision_.Dy_Rect_Rect(Arrow.Collision_, Arrow.GetVelocity(),
          player.GetVelocity(), dt))
      {
        player.Decrease_HP();
        player.PlayHit();
        Arrow.ResetDist();
        Arrow.SetActive(false);
      }
    }
  }
}

void Archer::Idle(Dragon &/*player*/, const float /*dt*/)
{
  //Do some idle animation
  Distance = 0.0f;
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
  //Opposite / Adjacent
  Angle = atan(Y / X);
  //Convert to degrees
  Angle *= (180.0f / PI);
  //Directional check
  if (X < 0.0f)
    Angle += 180.0f;
  //Normalize and scale
  AEVec2Normalize(&Disp, &Disp);
  AEVec2Scale(&Disp, &Disp, 260.0f);
  Arrow.SetVelocity(Disp);
  Arrow_CD = Archer_CD_Time;
  Attack_ = true;
  Distance = 0.0f;
}

void Archer::Dead()
{
  SetActive(false);
  Arrow.SetActive(false);
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
    A_Next = IDLE;
  }
}

void Archer::Update(Dragon& player, const float dt)
{
  if (Get_HP() <= 0)
  {
    Dead();
  }
  else
  {
    //Check if Dragon within Line Of Sight
    if (abs(player.PosX - PosX) < Archer_LOS)
    {
      Dragon_Seen = true;
    }
    if (player.PosX < PosX)
    {
      Transform_.SetScale(-Archer_Scale, Archer_Scale);
    }
    else
    {
      Transform_.SetScale(Archer_Scale, Archer_Scale);
    }
    PosY -= Gravity;
    //Assign state
    CheckState(player, dt);
    //Execute state
    (this->*Archer_State)(player, dt);
    //Update attacks
    Attack_Update(player, dt);
    //Check for collision
    Colision_Check(player, dt);
    Audio_.Update();
    //Concatenate all matrices
    Transform_.SetTranslate(PosX, PosY);
    Transform_.Concat();
  }
}

//Renders Archer and attacks
void Archer::Render()
{
  GameObject::Render();
  Arrow.Render();
}
