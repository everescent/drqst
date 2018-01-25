#pragma once
#include "AEEngine.h"

#define FLOOR_WIDTH 640
#define FLOOR_HEIGHT 100
#define PLAT_WIDTH 64
#define PLAT_HEIGHT 20

class Object {

public:
  void SetPosition(const float &posX, const float &posY);
  void SetTexPos(const float &posU, const float &posV);
  float posX() const { return X; };
  float posY() const { return Y; };
  float posU() const { return U; };
  float posV() const { return V; };
  AEGfxVertexList* mesh() const { return Mesh; };
  AEGfxRenderMode rm() const { return RM; };
  AEGfxTexture* tex() const { return Tex; };
  Object()
  :Mesh{ nullptr }, RM{ AE_GFX_RM_COLOR }, Tex{ nullptr }
  {
    X = 0.0f;
    Y = 0.0f;
    U = 0.0f;
    V = 0.0f;
  }
  Object(AEGfxVertexList * mesh)
  :Mesh{ mesh }, RM{AE_GFX_RM_COLOR}, Tex{nullptr}
  {
    AE_ASSERT_MESG(Mesh, "Failed to create mesh");
    X = 0.0f;
    Y = 0.0f;
    U = 0.0f;
    V = 0.0f;
  }
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

AEGfxVertexList* CreateSquare(float size, float scaleU = 1.0f, float scaleV = 1.0f, unsigned int color = 0xFFFFFF);

AEGfxVertexList* CreateRectangle(int width, int height, float ScaleU = 1.0f, float ScaleV = 1.0f, unsigned int color = 0xFFFFFF);

AEGfxVertexList* CreateFloor(float Multiplier = 1.0f, float MultiplierUV = 1.0f, unsigned int color = 0xFFFFFF);

AEGfxVertexList* CreatePlatform(float MultiplierW = 1.0f, float MultiplierH = 1.0f, float ScaleU = 1.0f, float ScaleV = 1.0f, unsigned int color = 0xFFFFFF);

void RenderObject(const Object &pMesh, float R = 1.0f, float G = 1.0f, float B = 1.0f, float A = 1.0f);
