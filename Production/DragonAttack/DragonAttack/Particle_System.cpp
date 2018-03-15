/*****************************************************************************/
/*!
\file    Particle_System.cpp
\author  Jacob Lim, jacob.lim, 440002417
\par     email: jaocb.lim\@digipen.edu
\date    March 08, 2018
\brief
  This file contains the implementation of the Emitter and
  Particle System.
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/*****************************************************************************/

#include "Particle_System.h"

//Initialize mesh and position, everything else is 0
//User should be keying in desired values
//Doing this in a "function" style is too messy
Emitter::Emitter(AEGfxVertexList* pMesh, AEVec2 Pos)
:pMesh_{ pMesh }, Pos_{ Pos }, Vol_Max{ 0 },
Dist_Min_{ 0.0f }, PPS_{ 0 }, Direction_{ 0.0f },
Spread_{ 0 }, Conserve_{ 0.0f }, Size_{ 0.0f }, 
Size_Rand_{ 0 }, Speed_{ 0.0f }, Sp_Rand_{ 0 },
Lifetime_{ 0.0f }, Life_Rand_{ 0 },
Color_{}, Transparency_{ 1.0f }, Exposure_{ 1.0f },
Particles_{}
{
  //Randomize a seed for rand
  srand((unsigned int)time(nullptr));
}

//Clear the vector of particles
Emitter::~Emitter()
{
  Particles_.clear();
}

//Creates a particle based on emitter attributes
Particle Particle_System::Create()
{
  //Make a temporary particle
  Particle tmp_;
  float Dir_tmp  = Emitter_.Direction_; //Final Direction
  float Sz_tmp   = Emitter_.Size_;      //Final Size
  float Spd_tmp  = Emitter_.Speed_;     //Final Speed
  float Life_tmp = Emitter_.Lifetime_;  //Final Lifetime
  //If there is spread
  if (Emitter_.Spread_)
  {
    //50% chance of returning a positive or negative
    Dir_tmp += (rand() % 100) < 50 ? (rand() % Emitter_.Spread_) :
               -(rand() % Emitter_.Spread_);
  }
  //If there is random size
  if (Emitter_.Size_Rand_)
  {
    //50% chance of returning a positive or negative
    Sz_tmp += (rand() % 100) < 50 ? (rand() % Emitter_.Size_Rand_) :
              -(rand() % Emitter_.Size_Rand_);
  }
  //If there is speed randomness
  if (Emitter_.Sp_Rand_)
  {
    //50% chance of returning a positive or negative
    Spd_tmp += (rand() % 100) < 50 ? (rand() % Emitter_.Sp_Rand_) :
               -(rand() % Emitter_.Sp_Rand_);
  }
  //If there is life randomness
  if (Emitter_.Life_Rand_)
  {
    //50% chance of returning a positive or negative
    Life_tmp += (rand() % 100) < 50 ? (rand() % Emitter_.Life_Rand_) :
                -(rand() % Emitter_.Life_Rand_);
  }
  //Convert to radians
  Dir_tmp = AEDegToRad(Dir_tmp);
  //Give mesh
  tmp_.pMesh_ = Emitter_.pMesh_;
  //Initialize position of particle
  tmp_.Pos_.x = Emitter_.Pos_.x + Emitter_.Dist_Min_;
  tmp_.Pos_.y = Emitter_.Pos_.y + Emitter_.Dist_Min_;
  //Initialize velocity
  tmp_.Vel_ = { cosf(Dir_tmp), sinf(Dir_tmp) };
  //Scale it by the speed
  AEVec2Scale(&tmp_.Vel_, &tmp_.Vel_, Spd_tmp);
  //Initialize the size
  tmp_.Size_ = Sz_tmp;
  //Initialize the lifetime
  tmp_.Lifetime_ = Life_tmp;
  //Initialize conserve
  tmp_.Conserve_ = Emitter_.Conserve_;
  //Initialize transparency
  tmp_.Transparency_ = Emitter_.Transparency_;
  //Initialize exposure
  tmp_.Exposure_ = Emitter_.Exposure_;
  //Initialize color
  tmp_.Color_.R = Emitter_.Color_.R;
  tmp_.Color_.G = Emitter_.Color_.G;
  tmp_.Color_.B = Emitter_.Color_.B;
  //Activate particle
  tmp_.Active_ = true;
  return tmp_;
}

void Particle_System::UpdateEmission()
{
  //Do not emit if particles are capped
  if (p_count >= Emitter_.Vol_Max)
  {
    return;
  }
  //Emit PPS_ number of particles
  for (unsigned i = 0; i < (unsigned)Emitter_.PPS_; ++i)
  {
    Emitter_.Particles_.push_back(Create());
    ++p_count;
  }
}

void Particle_System::Update(const float dt)
{
  //Update all particles
  for (auto& elem : Emitter_.Particles_)
  {
    elem.Update(dt);
  }
  //"Kill" inactive particles
  for (unsigned i = 0; i < Emitter_.Particles_.size(); ++i)
  {
    if (!Emitter_.Particles_[i].Active_)
    {
      Emitter_.Particles_.erase(Emitter_.Particles_.begin() + i);
      --p_count;
    }
  }
}

//Warm up the system
void Particle_System::WarmUp(const float dt, const float time)
{
  float tmp = time;
  while (tmp > 0.0f)
  {
    UpdateEmission();
    Update(dt);
    tmp -= dt;
  }
}

//Renders all particles
void Particle_System::Render()
{
  for (auto& elem : Emitter_.Particles_)
  {
    elem.Render();
  }
}

//Simulate random motion
void Particle_System::Turbulence(const float Strength,
                                 const function<float()>& PhaseX,
                                 const function<float()>& PhaseY)
{
  for (auto& elem : Emitter_.Particles_)
  {
    //Apply user's equation
    elem.Vel_.x += Strength * PhaseX();
    elem.Vel_.y += Strength * PhaseY();
  }
}

//Simulate drag
void Particle_System::Drag(const float Strength)
{
  //Make sure scale is not zero
  float tmp = 1.0f - Strength ? 1.0f - Strength : 0.000001f;
  for (auto& elem : Emitter_.Particles_)
  {
    AEVec2Scale(&elem.Vel_, &elem.Vel_, tmp);
  }
}

//Adds a force to specified direction
void Particle_System::Force(const float Strength, const bool X, const bool Y)
{
  for (auto& elem : Emitter_.Particles_)
  {
    if (X)
    {
      elem.Vel_.x += Strength;
    }
    if(Y)
    {
      elem.Vel_.y += Strength;
    }
  }
}

//Apply gravity
void Particle_System::Gravity(const float Strength)
{
  for (auto& elem : Emitter_.Particles_)
  {
    elem.Vel_.y -= Strength;
  }
}

//Enables exposure to be a factor of each particle's lifetime
void Particle_System::ColorRamp_Life()
{
  for (auto& elem : Emitter_.Particles_)
  {
    elem.Exposure_ = elem.Lifetime_ / Emitter_.Lifetime_;
  }
}

//Enables transparency to be a factor of each particle's exposure
void Particle_System::TransRamp_Exp()
{
  for (auto& elem : Emitter_.Particles_)
  {
    elem.Transparency_ = elem.Exposure_ * 1.5f;
  }
}
