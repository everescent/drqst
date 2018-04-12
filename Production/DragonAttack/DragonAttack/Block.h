/* Start Header ************************************************************************/
/*!
\file    Block.h
\project Dragon Attack
\author  Javon Lee
\email   xiongweijavon.lee@digipen.edu
\brief
Block class is declared here
All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/

#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "Dragon.h"
#include "Create_Object.h"
// Only include this namespace if the macro is defined 
#ifdef USE_BLOCK_MACRO
namespace Block_Macro{
	const float half_block_width = 40.0f; // This is HALF the block's width 
	const float half_block_height = 45.0f; // This is HALF the block's height
}
#endif

/****************************************************************************************
Description:
This is a class that is represents a static object in the game. They form the building 
blocks of the map, thus the name Block. 
****************************************************************************************/
class Block : public GameObject
{
public:
/****************************************************************************************
Description:
This is a constructor for the Block Object
p_Sprite: is a ptr to the Sprite that the block has
x: The x-coordinate position
y: The y-coordinate position
****************************************************************************************/
	Block(Sprite* const p_Sprite, float x, float y);
/****************************************************************************************
Description:
This function updates the collision reaction between the static object and the Dragon class
****************************************************************************************/
	void Update(Dragon& obj, const float &dt);
/****************************************************************************************
Description:
This function updates the collision reaction between the static object and all the other Character class
****************************************************************************************/
	void Update(Characters& obj, const float &dt);

private:
	
	AEVec2 Top_HS1;
	AEVec2 Top_HS2;
	AEVec2 Bottom_HS1;
	AEVec2 Bottom_HS2;
	AEVec2 Left_HS1;
	AEVec2 Left_HS2;
	AEVec2 Right_HS1;
	AEVec2 Right_HS2;
};

