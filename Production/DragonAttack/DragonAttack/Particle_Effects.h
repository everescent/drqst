#pragma once
#include "Particle_System.h"

enum EFFECTS
{
    MFIREBALL_PARTICLE,
    COIN_PARTICLE,
    ARONDIGHT_PARTICLE,
	KA_HEALING_PARTICLE,
};

void Effects_Init(void);
Particle_System* Effects_Get(EFFECTS);
void Effects_Cleanup(void);