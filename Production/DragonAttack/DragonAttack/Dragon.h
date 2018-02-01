#pragma once
#include "GameObject.h"
/*#include "Projectile.h"*/
#include "Characters.h"

//Indicates the type of power up
enum PowerUp {
  HP = 0, //Health Point +1
  DMG,    //Damage up
  SPD,    //Speed up
  INVUL   //Invul
};

class Dragon : public Characters/*, public Projectile */{

public:
  void Input();  //Handles player input
  void Update(); //Updates data members based on input

private:
  bool Attack;  //Check if player is attacking
  bool Pwr_Up;  //Check if power up is in effect
  int Damage;   //Amount of damage each attack does
  int Charge;   //Charge for Mega Fireball
  PowerUp Buff; //Type of power up

  //Private Functions START//////////////////////////////////////////////////////////////
  void ApplyPowerUP();
  //Private Functions END////////////////////////////////////////////////////////////////
};
