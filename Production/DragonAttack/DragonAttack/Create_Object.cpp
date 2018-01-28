/* Start Header ************************************************************************/
/*!
\file       Create_Object.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Mesh creation functions are defined here.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "AEEngine.h"
#include "Create_Object.h"

//This function creates a custom square
AEGfxVertexList* CreateSquare(float size, float scaleU, float scaleV, unsigned int color)
{
  AEGfxMeshStart();
  AEGfxTriAdd(
    -size, -size, color, 0.0f, 1.0f * scaleV,
    size, -size, color, 1.0f * scaleU, 1.0f * scaleU,
    -size, size, color, 0.0f, 0.0f);
  AEGfxTriAdd(
    size, -size, color, 1.0f * scaleU, 1.0f * scaleV,
    size, size, color, 1.0f * scaleU, 0.0f,
    -size, size, color, 0.0f, 0.0f);

  return AEGfxMeshEnd();
}

//This function creates a custom rectangle
AEGfxVertexList* CreateRectangle(float width, float height, float ScaleU, float ScaleV, unsigned int color)
{
  AEGfxMeshStart();
  AEGfxTriAdd(
    -width, -height, color, 0.0f, 1.0f,
    width, -height, color, (float)((int)width / (int)height) * ScaleU, 1.0f,
    -width, height, color, 0.0f, -1.0f * ScaleV + 1);
  AEGfxTriAdd(
    width, -height, color, 1.0f, 1.0f * ScaleV,
    width, height, color, 1.0f, 0.0f,
    -width, height, color, -(width / height) * ScaleU + 1, 0.0f);

  return AEGfxMeshEnd();
}
