/* Start Header ************************************************************************/
/*!
\file       Objects.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Object class and mesh creation functions are declared here.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include <string>
#include "Objects.h"
#include "AEEngine.h"

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

//This function creates a floor
AEGfxVertexList* CreateFloor(float Multiplier, float MultiplierUV, unsigned int color)
{
  AEGfxMeshStart();
  //Transformation matrix is applied to undistort the Texture
  AEGfxTriAdd(
    -FLOOR_WIDTH * Multiplier, -FLOOR_HEIGHT, color, 0.0f, 1.0f,
    FLOOR_WIDTH * Multiplier, -FLOOR_HEIGHT, color, (FLOOR_WIDTH / FLOOR_HEIGHT) * MultiplierUV, 1.0f,
    -FLOOR_WIDTH * Multiplier, FLOOR_HEIGHT, color, 0.0f, 0.0f);
  AEGfxTriAdd(
    FLOOR_WIDTH * Multiplier, -FLOOR_HEIGHT, color, 1.0f, 1.0f,
    FLOOR_WIDTH * Multiplier, FLOOR_HEIGHT, color, 1.0f, 0.0f,
    -FLOOR_WIDTH * Multiplier, FLOOR_HEIGHT, color, -(FLOOR_WIDTH / FLOOR_HEIGHT) * MultiplierUV + 1, 0.0f);

  return AEGfxMeshEnd();
}

//This function creates a platform
AEGfxVertexList* CreatePlatform(float MultiplierW, float MultiplierH, float ScaleU, float ScaleV, unsigned int color)
{
  AEGfxMeshStart();
  //Transformation matrix is applied to undistort the Texture
  AEGfxTriAdd(
    -PLAT_WIDTH * MultiplierW, -PLAT_HEIGHT * MultiplierH, color, 0.0f, 1.0f,
    PLAT_WIDTH * MultiplierW, -PLAT_HEIGHT * MultiplierH, color, (PLAT_WIDTH / PLAT_HEIGHT) * ScaleU, 1.0f,
    -PLAT_WIDTH * MultiplierW, PLAT_HEIGHT * MultiplierH, color, 0.0f, -1.0f * ScaleV + 1);
  AEGfxTriAdd(
    PLAT_WIDTH * MultiplierW, -PLAT_HEIGHT * MultiplierH, color, 1.0f, 1.0f * ScaleV,
    PLAT_WIDTH * MultiplierW, PLAT_HEIGHT * MultiplierH, color, 1.0f, 0.0f,
    -PLAT_WIDTH * MultiplierW, PLAT_HEIGHT * MultiplierH, color, -(PLAT_WIDTH / PLAT_HEIGHT) * ScaleU + 1.0f, 0.0f);

  return AEGfxMeshEnd();
}

//This function creates a background
AEGfxVertexList* CreateBG(float Multiplier, float MultiplierUV, unsigned int color)
{
  AEGfxMeshStart();
  //Transformation matrix is applied to undistort the Texture
  AEGfxTriAdd(
    -BG_WIDTH * Multiplier, -BG_HEIGHT, color, 0.0f, 1.0f,
    BG_WIDTH * Multiplier, -BG_HEIGHT, color, (BG_WIDTH / BG_HEIGHT) * MultiplierUV, 1.0f,
    -BG_WIDTH * Multiplier, BG_HEIGHT, color, 0.0f, 0.0f);
  AEGfxTriAdd(
    BG_WIDTH * Multiplier, -BG_HEIGHT, color, 1.0f, 1.0f,
    BG_WIDTH * Multiplier, BG_HEIGHT, color, 1.0f, 0.0f,
    -BG_WIDTH * Multiplier, BG_HEIGHT, color, -(BG_WIDTH / BG_HEIGHT) * MultiplierUV + 1.0f, 0.0f);

  return AEGfxMeshEnd();
}

//This function renders an object
void RenderObject(const Object &pMesh, float R, float G, float B, float A)
{
  AEGfxSetRenderMode(pMesh.rm());
  AEGfxSetPosition(pMesh.posX(), pMesh.posY());
  AEGfxTextureSet(pMesh.tex(), pMesh.posU(), pMesh.posV());
  AEGfxSetTintColor(R, G, B, A);
  AEGfxMeshDraw(pMesh.mesh(), AE_GFX_MDM_TRIANGLES);
}

void Object::SetPosition(const float &posX, const float &posY)
{
  X = posX;
  Y = posY;
}
void Object::SetTexPos(const float &posU, const float &posV)
{
  U = posU;
  V = posV;
}
