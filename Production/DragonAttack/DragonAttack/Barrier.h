/* Start Header ************************************************************************/
/*!
\file       Barrier.h
\author     Andrew Chong
\par email: c.jiahaoandrew\@digipen.edu
\brief
Character class, parent class of a destructable object to be created

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

// A class that creates a basic character
class Barrier : public GameObject
{
private:

public:
	Barrier(Sprite* const p_Sprite, float x, float y); // default constructor
	void Update(Dragon &player, const float &dt);
};

//enum BOX_SPAWN
//{
//	SPD1,
//	SPD2,
//	DMG1,
//	DMG2,
//	DMG3,
//	INVUL1
//};