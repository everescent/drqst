/*****************************************************************************/
/*!
\file    Particle.h
\author  Jacob Lim, jacob.lim, 440002417
\par     email: jaocb.lim\@digipen.edu
\date    March 08, 2018
\brief
  This file contains the Particle struct and the Color struct. 
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/*****************************************************************************/
#pragma once
#include "AEEngine.h"

/****************************************************************************************
Description:
  Struct for RGB color values. 
****************************************************************************************/
struct Color {
  float R; //Red
  float G; //Green
  float B; //Blue

  /**************************************************************************************
  Description:
    Default initializes to 1.0f for all colors.
  **************************************************************************************/
  Color() : R{ 1.0f }, G{ 1.0f }, B{ 1.0f } {}
  ~Color() = default; //Destructor is unneeded
};

/****************************************************************************************
Description:
  Particle struct to hold all particle attributes. 
****************************************************************************************/
struct Particle {

AEGfxVertexList* pMesh_       ; //Particle mesh
AEVec2           Pos_         ; //Particle position
AEVec2           Vel_         ; //PArticle velocity
AEMtx33          Tranform_    ; //Particle final transformation matrix
Color            Color_       ; //Particle color
float            Conserve_    ; //Law of Conservation of Energy
float            Lifetime_    ; //Particle lifetime
float            Size_        ; //Size value
float            Transparency_; //Transparency value
float            Exposure_    ; //Exposure (brightness) value
bool             Active_      ; //Active or not

//Particle system will initialize the variables directly
Particle()  = default;
~Particle() = default;

/****************************************************************************************
Description:
  Updates the particle.
dt:
  Delta time. 
****************************************************************************************/
void Update(const float dt)
{
  //Holds the scale and translate values in matrices for concatenation
  AEMtx33 scale, trans ;
  //Value of velocity after multiplies by dt
  AEVec2 Spd_tmp = Vel_;
  //Decrease lifetime
  Lifetime_ -= dt      ;
  //Particle "dies" if lifetime is up or completely transparent or
  //too small or completely black (no exposure)
  if (Lifetime_ <= 0.0f || Transparency_ <= 0.0f || 
      Size_     <= 0.0f || Exposure_     <= 0.0f    )
  {
    Lifetime_     = 0.0f ;
    Transparency_ = 0.0f ;
    Size_         = 0.0f ;
    Exposure_     = 0.0f ;
    Active_       = false;
  }
  //No point updating a dead particle so return
  if (!Active_)
    return;
  //Apply Conservation Law
  AEVec2Scale  (&Vel_, &Vel_, Conserve_   );
  //Scale by dt                           
  AEVec2Scale  (&Spd_tmp, &Spd_tmp, dt    );
  //Add to position                       
  AEVec2Add    (&Pos_, &Pos_, &Vel_       );
  //Calculate final matrix
  AEMtx33Scale (&scale, Size_, Size_      );
  AEMtx33Trans (&trans, Pos_.x, Pos_.y    );
  AEMtx33Concat(&Tranform_, &trans, &scale);
}

/****************************************************************************************
Description:
  Renders the particle.
****************************************************************************************/
void Render()
{
  //No point rendering inactive particles
  if (Active_)
  {
    //Set render settings
    AEGfxSetRenderMode  (AE_GFX_RM_COLOR             );
    //Exposure is a uniform multipler throughout RGB 
    AEGfxSetTintColor   (Color_.R * Exposure_,
                         Color_.G * Exposure_,
                         Color_.B * Exposure_,
                         1.0f
                                                     );
    //Apply transparency                             
    AEGfxSetTransparency(Transparency_               );
    AEGfxSetBlendMode   (AE_GFX_BM_BLEND             );
    AEGfxTextureSet     (NULL, 0.0f, 0.0f            );
    //Set render position                            
    AEGfxSetTransform   (Tranform_.m                 );
    AEGfxMeshDraw       (pMesh_, AE_GFX_MDM_TRIANGLES);
  }
}

};
