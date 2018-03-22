#pragma once
#pragma once
#include "AEEngine.h"
#include "GameObject.h"
#include "Create_Object.h"
#include <cmath>
#include <vector>
#include <iostream>

enum OBJ_TYPE
{
	//OBJ_TYPE_EMPTY,   // 0
	//OBJ_PLATFORM,     // 1
	//OBJ_FLOOR,        // 2
	//OBJ_GRUNT,		// 3
	//OBJ_SC,			// 4
	//OBJ_WALL,		    // 5
	//OBJ_BARRIER,	    // 6
	//OBJ_COIN,		    // 7
	//OBJ_HP,		    // 8
	//OBJ_SPD,		    // 9
	//OBJ_INVUL,		// 10
	//OBJ_DMG,		    // 11
	//OBJ_ARCHER,		// 12

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

//determine necessary functions for Level Editor 
 void SnapToCell(float *Coordinate);

//void FreeMapData(void); Got to free the allocated memory 

 bool Import_MapData(char* filename, int**& MapData, int& Map_Width, int& Map_Height); //Allocates the memory for the array 

float Convert_Y(float& y);

float Convert_X(float& x);

void PrintRetrievedInformation(int** MapData, int Map_Width, int Map_Height);

