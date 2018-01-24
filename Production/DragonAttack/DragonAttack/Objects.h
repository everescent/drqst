#pragma once
#include "AEEngine.h"

#define FLOOR_WIDTH 640.0f
#define FLOOR_HEIGHT 100.0f
#define PLAT_WIDTH 64.0f
#define PLAT_HEIGHT 20.0f

class Object {

public:
  void SetPosition(float posX, float posY);
  void SetTex(const char* TexFile, float posU, float posV);
  float posX() const { return X; };
  float posY() const { return Y; };
  float posU() const { return U; };
  float posV() const { return V; };
  AEGfxVertexList* mesh() const { return Mesh; };
  AEGfxRenderMode rm() const { return RM; };
  AEGfxTexture* tex() const { return Tex; };
  Object(AEGfxVertexList * mesh = nullptr)
  {
    Mesh = mesh;
    Tex = nullptr;
    RM = AE_GFX_RM_COLOR;
    X = 0.0f;
    Y = 0.0f;
    U = 0.0f;
    V = 0.0f;
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

AEGfxVertexList* CreateSquare(float size, unsigned int color);

AEGfxVertexList* CreateRectangle(float width, float height, unsigned int color);

AEGfxVertexList* CreateFloor(float Multiplier = 1.0f, unsigned int color = 0);

AEGfxVertexList* CreatePlatform(float MultiplierW = 1.0f, float MultiplierH = 1.0f, unsigned int color = 0);

void RenderObject(const Object &pMesh, float R = 1.0f, float G = 1.0f, float B = 1.0f, float A = 1.0f);