#include "AI_Attack_Sprites.h"

namespace
{
	Sprite EBALL_ATTACK;
	Sprite STAB_ATTACK;
	Sprite ARROW_ATTACK;
	Sprite FIREBALL_ATTACK;
}

void Attack_Sprite_Init(void)
{
	EBALL_ATTACK = S_CreateSquare(20.0f, ".//Textures/Energy_Ball_Final.png");
	STAB_ATTACK  = S_CreateSquare(40.0f, ".//Textures/arondight.png");
	ARROW_ATTACK = S_CreateSquare(20.0f, ".//Textures/Arrow.png");
	FIREBALL_ATTACK = S_CreateSquare(50.0f, ".//Textures/Fireball.png");

	EBALL_ATTACK.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	STAB_ATTACK.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	ARROW_ATTACK.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	FIREBALL_ATTACK.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
}

 Sprite* Get_Attack_Sprite(const ATTACK_SPRITE ATTACK)
{
	switch (ATTACK)
	{
	case EBALL_SPRITE:return &EBALL_ATTACK;
		break;
	case STAB_SPRITE: return &STAB_ATTACK;
		break;
	case ARROW_SPRITE: return &ARROW_ATTACK;
		break;
	case FIREBALL_SPRITE: return &FIREBALL_ATTACK;
		break;
	default: return nullptr;
	}
}

