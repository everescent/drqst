/* Start Header ************************************************************************/
/*!
\file       PickUp.h
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  PickUp class body declared here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "GameObject.h"
#include "Dragon.h"

class Dragon;

//Indicates the type of pick up
enum PickUpType {
  HP = 0, //Health Point +1
  DMG,    //Damage up
  SPD,    //Speed up
  INVUL,  //Invul
  COIN    //Increase score
};

//Alias for PickUpType
using PUT = PickUpType;

//Scale of the pickup object
const float PickUp_Scale{ 30.0f };

class PickUp : public GameObject{

public:
  PickUp(Sprite &&t_sprite, Col_Comp && t_col, const PUT type, const float posX, const float posY);
  void Update(Dragon &player, const float dt);
  //Returns the number of coins collected
  static int GetCoin() { return Coin_Counter; }
  //Resets teh number of coins collected
  static void ResetCoin() { Coin_Counter = 0; }

private:
  PUT Type_;
  static int Coin_Counter;
  float Cooldown_;
  bool Active_;
};
