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

enum EFFECTS
{
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

};

void Effects_Init(void);
Particle_System* Effects_Get(EFFECTS);
void Effects_Cleanup(void);