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

class Dragon; //Foward declaration of Dragon

/****************************************************************************************
Description:
  Indicates the type of pick up
****************************************************************************************/
enum PickUpType {
  HP = 0, //Health Point +1
  DMG   , //Damage up
  SPD   , //Speed up
  INVUL , //Invul
  COIN    //Increase score
};

//Alias for PickUpType
using PUT = PickUpType;

const float PickUp_Scale{ 30.0f }; //Scale of the pickup object
const float DMG_CD      { 20.0f }; //Damage power up cooldown time
const float SPD_CD      { 30.0f }; //Speed power up cooldown time
const float INVUL_CD    { 15.0f }; //Invulnerability cooldown time 

/****************************************************************************************
Description:
  A pickup object. This object can become any pickup-able item in the game. User have
  to specify what type the pickup is in the constructor. 
****************************************************************************************/
class PickUp : public GameObject{

public:

  /**************************************************************************************
  Description:
    Initialize with sprite, collision, type of power up and position. 
  p_sprite:
    Object sprite. 
  t_col:
    Collision box of the object. 
  type:
    Type of object. 
  posX:
    Position X of the object. 
  posY:
    Position Y of the object. 
  **************************************************************************************/
  PickUp(Sprite* p_sprite, Col_Comp && t_col, const PUT type,
         const float posX, const float posY                   );

  /**************************************************************************************
  Description:
    Updates collision and cooldown.
  player:
    Player controlled object. 
  dt:
    Delta time. 
  **************************************************************************************/
  void        Update   (Dragon &player, const float dt        );

  /**************************************************************************************
  Description:
    Returns the number of coins collected. 
  **************************************************************************************/
  static int  GetCoin  () { return Coin_Counter; }

  /**************************************************************************************
  Description:
    Resets the number of coins collected. 
  **************************************************************************************/
  static void ResetCoin() { Coin_Counter = 0; }

  //Particle system for coin
  static Particle_System* coin_particles;

private:
  PUT        Type_       ; //Type of power up
  static int Coin_Counter; //Counts the number of coins collected
  float      Cooldown_   ; //Cooldown timer
  bool       Active_     ; //Power Up active or not

};
