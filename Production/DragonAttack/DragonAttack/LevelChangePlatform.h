/* Start Header ************************************************************************/
/*!
\file       LevelChangePlatform.h
\author     Andrew Chong
\par email: c.jiahaoandrew\@digipen.edu
\brief
Character class, parent class of the characters to be created

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "Dragon.h"
#include "Create_Object.h"
#include "GameStateManager.h"

// A class that creates a basic character
class LevelChangePlatform : public GameObject
{
private:

public:
	LevelChangePlatform(Sprite* const p_Sprite, float x, float y); // default constructor
	void Update(Dragon &player, const float &dt);
};
