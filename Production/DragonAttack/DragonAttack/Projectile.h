/* Start Header ************************************************************************/
/*!
\file       Projectile.h
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Projectile class declared here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "GameObject.h"
#include "f_sqrt.h" //f_sqrt
#include <utility>  //move

/**************************************************************************************
Description:
  Creates an object that acts like any projectile. Bullets, arrows, etc. 
Constructors:
  Create this class with a sprite pointer and a collision part. 
**************************************************************************************/
class Projectile : public GameObject {

public:

  /**************************************************************************************
  Description:
    Updates the projectile, if active.
  dt:
    Delta time.
  scale:
    Collision scale. 
  circle:
    Check if circle collision type. If not, use rectangle. 
  angle:
    Rotation angle of the projectile. 
  **************************************************************************************/
  void Update(const float dt            , const float scale = 0.0f,
              const bool  circle = false, const float angle = 90.0f )
  {
    //Update distance travelled and resultant matrix
    if (IsActive())
    {
      if (GetDir())
      {
        PosX += GetVelocity().x * dt ;
        PosY += GetVelocity().y * dt ;
        Transform_.SetRotation(angle);
      }
      else
      {
        PosX -= GetVelocity().x * dt  ;
        PosY -= GetVelocity().y * dt  ;
        Transform_.SetRotation(-angle);
      }
      Transform_.SetTranslate(PosX, PosY);
      //If circle update circle
      if(circle)
        Collision_.Update_Col_Pos(PosX, PosY);
      //If not update by rectangle
      else
        Collision_.Update_Col_Pos(PosX - scale, PosY - scale, 
                                  PosX + scale, PosY + scale);
      //Pythagoras distance equation
      AddDist
      (
        f_sqrt
        ( 
          (GetVelocity().x * dt * GetVelocity().x * dt)
                                +
          (GetVelocity().y * dt * GetVelocity().y * dt)
        )
      );
      //Concatenate all matrices
      Transform_.Concat();
    }
  }

  /**************************************************************************************
  Description:
    Set the position of the projectile, if not active.
  X:
    Position X.
  Y:
    Position Y. 
  **************************************************************************************/
  void Pos(const float X, const float Y)
  {
    //Check for active
    if (!IsActive())
    {
      PosX = X;
      PosY = Y;
    }
  }

  /**************************************************************************************
  Description:
    Add distance travelled by the projectile.
  dist:
    Distance to add. 
  **************************************************************************************/
  void  AddDist    (const float dist )       { Distance += dist  ; }

  /**************************************************************************************
  Description:
    Returns Distance travelled by the projectile.
  **************************************************************************************/
  float GetDist    (                 ) const { return Distance   ; }

  /**************************************************************************************
  Description:
    Resets the distance travelled.
  **************************************************************************************/
  void  ResetDist  (                 )       { Distance  = 0.0f  ; }

  /**************************************************************************************
  Description:
    Sets the direction of the projectile.
  dir:
    Direction of projectile. TRUE = Right; FALSE = Left;
  **************************************************************************************/
  void  SetDir     (const bool dir   )       { Direction = dir   ; }

  /**************************************************************************************
  Description:
    Returns the direction of the projectile.
  **************************************************************************************/
  bool  GetDir     (                 ) const { return Direction  ; }

  /**************************************************************************************
  Description:
    Sets Collided true or false.
  Colide:
    Collision status. 
  **************************************************************************************/
  void  SetCollided(const bool Colide)       { Collided  = Colide; }

  /**************************************************************************************
  Description:
    Returns Collided status. TRUE = Collided; FALSE = Not Collided;
  **************************************************************************************/
  bool  GetCollided(                 ) const { return Collided   ; }

  /**************************************************************************************
  Description:
    Default contructor does nothing useful.
  **************************************************************************************/
  Projectile() = default;

  /**************************************************************************************
  Description:
    Initialize projectile with mesh and collision shape.
  p_Sprite:
    Pointer to sprite of projectile. 
  t_col:
    Collision component of the projectile. 
  **************************************************************************************/
  Projectile(Sprite *p_Sprite, Col_Comp &&t_col)
    :GameObject{ p_Sprite, std::move(t_col) },
     Distance  { 0.0f                       }, 
     Direction { true                       }, 
     Collided  { false                      }
  {}

private:

  bool  Collided ; //TRUE = Collided; FALSE = Not Collided
  bool  Direction; //TRUE = Right; FALSE = Left
  float Distance ; //Distance travelled
};
