#pragma once
#include "Sprite.h"
#include "Create_Object.h"

enum ATTACK_SPRITE
{
	EBALL_SPRITE,
	STAB_SPRITE,
	ARROW_SPRITE
};

void Attack_Sprite_Init(void);
Sprite* Get_Attack_Sprite(const ATTACK_SPRITE);