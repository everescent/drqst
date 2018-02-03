/* Start Header ************************************************************************/
/*!
\file       Projectile.h
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Projectile class declared here.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "GameObject.h"
#include <utility>

class Projectile : public GameObject {

public:
  void Update() {}
  void AddDist(const float &dist) { Distance += dist; }
  float GetDist() const { return Distance; }
  void ResetDist() { Distance = 0.0f; }
  void SetDir(const bool &dir) { Direction = dir; }
  bool GetDir() const { return Direction; }
  Projectile() = default;
  Projectile(Sprite &&t_sprite, Col_Comp &&t_col)
    :GameObject{ std::move(t_sprite), std::move(t_col) }, Distance{ 0.0f }, Direction{ true }
  {}
private:
  bool Direction; //TRUE = Right; FALSE = Left
  float Distance; //Distance travelled
};
