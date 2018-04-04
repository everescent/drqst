/* Start Header ************************************************************************/
/*!
\file       Particle_Effects.h
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file stores the different particle effects for the game

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
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

};

/**************************************************************************************
//
// initialize the meshes and particle systems that will be used in the game
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
//
**************************************************************************************/
void Effects_Cleanup(void);