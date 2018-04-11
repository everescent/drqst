/* Start Header ************************************************************************/
/*!
\file       Lancelot.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file creates, set the behaviour and rules for the first boss Lancelot

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include "Characters.h"   // parent class of lancelot
#include "Dragon.h"       // player class
#include "Boss_Phase.h"   // enum for lancelot phase
#include "Boss_States.h"  // boss_attack, boss_action_state

class Lancelot : public Characters
{
private:
  //------------------------------------------------------------------
  //
  //  BEHAVIOUR FOR LANCELOT
  //
  //------------------------------------------------------------------
  void Idle(const Dragon &  , const float dt); // Lancelot idle state
  void Moving(const Dragon& , const float dt); // moving state of Lancelot
  void Attack(Dragon &      , const float dt); // attack state of Lancelot
  void Dead(void);

  //------------------------------------------------------------------
  //
  //  INITIALIZERS FOR LANCELOT VARIABLES
  //
  //------------------------------------------------------------------
  void Init(void);             // initializer for Lancelot
  void Init_Stab(void);        // initialize the variables for stab attack
  void Init_Slash(void);       // initialize the variables for slash attack
  void Init_Arondight(void);   // initialize the variables for Arondight
  void Init_Particles(void);   // initialize the particle systems used by Lancelot
  
  //------------------------------------------------------------------
  //
  //  HELPER FUNCTIONS FOR LANCELOT
  //
  //------------------------------------------------------------------
  void Set_Face_Dir(const Dragon &);     // set the texture facing direction
  void Set_Attk_Dir(void);               // set the attack direction of Lancelot
  void Lancelot_Phase2(const float dt);  // set Lancelot to go to phase 2
  void Update_Particles(const float dt); // updates the particle systems

  //------------------------------------------------------------------
  //
  //  LANCELOT ATTACK FUNCTIONS
  //
  //------------------------------------------------------------------
  void Stab(Dragon&, const float dt);      // first basic attack of  Lancelot
  void Slash(Dragon&, const float dt);     // second basic attack of Lancelot
  void Mad_Enhancement(const float dt);    // makes Lancelot move faster, shorter cooldown
  void Arondight(Dragon&, const float dt); // phase 2 mechanism, long range attack


  //------------------------------------------------------------------
  //
  //  LANCELOT VARIABLES
  //
  //------------------------------------------------------------------
  BOSS_PHASE        phase;                   // flag for lancelot phase
  bool              M_E;                     // lancelot berserk state
  Particle_System  *arondight_particle;      // particle system for arondight
  Particle_System  *me_particle;             // particle system for mad enhancement
  Particle_System  *phase_particle;          // particle effect for phase transition
  Animation         anim;                    // lancelot animation
  float             timer;                   // timer for phase transition
  Boss_Action_State current_action;          // lancelot current action
  Audio_Engine      music;                   // music for lancelot

public:
  //------------------------------------------------------------------
  //
  //  CONSTRUCTOR AND DESTRUCTOR
  //
  //------------------------------------------------------------------
  Lancelot(Sprite*);  // default constructor for lancelot
  ~Lancelot();        // destructor for lancelot
  
  //------------------------------------------------------------------
  //
  //  UPDATE AND RENDER FUNCTIONS FOR LANCELOT
  //
  //------------------------------------------------------------------
  void Update(Dragon &player, const float dt);
  void Render(void);

  void Mute(void) override;
  void Unmute(void) override;

};