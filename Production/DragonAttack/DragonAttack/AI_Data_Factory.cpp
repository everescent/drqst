#include "AI_Data_Factory.h"

Characters *Create_Basic_AI(const BASIC_AI mob, const AEVec2& position)
{
	
	switch (mob)
	{
	case GRUNT: return new Grunt (position.x, position.y);
		break;
	case MAGE:  return new Mage (position);
		break;

	default: break;
	}
	
	
	return nullptr;
}

Characters *Create_Boss_AI(const BOSS_AI boss)
{
	switch (boss)
	{
	case LANCELOT:
		break;
	case MERLIN: return new Merlin;
		break;
	case KING_ARTHUR: return new King_Arthur;
		break;

	default: break;
	}
	
	
	return nullptr;
}