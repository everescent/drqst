/* Start Header ************************************************************************/
/*!
\file       Particle_Effects.h
\project    Dragon Attack
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Header file for Particle_Effects

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "Particle_System.h"

/**************************************************************************************
//
// Enumeration for the different particle effects that is used for the game
//
**************************************************************************************/
enum EFFECTS
{
    // For stage/player
	MFIREBALL_PARTICLE,
    COIN_PARTICLE,

    // For lancelot
    ME_PARTICLE,
    ARONDIGHT_PARTICLE,

    // For King Arthur
    KA_SLASH_PARTICLE,
	KA_HEALING_PARTICLE,
    KA_SWORD_PARTICLE,

	// Miscellaneous
	CREDIT_PARTICLE,
	SCORE_PARTICLE,
	PHASE_PARTICLE,
	CURSOR_PARTICLE,

};

/**************************************************************************************
//
// initialize the meshes and particle systems that will be used in the game
// CALLED ONLY ONCE AT THE START OF THE GAME INITIALIZING PHASE
//
**************************************************************************************/
void Effects_Init(void);


/**************************************************************************************
//
// Returns a pointer to the corresponding particle system that the user asked for
//
**************************************************************************************/
Particle_System* Effects_Get(EFFECTS);

/**************************************************************************************
//
// deletes the memory that was allocated for partial effects and mesh 
// CALLED ONLY ONCE AT THE END OF THE GAME
//
**************************************************************************************/
void Effects_Cleanup(void);