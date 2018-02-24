/* Start Header ************************************************************************/
/*!
\file       Platform.h
\author     Andrew Chong/Jacob Lim
\par email: c.jiahaoandrew\@digipen.edu/jacob.lim\@digipen.edu
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

// A class that creates a basic character
class Floor : public GameObject
{
private:

public:
	Floor(float x, float y); // default constructor
  //(Jacob) Changed to take in player and dt
	void Update(Dragon &player, const float &dt);
	void Update() override {};
};