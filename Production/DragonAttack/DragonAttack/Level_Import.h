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
	OBJ_TYPE_EMPTY, // 0
	OBJ_PLATFORM,   // 1
	OBJ_FLOOR,      // 2
	OBJ_GRUNT,		// 4
	OBJ_SC
};

//determine necessary functions for Level Editor 
 void SnapToCell(float *Coordinate);

//void FreeMapData(void); Got to free the allocated memory 

 bool Import_MapData(char* filename, int**& MapData, int& Map_Width, int& Map_Height); //Allocates the memory for the array 

float Convert_Y(float& y);

float Convert_X(float& x);

void PrintRetrievedInformation(int** MapData, int Map_Width, int Map_Height);

