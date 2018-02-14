/* Start Header ************************************************************************/
/*!
\file       Create_Object.h
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Mesh creation functions are declared here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
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
scaleU : Texture scale in the U direction.
scaleV : Texture scale in the V direction.
color  : Mesh color.

Return:
A pointer to a mesh.
****************************************************************************************/
AEGfxVertexList* CreateSquare(float size, unsigned int color = 0xFFFFFF);

/****************************************************************************************
Description:
Creates a rectangle mesh.

Parameters:
width  : Width of the rectangle.
height : Height of the rectangle.
scaleU : Texture scale in the U direction.
scaleV : Texture scale in the V direction.
color  : Mesh color.

Return:
A pointer to a mesh.
****************************************************************************************/
AEGfxVertexList* CreateRectangle(float width, float height, 
                                 unsigned int color = 0xFFFFFF);

/****************************************************************************************
Description:
Creates a square mesh.

Parameters:
size    : The size of the square (its width).
scaleU  : Texture scale in the U direction.
scaleV  : Texture scale in the V direction.
texFile : Texture file, if nullptr - COLOR mode will be chosen.
color   : Mesh color.

Return:
Sprite object.
****************************************************************************************/
Sprite S_CreateSquare(float size, const char* texFile = nullptr, 
                      unsigned int color = 0xFFFFFF);

/****************************************************************************************
Description:
Creates a rectangle mesh.

Parameters:
width   : Width of the rectangle.
height  : Height of the rectangle.
scaleU  : Texture scale in the U direction.
scaleV  : Texture scale in the V direction.
texFile : Texture file, if nullptr - COLOR mode will be chosen.
color   : Mesh color.

Return:
Sprite object
****************************************************************************************/
Sprite S_CreateRectangle(float width, float height, const char* texFile = nullptr, 
                         unsigned int color = 0xFFFFFF);

/****************************************************************************************
Description:
Creates a platform mesh.

Parameters:
MultW   : Width multiplier.
MultH   : Height multiplier.
scaleU  : Texture scale in the U direction.
scaleV  : Texture scale in the V direction.
texFile : Texture file, if nullptr - COLOR mode will be chosen.
color   : Mesh color.

Return:
Sprite object
****************************************************************************************/
Sprite CreatePlatform(float MultW = 1.0f, float MultH = 1.0f,
                      const char* texFile = nullptr, 
                      unsigned int color = 0xFFFFFF);

/****************************************************************************************
Description:
Creates a floor mesh.

Parameters:
MultW   : Width multiplier.
scaleU  : Texture scale in the U direction.
scaleV  : Texture scale in the V direction.
texFile : Texture file, if nullptr - COLOR mode will be chosen.
color   : Mesh color.

Return:
Sprite object
****************************************************************************************/
Sprite CreateFloor(float MultW = 1.0f, const char* texFile = nullptr,
                   unsigned int color = 0xFFFFFF);

/****************************************************************************************
Description:
Creates a BG mesh.

Parameters:
MultW   : Width multiplier.
scaleU  : Texture scale in the U direction.
scaleV  : Texture scale in the V direction.
texFile : Texture file, if nullptr - COLOR mode will be chosen.
color   : Mesh color.

Return:
Sprite object
****************************************************************************************/
Sprite CreateBG(float MultW = 1.0f, const char* texFile = nullptr,
                unsigned int color = 0xFFFFFF);
