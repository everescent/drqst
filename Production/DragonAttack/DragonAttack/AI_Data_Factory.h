#pragma once
#include "Characters.h"
#include "Grunt.h"
#include "Mage.h"
#include "Merlin.h"
#include "King_Arthur.h"

enum BASIC_AI
{
	GRUNT,
	MAGE,

	// add any mobs that you have created here
};

enum BOSS_AI
{
	LANCELOT,
	MERLIN,
	KING_ARTHUR

	// add any boss you create here
};

Characters *Create_Basic_AI(const BASIC_AI mob_name, const AEVec2 &initial_position);

Characters *Create_Boss_AI(const BOSS_AI boss_name);