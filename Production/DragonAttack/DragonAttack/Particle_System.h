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

/****************************************************************************************
Description:
  Describes the type of emitter. 
****************************************************************************************/
enum Emitter_Type
{
  CENTER, //Point emission
  BOX     //Surface emission
};

/****************************************************************************************
Description:
  Stores the values to randomize the particle variables.
  Set to 0 if it is not meant to be used. 
****************************************************************************************/
struct Particle_Range
{
  int Spread_   ; //Directional spread; Set to 0 for no spread
  int Size_Rand_; //Size variation of each particle
  int Sp_Rand_  ; //Range for the random generation of speed
  int Life_Rand_; //Range for the random generation of lifetime

  /**************************************************************************************
  Description:
    Default constructor to zero initialize the variables. 
  **************************************************************************************/
  Particle_Range()
  :Spread_{}, Size_Rand_{}, Sp_Rand_{}, Life_Rand_{}
  {}
};

/****************************************************************************************
Description:
  Emitter handles the processing of the particle attributes and 
  also emission properties.
****************************************************************************************/
struct Emitter {

  /**************************************************************************************
  Description:
    Construct emitter with a mesh for particles and its position.
    Initialize mesh and position, everything else is 0. 
    User should be keying in desired values. 
    Doing this in a "function" style is too messy. 
  **************************************************************************************/
  Emitter(AEGfxVertexList* pMesh, AEVec2 Pos, Emitter_Type type);

  /**************************************************************************************
  Description:
    Clears the vector of particles.
  **************************************************************************************/
  ~Emitter();
  
  /**************************************************************************************
  Description:
    This defines the position attributes for the different type of emitters.
  **************************************************************************************/
  union Particle_Pos
  {
    /************************************************************************************
    Description:
      This puts all the variables needed for both point and surface emitter.
      The emitter only uses what is needed based on the type. 
    ************************************************************************************/
    struct Box{
        AEVec2 Point_Min; //Minimum point
        AEVec2 Point_Max; //Maximum point
        float  Angle_   ; //Angle of orientation
    };
    Box    Min_Max         ; //For surface emission (box)
    AEVec2 Point           ; //For point emitter
    AEVec2 Point_Min_Max[2]; //For array access
  };
  
//EMITTER ATTRIBUTES START///////////////////////////////////////////////////////////////
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
//EMITTER ATTRIBUTES END/////////////////////////////////////////////////////////////////
};


/****************************************************************************************
Description:
  Particle system takes care of the environment and
  keeping the whole system up to date. 
  Call UpdateEmission first, followed by the fields. Lastly, call update. 
  Needless to say, please set your emitter settings first 
  before you call these functions. 
****************************************************************************************/
struct Particle_System {

  /**************************************************************************************
  Description:
    Initialize emitter.
  **************************************************************************************/
  Particle_System(AEGfxVertexList* pMesh, AEVec2 Pos, Emitter_Type type)
  :Emitter_{ pMesh, Pos, type }, p_count{ 0 }
  {}

  /**************************************************************************************
  Description:
    Call emitter destructor to clear vector. 
  **************************************************************************************/
  ~Particle_System()
  {
    //Just to be explicit
    Emitter_.~Emitter();
  }

  /**************************************************************************************
  Description:
    Simulates Brownian Motion. 
    User can specify the equation for phase x and phase y. 
  Strength:
    How strong the turbulence is. 
  PhaseX:
    An equation for random motion in the X-axis.
  PhaseY:
    An equation for random motion in the Y-axis. 
  **************************************************************************************/
  void Turbulence(const float Strength,
                  //Default equation for movement along X-axis
                  const function<float()>& PhaseX = 
                  []() -> float 
                  {  
                    float phaseRot = (float)(rand() % 360);
                    phaseRot       = AEDegToRad(phaseRot) ;
                    return cosf(phaseRot)                 ;
                  },
                  //Default equation for movement along Y-axis
                    const function<float()>& PhaseY =
                  []() -> float 
                  {
                    float phaseRot = (float)(rand() % 360);
                    phaseRot       = AEDegToRad(phaseRot) ;
                    return sinf(phaseRot)                 ;
                  });

