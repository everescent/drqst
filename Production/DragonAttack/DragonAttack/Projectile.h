#pragma once
#include "GameObject.h"
#include <utility>

class Projectile : public GameObject {

public:
  void Update() {}
  void AddDist(float dist) { Distance += dist; }
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
  float Distance;
};
