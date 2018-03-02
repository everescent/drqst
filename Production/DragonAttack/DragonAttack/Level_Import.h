#pragma once
#pragma once
#include "AEEngine.h"
#include "GameObject.h"
#include "Create_Object.h"
#include <cmath>
#include <vector>
#include <iostream>

namespace Level_Import
{

	void Init(void);

	void Load(void);

	void Update(float dt);

	void Draw(void);

	void Free(void);

	void Unload(void);

}
//determine necessary functions for Level Editor 
 void SnapToCell(float *Coordinate);

//void FreeMapData(void); Got to free the allocated memory 

bool Import_MapData(char* filename); //Allocates the memory for the array 
