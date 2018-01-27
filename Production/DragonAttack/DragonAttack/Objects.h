#pragma once
#include "AEEngine.h"

#define FLOOR_WIDTH 640
#define FLOOR_HEIGHT 100
#define PLAT_WIDTH 64
#define PLAT_HEIGHT 20

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
  float posX() const { return X; };
  float posY() const { return Y; };
  float posU() const { return U; };
  float posV() const { return V; };
  AEGfxVertexList* mesh() const { return Mesh; };
  AEGfxRenderMode rm() const { return RM; };
  AEGfxTexture* tex() const { return Tex; };
  //Default constructor; Sets everything to zero
  Object()
  :Mesh{ nullptr }, RM{ AE_GFX_RM_COLOR }, Tex{ nullptr }
  {
    X = 0.0f;
    Y = 0.0f;
    U = 0.0f;
    V = 0.0f;
  }
  //Construct with a mesh, but no texture
  Object(AEGfxVertexList * mesh)
  :Mesh{ mesh }, RM{AE_GFX_RM_COLOR}, Tex{nullptr}
  {
    AE_ASSERT_MESG(Mesh, "Failed to create mesh");
    X = 0.0f;
    Y = 0.0f;
    U = 0.0f;
    V = 0.0f;
  }
  //Construct with a mesh and texture
  Object(AEGfxVertexList * mesh, const char* TexFile)
  :Mesh{ mesh }, RM{ AE_GFX_RM_TEXTURE }, Tex{ AEGfxTextureLoad(TexFile) }
  {
    AE_ASSERT_MESG(Tex, "Failed to load texture");
    AE_ASSERT_MESG(Mesh, "Failed to create mesh");
    X = 0.0f;
    Y = 0.0f;
    U = 0.0f;
    V = 0.0f;
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
  AEGfxVertexList *Mesh;
  AEGfxRenderMode RM;
  AEGfxTexture *Tex;
  float X; 
  float Y;
  float U;
  float V;
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

/*************************************************************************
Description:
  Creates a floor mesh.

Parameters:
  Multiplier   : This value multiplies the length of the floor.
  MultiplierUV : This multiplier scales the texture.
  color        : Mesh color.

Return:
  A pointer to a mesh.
*************************************************************************/
AEGfxVertexList* CreateFloor(float Multiplier = 1.0f, float MultiplierUV = 1.0f, unsigned int color = 0xFFFFFF);

/*************************************************************************
Description:
  Creates a platform mesh.

Parameters:
  MultiplierW : This value multiplies the width of the platform.
  MultiplierH : This value multiplies the height of the platform.
  scaleU      : Texture scale in the U direction.
  scaleV      : Texture scale in the V direction.
  color       : Mesh color.

Return:
  A pointer to a mesh.
*************************************************************************/
AEGfxVertexList* CreatePlatform(float MultiplierW = 1.0f, float MultiplierH = 1.0f, float ScaleU = 1.0f, float ScaleV = 1.0f, unsigned int color = 0xFFFFFF);

/*************************************************************************
Description:
  Renders an object.

Parameters:
  pMesh : The object to be rendered.
  R     : Amount of red to render.
  G     : Amount of Green to render.
  B     : Amount of Blue to render.
  A     : Alpha transparency.

Return:
  void
*************************************************************************/
void RenderObject(const Object &pMesh, float R = 1.0f, float G = 1.0f, float B = 1.0f, float A = 1.0f);
