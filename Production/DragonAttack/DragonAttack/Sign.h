/* Start Header ************************************************************************/
/*!
\file       Sign.h
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
class Sign : public GameObject
{
private:
public:
	Sign(float x, float y); // default constructor
	bool ShowTutorial;
	void Update(Dragon &player, const float &dt);
};
