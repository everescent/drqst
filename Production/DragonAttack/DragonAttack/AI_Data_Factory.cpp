/* Start Header ************************************************************************/
/*!
\file       AI_Data_Factory.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file creates ais base on the enum that was passed in

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "AI_Data_Factory.h"

Characters *Create_Basic_AI(const BASIC_AI mob, const AEVec2& position)
{

	switch (mob)
	{
	case GRUNT:  return new Grunt (position.x, position.y);
		break;
	case MAGE:   return new Mage (position);
		break;
    case ARCHER: return new Archer(position.x, position.y);
	    break;
	case KNIGHT: return new Knight(position);
		break;
	default: break;
	}
	
	// returns nullptr if no match was found
	return nullptr;
}

Characters *Create_Boss_AI(const BOSS_AI boss)
{
	switch (boss)
	{
	case LANCELOT:    return new Lancelot;
		break;
	case MERLIN:      return new Merlin;
		break;
	case KING_ARTHUR: return new King_Arthur;
		break;

	default: break;
	}
	
	// returns null ptr if no match was found
	return nullptr;
}