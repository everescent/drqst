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
Emitter::Emitter(AEGfxVertexList* pMesh, AEVec2 Pos, Emitter_Type type)
:pMesh_{ pMesh },       Pos_{ Pos.x, Pos.y }, Vol_Max{ 0 },
 Dist_Min_{ 0.0f },     PPS_{ 0 },            Direction_{ 0.0f },
 Conserve_{ 0.0f },     Size_{ 0.0f },        Speed_{ 0.0f }, 
 Lifetime_{ 0.0f },     Particle_Rand_{},     Color_{}, 
 Transparency_{ 1.0f }, Exposure_{ 1.0f },    Particles_{},
 Type_{type}
{
  //Randomize a seed for rand
  srand((unsigned int)time(nullptr));
  Pos_.Min_Max.Angle_ = PI / 2.0f;
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
  if (Emitter_.Particle_Rand_.Spread_)
  {
  float min = Emitter_.Direction_ - Emitter_.Particle_Rand_.Spread_;
  float max = Emitter_.Direction_ + Emitter_.Particle_Rand_.Spread_;

  //Randomize a direction between min and max values
  Dir_tmp = (min + rand() % (int)(max - min + 1));
  }
  //If there is random size
  if (Emitter_.Particle_Rand_.Size_Rand_)
  {
  float min = Emitter_.Size_ - Emitter_.Particle_Rand_.Size_Rand_;
  float max = Emitter_.Size_ + Emitter_.Particle_Rand_.Size_Rand_;

  //Randomize a size between min and max values
  Sz_tmp = (min + rand() % (int)(max - min + 1));
  }
  //If there is speed randomness
  if (Emitter_.Particle_Rand_.Sp_Rand_)
  {
  float min = Emitter_.Speed_ - Emitter_.Particle_Rand_.Sp_Rand_;
  float max = Emitter_.Speed_ + Emitter_.Particle_Rand_.Sp_Rand_;

  //Randomize the speed between min and max values
  Spd_tmp = (min + rand() % (int)(max - min + 1));
  }
  //If there is life randomness
  if (Emitter_.Particle_Rand_.Life_Rand_)
  {
  float min = Emitter_.Lifetime_ - Emitter_.Particle_Rand_.Life_Rand_;
  float max = Emitter_.Lifetime_ + Emitter_.Particle_Rand_.Life_Rand_;

  Life_tmp = (min + rand() % (int)(max - min + 1));
  }
  //Convert to radians
  Dir_tmp = AEDegToRad(Dir_tmp);
  //Give mesh
  tmp_.pMesh_ = Emitter_.pMesh_;

  //Initialize position of particle
  switch(Emitter_.Type_)
  {   
    case CENTER:
      tmp_.Pos_.x = Emitter_.Pos_.Point.x;
      tmp_.Pos_.y = Emitter_.Pos_.Point.y;
      break;
    case BOX:
      //Determines the maximum magnitude in the x direction
      float MagnitudeW = Emitter_.Pos_.Min_Max.Point_Max.x - 
                         Emitter_.Pos_.Min_Max.Point_Min.x;
      //Determines the maximum magnitude in the y direction
      float MagnitudeH = Emitter_.Pos_.Min_Max.Point_Max.y - 
                         Emitter_.Pos_.Min_Max.Point_Min.y;
      //Direction Up
      AEVec2 a1{ cosf(Emitter_.Pos_.Min_Max.Angle_), 
                 sinf(Emitter_.Pos_.Min_Max.Angle_) };
      //Direction Right
      AEVec2 a2{ cosf(Emitter_.Pos_.Min_Max.Angle_ - (PI / 2.0f)), 
                 sinf(Emitter_.Pos_.Min_Max.Angle_ - (PI / 2.0f)) };
      //Get random position anywhere on the x-axis
      int Mag_RandW = rand() % (int)MagnitudeW;
      //Get random position anywhere on the y-axis
      int Mag_RandH = rand() % (int)MagnitudeH;
      //Scale both axis by final magitude
      AEVec2Scale(&a1, &a1, (float)Mag_RandH);
      AEVec2Scale(&a2, &a2, (float)Mag_RandW);
      //Move in the y-axis then in the x-axis
      AEVec2Add(&tmp_.Pos_, &Emitter_.Pos_.Min_Max.Point_Min, &a1);
      AEVec2Add(&tmp_.Pos_, &tmp_.Pos_, &a2);
      break;
  }
  switch (rand() % 8)
  {
    case 0: 
      tmp_.Pos_.x += Emitter_.Dist_Min_;
      break;
    case 1: 
      tmp_.Pos_.y += Emitter_.Dist_Min_;
      break;
    case 2: 
      tmp_.Pos_.x -= Emitter_.Dist_Min_;
      break;
    case 3: 
      tmp_.Pos_.y -= Emitter_.Dist_Min_;
      break;
    case 4: 
      tmp_.Pos_.x += Emitter_.Dist_Min_;
      tmp_.Pos_.y += Emitter_.Dist_Min_;
      break;
    case 5: 
      tmp_.Pos_.x -= Emitter_.Dist_Min_;
      tmp_.Pos_.y -= Emitter_.Dist_Min_;
      break;
    case 6: 
      tmp_.Pos_.x += Emitter_.Dist_Min_;
      tmp_.Pos_.y -= Emitter_.Dist_Min_;
      break;
    case 7: 
      tmp_.Pos_.x -= Emitter_.Dist_Min_;
      tmp_.Pos_.y += Emitter_.Dist_Min_;
      break;
  }
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
void Particle_System::WarmUp(const float dt, const float time, const function<void()> &Fields)
{
  float tmp = time;
  while (tmp > 0.0f)
  {
    UpdateEmission();
    Fields();
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

//Scales particles as time passes
void Particle_System::ScaleRamp(const float strength)
{
	for (auto& elem : Emitter_.Particles_)
	{
		elem.Size_ *= strength;
	}
}

//Attract the particles to a certain point
void Particle_System::Newton(const AEVec2 Point, const float strength, const float Attenuation)
{
  //Displacement between Newton point and particles
  AEVec2 final_displacement{}; 
  for (auto& elem : Emitter_.Particles_)
  {
    //Calculate displacement
    final_displacement.x = Point.x - elem.Pos_.x;
    final_displacement.y = Point.y - elem.Pos_.y;
    if (Attenuation)
    {
      if (AEVec2SquareLength(&final_displacement) > Attenuation * Attenuation)
        continue;
    }
    //Normalize and scale according to strength
    AEVec2Normalize(&final_displacement, &final_displacement);
    AEVec2Scale(&final_displacement, &final_displacement, strength);

    //Check if vectors are not parallel
    if (AEVec2CrossProductMag(&final_displacement, &elem.Vel_))
    {
      //Apply the "Attraction"
      elem.Vel_.x += final_displacement.x;
      elem.Vel_.y += final_displacement.y;
    }
    
  }
}