  /**************************************************************************************
  Description:
    Simulates speed dampening. For example, Wind Resistance or Friction. 
  Strength:
    How strong the drag is. 
  **************************************************************************************/
  void     Drag          (const float Strength                            );

  /**************************************************************************************
  Description:
    Enacts a force in the specified direction, X or Y.
  Strength:
    How strong the force is. 
  X:
    Exert force in the X axis.
  Y:
    Exert force in the Y axis. 
  **************************************************************************************/
  void     Force         (const float Strength, const bool X, const bool Y);

  /**************************************************************************************
  Description:
    Simulates Gravity. Simple version. 
    This does not simulate the curvature of spacetime (although that's what gravity is).
  Strength:
    How strong gravity is. They need to know the gravity of the situation. 
  **************************************************************************************/
  void     Gravity       (const float Strength                            );
  
  /**************************************************************************************
  Description:
    Attract the particles to a certain point. Modelled after a Schwarzchild black hole.
    I wanted to do a Kerr black hole but did not know how. We did not really
    need that behavior though, so I did not spend so much time on it. But I have to say,
    even though I said attract, black holes don't attract objects. Objects fall towards
    black holes. So this really just tries to simulate particles falling towards the 
    bottom of a spacetime curvature. 
  Point:
    Newton position. AKA, the position of attract the paritcles towards. 
  Strength:
    How strong the power of attraction is. Alhtough, in terms of Physics, it really just
    means how strong the gravity is. But with this you can decide where spacetime
    curves. 
  Atten:
    Attenuation. This value decides the range in which the Newton Field affects
    the particle. 
  **************************************************************************************/
  void     Newton        (const AEVec2 Point, const float strength,
                          const float  Atten = 0.0f                       );

  /**************************************************************************************
  Description:
    Enables exposure to be a factor of each particle's lifetime. 
  **************************************************************************************/
  void     ColorRamp_Life(                                                );

  /**************************************************************************************
  Description:
    Enables transparency to be a factor of each particle's exposure.
  **************************************************************************************/
  void     TransRamp_Exp (                                                );

  /**************************************************************************************
  Description:
    Scale the particle as time passes.
  Strength:
    How strong the ramp affects the scale. 
  **************************************************************************************/
  void     ScaleRamp     (const float Strength                            );

  /**************************************************************************************
  Description:
    Creates each particle based on the emitter's attributes. 
  **************************************************************************************/
  Particle Create        (                                                );

  /**************************************************************************************
  Description:
    Emits new particles. 
  **************************************************************************************/
  void     UpdateEmission(                                                );

  /**************************************************************************************
  Description:
    Updates the whole system. 
  dt:
    Delta time. 
  **************************************************************************************/
  void     Update        (const float dt                                  );

  /**************************************************************************************
  Description:
    Warm up time.
  dt:
    Delta time.
  time:
    How long to warm up the system. 
  Fields:
    The different fields to be simulated for the warm up. Pass in a function. 
    It could be just one field or a function calling multiple fields. 
  **************************************************************************************/
  void     WarmUp        (const float dt, const float time,
                          const function<void()>     &Fields              );

  /**************************************************************************************
  Description:
    Resets the emitter. 
  **************************************************************************************/
  void Off_Emitter       (void                                            );
  
  /**************************************************************************************
  Description:
    Get the number of particles in the volume.
  **************************************************************************************/
  const int GetParticleCount() const { return p_count; }

  /**************************************************************************************
  Description:
    Renders the particles.
  **************************************************************************************/
  void Render();

  Emitter Emitter_; //Emitter
  
private:

  int p_count; //Counter for the number of particles birthed so far
};
