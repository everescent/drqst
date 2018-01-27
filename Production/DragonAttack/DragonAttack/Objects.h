/* Start Header ************************************************************************/
/*!
\file       Objects.h
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

#pragma once
#include "AEEngine.h"

const float FLOOR_WIDTH { 640.0f }; //The width of the floor
const float FLOOR_HEIGHT{ 100.0f }; //The height of the floor
const float PLAT_WIDTH  { 64.0f  }; //The width of one platform
const float PLAT_HEIGHT { 20.0f  }; //The height of one platform
const float BG_WIDTH    { 640.0f }; //The width of one background
const float BG_HEIGHT   { 360.0f }; //The height of one background

/*************************************************************************
Description: 
  Creates an object.
Constructors:
  Object can be constructed with a mesh or with mesh and texture or none
Deconstructor:
  Object will free its resources when done
*************************************************************************/
class Object {

public:
  //Set object position
  void SetPosition(const float &posX, const float &posY);
  //Set texture position
  void SetTexPos(const float &posU, const float &posV);
  //Set tint RGB
  void SetRGB(const float &Red, const float &Green, const float &Blue);
  //Set tint Alpha
  void SetAlpha(const float &Alpha);
  //Set transparency value for rendering
  void SetTransparency(const float &Trans);
  //Set blend mode for rendering
  void SetBlendMode(AEGfxBlendMode BlendMode);
  //Renders the object
  void RenderObject();
  float Get_posX() const { return X; };
  float Get_posY() const { return Y; };
  float Get_posU() const { return U; };
  float Get_posV() const { return V; };
  float Get_Width() const { return Width; };
  float Get_Height() const { return Height; };
  AEGfxVertexList* Get_mesh() const { return Mesh; };
  AEGfxRenderMode Get_rm() const { return RM; };
  AEGfxTexture* Get_tex() const { return Tex; };
  //Default constructor; Sets everything to zero
  Object()
  :Mesh{ nullptr }, RM{ AE_GFX_RM_COLOR }, Tex{ nullptr }, BM{ AE_GFX_BM_NONE }
  {
    X = 0.0f;
    Y = 0.0f;
    U = 0.0f;
    V = 0.0f;
    R = 1.0f;
    G = 1.0f;
    B = 1.0f;
    A = 1.0f;
    Transparency = 1.0f;
    Width = 0.0f;
    Height = 0.0f;
  }
  //Construct with a mesh, but no texture
  Object(AEGfxVertexList * mesh, const float &ObjectW, const float &ObjectH)
  :Mesh{ mesh }, RM{AE_GFX_RM_COLOR}, Tex{nullptr}, BM{ AE_GFX_BM_NONE }
  {
    X = 0.0f;
    Y = 0.0f;
    U = 0.0f;
    V = 0.0f;
    R = 1.0f;
    G = 1.0f;
    B = 1.0f;
    A = 1.0f;
    Transparency = 1.0f;
    Width = ObjectW;
    Height = ObjectH;
  }
  //Construct with a mesh and texture
  Object(AEGfxVertexList * mesh, const char* TexFile, float ObjectW, const float &ObjectH)
  :Mesh{ mesh }, RM{ AE_GFX_RM_TEXTURE }, Tex{ AEGfxTextureLoad(TexFile) }, BM{ AE_GFX_BM_NONE }
  {
    X = 0.0f;
    Y = 0.0f;
    U = 0.0f;
    V = 0.0f;
    R = 1.0f;
    G = 1.0f;
    B = 1.0f;
    A = 1.0f;
    Transparency = 1.0f;
    Width = ObjectW;
    Height = ObjectH;
  }
  //Free all resources
  ~Object()
  {
    if (Tex != nullptr)
      AEGfxTextureUnload(Tex);
    if (Mesh != nullptr)
      AEGfxMeshFree(Mesh);
  }
private:
  AEGfxVertexList *Mesh; //A pointer to object's mesh
  AEGfxRenderMode RM;    //Render Mode
  AEGfxTexture *Tex;     //Texture
  AEGfxBlendMode BM;     //Blend Mode
  float X;               //Position X
  float Y;               //Position Y
  float U;               //Texture U
  float V;               //Texture V
  float R;               //Red tint value
  float G;               //Green tint value
  float B;               //Blue tint value
  float A;               //Alpha tint value
  float Transparency;    //Transparency value
  float Width;           //Width of object
  float Height;          //Height of object
};

/*************************************************************************
Description:
  Creates a square mesh.

Parameters:
  size   : The size of the square (its width).
  scaleU : Texture scale in the U direction.
  scaleV : Texture scale in the V direction.
  color  : Mesh color.

Return:
  A pointer to a mesh.
*************************************************************************/
AEGfxVertexList* CreateSquare(float size, float scaleU = 1.0f, float scaleV = 1.0f, unsigned int color = 0xFFFFFF);

/*************************************************************************
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
*************************************************************************/
AEGfxVertexList* CreateRectangle(float width, float height, float ScaleU = 1.0f, float ScaleV = 1.0f, unsigned int color = 0xFFFFFF);
