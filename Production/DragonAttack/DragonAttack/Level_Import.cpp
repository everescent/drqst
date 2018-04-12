/* Start Header ************************************************************************/
/*!
\file    Level_Import.cpp
\project Dragon Attack
\author  Javon Lee
\email   xiongweijavon.lee@digipen.edu
\brief
The Level Import functions are forward declared here

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/

#include "Level_Import.h"
#include "Platform.h"
#include "Grunt.h"
#include <vector>
#include "Camera.h"
#include "GameStateList.h"
#include "Floor.h"
#include "Scarecrow.h"


bool Import_MapData(char* filename,  int**& MapData, int& Map_Width, int& Map_Height)
{
	FILE * Exported_file;

	errno_t error = fopen_s(&Exported_file, filename, "r");

	if (error)
		{
			return false;
		};
	
	fscanf_s(Exported_file, "%*s %d", &Map_Width); 
	fscanf_s(Exported_file, "%*s %d", &Map_Height); //continue the scan for the height 
	//Allocate sufficient memory for the map 2d array
	MapData = new int*[Map_Height];
	for (int i = 0; i < Map_Height; ++i)
		{
			MapData[i] = new int [Map_Width];
		}

	for (int y = 0; y < Map_Height; ++y)
		{
			for (int x = 0; x < Map_Width; ++x)
				{
					fscanf_s(Exported_file, "%d", &MapData[y][x]);
				}
		}

	return true;
}

void SnapToCell(float *Coordinate)
{
	int i = (int)*Coordinate;

	*Coordinate = (float)i + 0.5f;
}

void PrintRetrievedInformation(int** MapData, int Map_Width, int Map_Height) //for checking Import function 
{
	int x; //x_coordinate counter 
	int y; //y_coordinate counter 

		   //Print a title 
	printf("Map Data\n");
	// Check Map Width 
	printf("Width : %d\n", Map_Width);
	//Check Map Height
	printf("Height : %d\n", Map_Height);

	//Use a nested for loop to print out the map data 
	for (y = 0; y < Map_Height; ++y)
	{
		for (x = 0; x < Map_Width; ++x)
		{
			printf("%d ", MapData[y][x]);
		}
		printf("\n");
	}
}

float Convert_X(float& x)
{
	float global_x = x* 80 -640.0f;
	return global_x;
}

 float Convert_Y(float& y)
 {
	 float global_y = 360.0f - y* 90;
	 
	 return global_y;
 }
 /****************************************************************************************
 Description:
 This functionis an example provided on how to initialize the Game World to construct the
 different game Objects 
 ****************************************************************************************/
 //void Construct_ALL() // call in Init
 //{
	// 
	// for (int y = 0; y < Map_Height; ++y)
	// {
	//	 for (int x = 0; x < Map_Width; ++x)
	//	 {
	//		 //--------------------------------construct platform objects------------------------------------
	//		 if (MapData[y][x] == OBJ_PLATFORM)
	//		 {
	//			 float f_x = (float)x;
	//			 float f_y = (float)y;
	//			 platforms.push_back(Platform{ Convert_X(f_x) , Convert_Y(f_y) });
	//		 }
	//		 //--------------------------------construct floor objects------------------------------------
	//		 if (MapData[y][x] == OBJ_FLOOR)
	//		 {
	//			 float f_x = (float)x;
	//			 float f_y = (float)y;
	//			 floors.push_back( Floor { Convert_X(f_x) , Convert_Y(f_y) });
	//		 }
	//		 //--------------------------------construct enemy objects------------------------------------
	//		 if (MapData[y][x] == OBJ_GRUNT)
	//		 {
	//			 float f_x = (float)x;
	//			 float f_y = (float)y;
	//			 enemies.push_back(Grunt{ Convert_X(f_x) , Convert_Y(f_y) });
	//		 }
	//		 //--------------------------------construct scarecrow objects-------------------------------
	//		 if (MapData[y][x] == OBJ_SC)
	//		 {
	//			 float f_x = (float)x;
	//			 float f_y = (float)y;
	//			 scarecrows.push_back(Scarecrow { Convert_X(f_x) , Convert_Y(f_y) });
	//		 }
	//	 }
	// }
 //}

