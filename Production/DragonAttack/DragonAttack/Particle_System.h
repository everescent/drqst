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

using std::vector;
using std::function;

//Emitter handles the processing of the particle attributes and 
//also emission properties.
struct Emitter {
//Construct emitter with a mesh for particles and its position
Emitter(AEGfxVertexList* pMesh, AEVec2 Pos);
//Clears the vector of particles
~Emitter();

AEGfxVertexList* pMesh_;        //Particle mesh
AEVec2           Pos_;          //Emitter position
int              Vol_Max;       //Maximum number of particles that can be emitted
float            Dist_Min_;     //Minimum emission distance
int              PPS_;          //Particle Per Second
float            Direction_;    //Particle direction
int              Spread_;       //Directional spread; Set to 0 for no spread
float            Conserve_;     //Law of Conservation of Energy
float            Size_;         //Size of each particle
int              Size_Rand_;    //Size variation of each particle
float            Speed_;        //Scales the speed of all particles by this value
int              Sp_Rand_;      //Range for the random generation of speed
                                //Set to 0 for no random speed
float            Lifetime_;     //Lifetime of all particles
int              Life_Rand_;    //Range for the random generation of lifetime
                                //Set to 0 for no random lifetime
Color            Color_;        //Color of particles
float            Transparency_; //Transparency of particles
float            Exposure_;     //Exposure of particles
vector<Particle> Particles_;    //Dynamic array of particles
};

//Particle system takes care of the environment and
//keeping the whole system up to date. 
struct Particle_System {
//Initialize emitter
Particle_System(AEGfxVertexList* pMesh, AEVec2 Pos)
:Emitter_{ pMesh, Pos }, p_count{ 0 }
{}
//Call emitter destructor to clear vector
~Particle_System()
{
  //Just to be explicit
  Emitter_.~Emitter();
}
//Simulates Brownian Motion
//User specifies the equation for phase x and phase y
void Turbulence(const float Strength, 
                const function<float()>& PhaseX = 
                []() ->float {return cosf((float)time(nullptr)); },
                const function<float()>& PhaseY = 
                []() ->float {return sinf((float)time(nullptr)); }
               );
//Simulates speed dampening
void Drag(const float Strength);
//Enacts a force in the specified direction, X or Y
void Force(const float Strength, const bool X, const bool Y);
//Gravity
void Gravity(const float Strength);
//Enables exposure to be a factor of each particle's lifetime
void ColorRamp_Life();
//Enables transparency to be a factor of each particle's exposure
void TransRamp_Exp();
//Creates each particle based on the emitter's attributes
Particle Create();
//Emits new particles
void UpdateEmission();
//Updates the whole system
void Update(const float dt);
//Warm up time
void WarmUp(const float dt, const float time);
//Renders the particles
void Render();
//Emitter
Emitter Emitter_;
//Counter for the number of particles birthed so far
int p_count;
};