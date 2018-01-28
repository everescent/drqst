/* Start Header ************************************************************************/
/*!
\file       Objects.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Object class and mesh creation functions are defined here.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

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

//This function renders an object
void Object::RenderObject(AEMtx33 matrix) const
{
  AEGfxSetRenderMode(RM);
  AEGfxSetTransform(matrix.m);
  AEGfxTextureSet(Tex, U, V);
  AEGfxSetTintColor(R, G, B, A);
  AEGfxSetTransparency(Transparency);
  AEGfxSetBlendMode(BM);
  AEGfxMeshDraw(Mesh, AE_GFX_MDM_TRIANGLES);
}

//Set texture position
Object& Object::SetTexPos(const float &posU, const float &posV)
{
  U = posU;
  V = posV;
  return *this;
}
//Set tint RGB
Object& Object::SetRGB(const float &Red, const float &Green, const float &Blue)
{
  R = Red;
  G = Green;
  B = Blue;
  return *this;
}
//Set tint Alpha
Object& Object::SetAlpha(const float &Alpha)
{
  A = Alpha;
  return *this;
}
//Set transparency value for rendering
Object& Object::SetTransparency(const float &Trans)
{
  Transparency = Trans;
  return *this;
}
//Set blend mode for rendering
Object& Object::SetBlendMode(AEGfxBlendMode BlendMode)
{
  BM = BlendMode;
  return *this;
}
