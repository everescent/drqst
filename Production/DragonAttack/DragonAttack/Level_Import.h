/* Start Header ************************************************************************/
/*!
\file    Level_Import.h
\project Dragon Attack
\author  Javon Lee
\email   xiongweijavon.lee@digipen.edu
\brief
The Level Import functions are forward declared here

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#pragma once
#include "AEEngine.h"
#include "GameObject.h"
#include "Create_Object.h"
#include <cmath>
#include <vector>
#include <iostream>
//This is an enumeration to differentiate between the different object types in the txt file 
enum OBJ_TYPE
{
	OBJ_TYPE_EMPTY,	// 0
	OBJ_PLATFORM,	// 1
	OBJ_FLOOR,		// 2
	OBJ_WALL,		// 3
	OBJ_GRUNT,		// 4
	OBJ_ARCHER,		// 5
	OBJ_KNIGHT,		// 6
	OBJ_MAGE,		// 7
	OBJ_COIN,		// 8
	OBJ_HP,			// 9
	OBJ_SPD,		// 10
	OBJ_DMG,		// 11
	OBJ_INVUL,		// 12
	OBJ_LCPLAT		// 13
};


/****************************************************************************************
Description:
This function is used to snap a coordinate to the cell of the map
****************************************************************************************/
 void SnapToCell(float *Coordinate);
 /****************************************************************************************
 Description:
 This function allocates the memory for the array to store the Map data 
 ****************************************************************************************/
 bool Import_MapData(char* filename, int**& MapData, int& Map_Width, int& Map_Height); 
 /****************************************************************************************
 Description:
 This function converts the y-coordinate to fit the game's map size
 ****************************************************************************************/
float Convert_Y(float& y);
/****************************************************************************************
Description:
This function converts the x-coordinate to fit the game's map size
****************************************************************************************/
float Convert_X(float& x);
/****************************************************************************************
Description:
This function is used for debugging to check that the txt file is imported correctly 
****************************************************************************************/
void PrintRetrievedInformation(int** MapData, int Map_Width, int Map_Height);

