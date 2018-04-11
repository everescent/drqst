/* Start Header ************************************************************************/
/*!
\file       King_Arthur.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file creates, set the behaviour and rules for the last boss king arthur

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include "Characters.h"  // parent class
#include "Boss_Phase.h"  // boss_phase
#include "Boss_States.h" // boss_attack, Boss_Action_State
#include "Dragon.h"      // player class

class King_Arthur : public Characters
{
private:
    // enumerators for king arthur attacks
	enum KA_MoveSet
    {
        SINGLE_SLASH = 0,
        TRIPLE_SLASH,
        DASH = 4,
        HEAL,
        SPIN_SWORD,
        NONE
    };

    //------------------------------------------------------------------
    //
    // INITIALIZERS FOR KING ARTHUR
    //
    //------------------------------------------------------------------
    void Init_KA_Attacks(void);    // initialize king arthur attacks
    void Init_MobArray(void);      // initialize the mob array for phase 2
    void Init_Particle(void);      // initializes the various particle effects ka will have
    
    //------------------------------------------------------------------
    //
    // HELPER FUNCTIONS FOR KING ARTHUR
    //
    //------------------------------------------------------------------
    void Set_Forward_Dir(const Dragon&);     // set the direction king arthur should face 
    void Off_Particles(void);                // set all particle active to false
	void Update_Particle(const float dt);    // update the particle system
	void King_Arthur_Phase2(const float dt); // changes mechanics for phase 2
    void King_Arthur_Phase3(const float dt); // changes mechanics for phase 3
    
    //------------------------------------------------------------------
    //
    // BEHAVIOR FOR KING ARTHUR
    //
    //------------------------------------------------------------------
    void Idle(const Dragon &d, const float dt);
    void Moving(const Dragon &d, const float dt);
    void Attack(Dragon &d, const float dt);

    //------------------------------------------------------------------
    //
    // ATTACKS FOR KING ARTHUR
    //
    //------------------------------------------------------------------
    void Dash_Attack(Dragon &d, const float dt);     // jump attack
    void Single_Slash(Dragon &d, const float dt);    // single slash + wave
    void Triple_Slash(Dragon &d, const float dt);    // single slash 3 times
    void Heal_and_Spawn(Dragon &d, const float dt);  // phase 2, heal and spawm mobs
    void Spinning_Blades(Dragon &d, const float dt); // rotating swords that attacks player

	//------------------------------------------------------------------
    //
    // VARIABLES FOR KING ARTHUR
    //
    //------------------------------------------------------------------
    BOSS_PHASE                ka_phase;           // current phase of king arthur
    Particle_System *         healing_effect;     // particle system for healing effect
    Particle_System *         sword_effect;       // particle system for the swords in phase 3
    Particle_System *         slash_effect[3];    // particle system for the slashes
    Particle_System *         phase_effect;       // particle system for phase transition
    KA_MoveSet                currAttk;           // current attack king arthur is using
    Animation		          anim;               // king arthur's animation
    Boss_Action_State         current_action;     // different states of boss arthur
    std::vector <Characters*> mobs;               //an array to store the mobs to be spawn
    float                     timer;              // for phase transition
    float                     mob_timer;          // render the mobs base on time
    Audio_Engine              music;              // music for king arthur
    char                      active_mobs;        // number of mobs to spawn
    bool                      spawn_mob;          // check when to update and render mob
    char                      behavior_swap;      // switch between idling and moving
    bool                      healing;            // determine if king arthur is healing

public:
    //------------------------------------------------------------------
    //
    //  CONSTRUCTOR AND DESTRUCTOR
    //
    //------------------------------------------------------------------
	King_Arthur(Sprite*);
    ~King_Arthur(); 
	
	//------------------------------------------------------------------
    //
    //  PUBLIC FUNCTIONS
    //
    //------------------------------------------------------------------
    void Update(Dragon &player, const float dt);  // updates king arthur
    void Render();                                // render king arthur
    void Dead(void);                              // update king arthur when he dies
    BOSS_PHASE Get_Phase(void) const;             // get the current phsae of king arthur
    std::vector <Characters*>& Get_Mobs(void);    // get the vector storing the mobs
};

