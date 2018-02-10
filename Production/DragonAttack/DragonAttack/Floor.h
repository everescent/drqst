/* Start Header ************************************************************************/
/*!
\file       Platform.h
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
#include <utility>
#include "GameObject.h"
#include "Transform.h"
#include "Create_Object.h"

// A class that creates a basic character
class Floor : public GameObject
{
private:

public:
	Floor(); // default constructor

	void Update(float X, float Y);
	void Update() override {};
};
