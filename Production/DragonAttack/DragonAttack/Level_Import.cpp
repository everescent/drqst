/* Start Header ************************************************************************/
/*!
\file       Level_Import.cpp
\author     Javon Lee
\par email: xiongweijavon.lee\@digipen.edu
\brief

This file contains the functions to import 

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Level_Import.h"
#include "Platform.h"
#include "Grunt.h"
#include <vector>

namespace
{
	static int ** MapData;
	static int Map_Width;
	static int Map_Height;
	std::vector<Platform> platforms;  
	Dragon *player; // for updating platforms because i can't render it properly without calling the update function that requires a ptr to Dragon as a parameter

	//Platform ** P_array; 
	int count = 6;

	enum OBJ_TYPE
	{
		OBJ_TYPE_EMPTY, // 0
		OBJ_PLATFORM,   // 1
		OBJ_DRAGON,		// 2
		OBJ_GRUNT,		// 3
		OBJ_KING,		// 4
		OBJ_MERLIN,		// 5
		OBJ_LANCE,		// 6
	};

}

bool Import_MapData(char* filename)
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

void PrintRetrievedInformation(void) //for checking Import function 
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
			//If the x counter reaches the last column 
			if ((x + 1) % Map_Height == 0)
			{
				printf("\n");
			}
		}
	}
}

float Convert_X(float& x)
{
	float global_x = x - AEGfxGetWinMinX();
	std::cout <<" winMin:" << AEGfxGetWinMinX() << std::endl;
	std::cout << global_x << std::endl;
	return global_x;
}

 float Convert_Y(float& y)
 {
	 float global_y = y ;
	 global_y = AEGfxGetWinMaxY() - y;
	 std::cout << global_y << std::endl;
	 return global_y;
 }


namespace Level_Import
{
	void Load(void)
	{
		if (!Import_MapData("level_2.txt"))
		{
			AEGfxExit();
		}
	}	
	
	void Init(void)
	{
		player = new Dragon{};

		int counter = 0;
		for (int y = 0; y < Map_Height; ++y)
		{
			for (int x = 0; x < Map_Width; ++x)
			{
				if (MapData[y][x] == OBJ_PLATFORM)
				{
					//INSERT CODE HERE
					if (count >= 0)
					{
						float f_x = (float)x;
						float f_y = (float)y;
					counter++;
					platforms.push_back(Platform{ Convert_X(f_x) , Convert_Y(f_y) });
						count--;
					}
				}
			}
		}
		std::cout << counter << std::endl;
	}

	void Update(float dt)
	{
		for (Platform& elem : platforms)
		{
			elem.Update(*player, dt);
		}
	}

	void Draw(void)
	{
		for ( Platform& elem : platforms)
		{
			elem.Render();
		}
	}

	void Free(void)
	{

	}

	void Unload(void)
	{

	}
}
