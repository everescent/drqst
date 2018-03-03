#pragma once
#include "Characters.h"
#include "Projectile.h"
#include "Boss_States.h"
#include "Dragon.h"
#include <cmath>

namespace ArcherMacros {
  //Global Read-Only Variables START///////////////////////////////////////////////////////
  const int   Archer_HP{ 10 }; //Archer HP
  const float Archer_Scale{ 100.0f }; //Scale of Archer mesh
  const float Archer_Speed{ 120.0f }; //Scale of Archer mesh
  const float Arrow_Scale{ 20.0f }; //Scale of arrow mesh
  const float Arrow_Death{ 800.0f }; //Scale of arrow mesh
  const float Archer_LOS{ 250.0f }; //Line Of Sight
  const float Archer_CD_Time{ 2.0f }; //Attack Interval
  const float Archer_Max_Dist{ 100.0f }; //Maximum travelling distance
  const float Gravity{ 10.0f }; //Gravity
  //Global Read-Only Variables END/////////////////////////////////////////////////////////
}
class Archer : public Characters {

public: 
  //Constructs Archer with desired position
  Archer(const float posX = 0.0f, const float posY = 0.0f);
  //Clears arrow vector memory
  ~Archer() = default;
  //Updates the Archer
  void Update(Dragon& player, const float dt);
  //Renders Archer and attacks
  void Render();

private:

  Projectile Arrow;                            //Arrow to shoot
  void(Archer::*Archer_State)(Dragon &player, const float dt); //Pointer to current state function
  Boss_Action_State A_Curr;                    //Current state
  Boss_Action_State A_Next;                    //Next state
  bool Dragon_Seen;                            //Flag to check if dragon is seen
  bool Attack_;                                //Check if attack on cooldown
  float Distance;                              //Distacne travelled by Archer
  float Arrow_CD;                              //Arrow cooldown timer
  float Angle;

//Attack Functions START///////////////////////////////////////////////////////////////
  void Attack_Update(Dragon &player, const float dt);  //Arrow rain update
  void Colision_Check(Dragon &player, const float dt); //Checks if attacks hit
//Attack Functions END/////////////////////////////////////////////////////////////////

//State Functions START////////////////////////////////////////////////////////////////
  void Idle(Dragon &player, const float dt);       //Idle state function
  void Move(Dragon &player, const float dt);       //Move state function
  void Attack(Dragon &player, const float dt);     //Attack state function
  void CheckState(Dragon &player, const float dt); //Sets current state to next, checks for next state
//State Functions END//////////////////////////////////////////////////////////////////
};