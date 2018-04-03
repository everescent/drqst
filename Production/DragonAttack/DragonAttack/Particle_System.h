/*****************************************************************************/
/*!
\file    Particle_System.h
\author  Jacob Lim, jacob.lim, 440002417
\par     email: jaocb.lim\@digipen.edu
\date    March 08, 2018
\brief
  This file contains the Emitter struct and the Particle System struct. 
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/*****************************************************************************/

#pragma once
#include "Particle.h" //Particle, Color
#include <vector>     //vector<>
#include <cmath>      //sinf, cosf
#include <ctime>      //time
#include <functional> //function<>

using std::vector  ;
using std::function;

//Stores the values to randomize the particle variables
//Set to 0 if it is not meant to be used
struct Particle_Range
{
  int Spread_   ; //Directional spread; Set to 0 for no spread
  int Size_Rand_; //Size variation of each particle
  int Sp_Rand_  ; //Range for the random generation of speed
  int Life_Rand_; //Range for the random generation of lifetime

  //Default constructor to zero initialize the variables
  Particle_Range()
  :Spread_{}, Size_Rand_{}, Sp_Rand_{}, Life_Rand_{}
  {}
};

enum Emitter_Type
{
  CENTER, //Point emission
  BOX     //Surface emission
};

//Emitter handles the processing of the particle attributes and 
//also emission properties.
struct Emitter {
//Construct emitter with a mesh for particles and its position
Emitter(AEGfxVertexList* pMesh, AEVec2 Pos, Emitter_Type type);
//Clears the vector of particles
~Emitter();

union Particle_Pos
{
  struct Box{
      AEVec2 Point_Min; //Minimum point
      AEVec2 Point_Max; //Maximum point
      float  Angle_   ; //Angle of orientation
  };
  Box    Min_Max         ; //For surface emission (box)
  AEVec2 Point           ; //For point emitter
  AEVec2 Point_Min_Max[2]; //For array access
};

AEGfxVertexList* pMesh_        ; //Particle mesh
Particle_Pos     Pos_          ; //Emitter position
int              Vol_Max       ; //Maximum number of particles that can be emitted
float            Dist_Min_     ; //Minimum emission distance

int              PPS_          ; //Particle Per Second
float            Conserve_     ; //Law of Conservation of Energy

float            Direction_    ; //Particle direction
float            Size_         ; //Size of each particle
float            Speed_        ; //Scales the speed of all particles by this value
float            Lifetime_     ; //Lifetime of all particles

Particle_Range   Particle_Rand_; //Sets the limit of variable if randomizing them
Color            Color_        ; //Color of particles
float            Transparency_ ; //Transparency of particles
float            Exposure_     ; //Exposure of particles
vector<Particle> Particles_    ; //Dynamic array of particles

Emitter_Type     Type_         ; //Type of emitter
};

//Particle system takes care of the environment and
//keeping the whole system up to date. 
struct Particle_System {
//Initialize emitter
Particle_System(AEGfxVertexList* pMesh, AEVec2 Pos, Emitter_Type type)
:Emitter_{ pMesh, Pos, type }, p_count{ 0 }
{}
//Call emitter destructor to clear vector
~Particle_System()
{
  //Just to be explicit
  Emitter_.~Emitter();
}
//Simulates Brownian Motion
//User can specify the equation for phase x and phase y
void Turbulence(const float Strength, 
                const function<float()>& PhaseX = 
                []() -> float 
                {  
                  float phaseRot = (float)(rand() % 360);
                  phaseRot       = AEDegToRad(phaseRot) ;
                  return cosf(phaseRot)                 ;
                },
                const function<float()>& PhaseY = 
                []() -> float 
                {
                  float phaseRot = (float)(rand() % 360);
                  phaseRot       = AEDegToRad(phaseRot) ;
                  return sinf(phaseRot)                 ;
                });
//Simulates speed dampening
void     Drag          (const float Strength                            );
//Enacts a force in the specified direction, X or Y                     
void     Force         (const float Strength, const bool X, const bool Y);
//Gravity                                                               
void     Gravity       (const float Strength                            );

//Attract the particles to a certain point
//Modelled after a Schwarzchild black hole
//Set Attentuation to 0 for infinite range
void     Newton        (const AEVec2 Point, const float strength, 
                        const float  Atten = 0.0f                       );
//Enables exposure to be a factor of each particle's lifetime
void     ColorRamp_Life(                                                );
//Enables transparency to be a factor of each particle's exposure       
void     TransRamp_Exp (                                                );
//Creates each particle based on the emitter's attributes               
Particle Create        (                                                );
//Emits new particles                                                   
void     UpdateEmission(                                                );
//Updates the whole system                                              
void     Update        (const float dt                                  );
//Warm up time                                                          
void     WarmUp        (const float dt, const float time, 
                        const function<void()>     &Fields              );
//Scale the particle as time passes                                     
void     ScaleRamp     (const float Strength                            );




void Off_Emitter(void);


//Get the number of particles in the volume
const int GetParticleCount() const { return p_count; }
//Renders the particles
void Render();
//Emitter
Emitter Emitter_;

private:
//Counter for the number of particles birthed so far
int p_count;
};
