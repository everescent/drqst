#include "AI_Attack_Sprites.h"

namespace
{
	Sprite EBALL_ATTACK;
	Sprite STAB_ATTACK;
	Sprite ARROW_ATTACK;
}

void Attack_Sprite_Init(void)
{
	EBALL_ATTACK = S_CreateSquare(20.0f, "energyball.png");
	STAB_ATTACK  = S_CreateSquare(40.0f, ".//Textures/arondight.png");

	EBALL_ATTACK.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	 STAB_ATTACK.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

const Sprite* Get_Attack_Sprite(const ATTACK_SPRITE ATTACK)
{
	switch (ATTACK)
	{
	case EBALL_SPRITE:return &EBALL_ATTACK;
		break;
	case STAB_SPRITE: return &STAB_ATTACK;
		break;
	case ARROW_SPRITE:
		break;
	default: return nullptr;
	}
}

