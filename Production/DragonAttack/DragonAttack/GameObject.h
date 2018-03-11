/* Start Header ************************************************************************/
/*!
\file       GameObject.h
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Game Object class declared here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "AEEngine.h"
#include "Transform.h"
#include "Collision.h"
#include "Sprite.h"

/****************************************************************************************
Description:
  Creates a Game Object
Constructors:
  Game Object can be constructed with a mesh or with mesh and texture or none
Deconstructor:
  None
****************************************************************************************/
class GameObject {

public:
  //Renders the object
  virtual void Render();
  //Sets the object to active or inactive
  void SetActive(bool status);
  //Returns Active or Inactive status
  bool IsActive() const { return active; }
  //Sets object position
  void SetPos(const float &X, const float &Y) { PosX = X; PosY = Y; }
  //Set Velocity
  GameObject &SetVelocity(const AEVec2 &t_vec) { Velocity = t_vec; return *this; }
  //Get Velocity
  AEVec2 GetVelocity() const { return Velocity; }
  //Default constructor; Initializes to zero
  GameObject();
  //Move constructor for sprite
  GameObject(Sprite* p_Sprite, Col_Comp &&t_Col, float x = 0.0f, float y = 0.0f);
  Transform Transform_; //Transform holds the object's position, scale and rotation
  Sprite *Sprite_;      //Sprite handles mesh, texture and rendering
  Col_Comp Collision_;  //Collision handles object collision
  float PosX;           //Position X
  float PosY;           //Position Y

private:
  bool   active;   //Active or Inactive
  AEVec2 Velocity; //Object Velocity
};
