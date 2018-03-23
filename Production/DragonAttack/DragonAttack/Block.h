#pragma once
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
class Block : public GameObject
{
public:
	Block(Sprite* const p_Sprite, float x, float y);

	void Update(Dragon& obj, const float &dt);


private:
	const float half_block_width; // This is HALF the block's width 
	const float half_block_height; // This is HALF the block's height
	AEVec2 Top_HS1;
	AEVec2 Top_HS2;
	AEVec2 Bottom_HS1;
	AEVec2 Bottom_HS2;
	AEVec2 Left_HS1;
	AEVec2 Left_HS2;
	AEVec2 Right_HS1;
	AEVec2 Right_HS2;
};

