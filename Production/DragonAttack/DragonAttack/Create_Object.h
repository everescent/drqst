/* Start Header ************************************************************************/
/*!
\file    Create_Object.h
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Mesh creation functions are declared here.

All content � 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include "Sprite.h"

/****************************************************************************************
Description:
Creates a square mesh.

Parameters:
size   : The size of the square (its width).
scaleU : UV scale horizontal.
scaleV : UV scale vertical.
color  : Mesh color.

Return:
A pointer to a mesh.
****************************************************************************************/
AEGfxVertexList* CreateSquare(float size, float scaleU = 1.0f, float scaleV = 1.0f, 
                              unsigned int color = 0xFFFFFFFF);

/****************************************************************************************
Description:
Creates a rectangle mesh.

Parameters:
width  : Width of the rectangle.
height : Height of the rectangle.
scaleU : UV scale horizontal.
scaleV : UV scale vertical.
color  : Mesh color.

Return:
A pointer to a mesh.
****************************************************************************************/
AEGfxVertexList* CreateRectangle(float width, float height, 
                                 float scaleU = 1.0f, float scaleV = 1.0f,
                                 unsigned int color = 0xFFFFFFFF);

/****************************************************************************************
Description:
Creates a square mesh.

Parameters:
size    : The size of the square (its width).
texFile : Texture file, if nullptr - COLOR mode will be chosen.
scaleU  : UV scale horizontal.
scaleV  : UV scale vertical.
color   : Mesh color.

Return:
Sprite object.
****************************************************************************************/
Sprite S_CreateSquare(float size, const char* texFile = nullptr, 
                      float scaleU = 1.0f, float scaleV = 1.0f,
                      unsigned int color = 0xFFFFFFFF);

/****************************************************************************************
Description:
Creates a rectangle mesh.

Parameters:
width   : Width of the rectangle.
height  : Height of the rectangle.
texFile : Texture file, if nullptr - COLOR mode will be chosen.
scaleU  : UV scale horizontal.
scaleV  : UV scale vertical.
color   : Mesh color.

Return:
Sprite object
****************************************************************************************/
Sprite S_CreateRectangle(float width, float height, const char* texFile = nullptr, 
                         float scaleU = 1.0f, float scaleV = 1.0f,
                         unsigned int color = 0xFFFFFFFF);

/****************************************************************************************
Description:
Creates a platform mesh.

Parameters:
MultW   : Width multiplier.
MultH   : Height multiplier.
texFile : Texture file, if nullptr - COLOR mode will be chosen.
scaleU  : UV scale horizontal.
scaleV  : UV scale vertical.
color   : Mesh color.

Return:
Sprite object
****************************************************************************************/
Sprite CreatePlatform(float MultW = 1.0f, float MultH = 1.0f,
                      const char* texFile = nullptr, 
                      float scaleU = 1.0f, float scaleV = 1.0f,
                      unsigned int color = 0xFFFFFFFF);

/****************************************************************************************
Description:
Creates a floor mesh.

Parameters:
MultW   : Width multiplier.
texFile : Texture file, if nullptr - COLOR mode will be chosen.
scaleU  : UV scale horizontal.
scaleV  : UV scale vertical.
color   : Mesh color.

Return:
Sprite object
****************************************************************************************/
Sprite CreateFloor(float MultW = 1.0f, const char* texFile = nullptr, 
                   float scaleU = 1.0f, float scaleV = 1.0f,
                   unsigned int color = 0xFFFFFFFF);

/****************************************************************************************
Description:
Creates a BG mesh.

Parameters:
MultW   : Width multiplier.
texFile : Texture file, if nullptr - COLOR mode will be chosen.
scaleU  : UV scale horizontal.
scaleV  : UV scale vertical.
color   : Mesh color.

Return:
Sprite object
****************************************************************************************/
Sprite CreateBG(float MultW = 1.0f, float MultH = 1.0f, const char* texFile = nullptr, 
                float scaleU = 1.0f, float scaleV = 1.0f,
                unsigned int color = 0xFFFFFFFF);
