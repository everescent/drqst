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
#include "f_sqrt.h"
#include <utility>

class Projectile : public GameObject {

public:
  //Updates the projectile, if active
  void Update(const float dt, const float scale = 0.0f, const bool circle = false, const float angle = 90.0f)
  {
    //Update distance travelled and resultant matrix
    if (IsActive())
    {
      if (GetDir())
      {
        PosX += GetVelocity().x * dt;
        PosY += GetVelocity().y * dt;
        Transform_.SetRotation(angle);
      }
      else
      {
        PosX -= GetVelocity().x * dt;
        PosY -= GetVelocity().y * dt;
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
      AddDist(
       f_sqrt( (GetVelocity().x) * dt * (GetVelocity().x * dt)
                                       +
              (GetVelocity().y) * dt * (GetVelocity().y) * dt)
      );
      Transform_.Concat();
    }
  }
  //Set the position of the projectile, if not active
  void Pos(const float &X, const float &Y)
  {
    //Check for active
    if (!IsActive())
    {
      PosX = X;
      PosY = Y;
    }
  }
  //Distance travelled by the projectile
  void AddDist(const float dist) { Distance += dist; }
  //Returns Distance travelled by the projectile
  float GetDist() const { return Distance; }
  //Resets the distance travelled
  void ResetDist() { Distance = 0.0f; }
  //Sets the direction of the projectile
  void SetDir(const bool dir) { Direction = dir; }
  //Returns the direction of the projectile
  bool GetDir() const { return Direction; }
  //Sets Collided true or false
  void SetCollided(const bool Colide) { Collided = Colide; }
  //Returns Collided
  bool GetCollided() const { return Collided; }
  //Default contructor does nothing
  Projectile() = default;
  //Initialize projectile with mesh and collision shape
  Projectile(const Sprite *p_Sprite, Col_Comp &&t_col)
    :GameObject{ p_Sprite, std::move(t_col) },
    Distance{ 0.0f }, Direction{ true }, Collided{ false }
  {}
private:
  bool Collided;  //TRUE = Collided; FALSE = Not Collided
  bool Direction; //TRUE = Right; FALSE = Left
  float Distance; //Distance travelled
};
