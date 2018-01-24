#include <string>
#include "Objects.h"
#include "AEEngine.h"

AEGfxVertexList* CreateSquare(float size, unsigned int color, float scaleU, float scaleV)
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

AEGfxVertexList* CreateRectangle(float width, float height, unsigned int color, float scaleU, float scaleV)
{
  AEGfxMeshStart();
  AEGfxTriAdd(
    -width, -height, color, 0.0f, 1.0f * scaleV,
    width, -height, color, 1.0f * scaleU, 1.0f * scaleU,
    -width, height, color, 0.0f, 0.0f);
  AEGfxTriAdd(
    width, -height, color, 1.0f * scaleU, 1.0f * scaleV,
    width, height, color, 1.0f * scaleU, 0.0f,
    -width, height, color, 0.0f, 0.0f);

  return AEGfxMeshEnd();
}

AEGfxVertexList* CreateFloor(float Multiplier, unsigned int color)
{
  AEGfxMeshStart();
  AEGfxTriAdd(
    -FLOOR_WIDTH * Multiplier, -FLOOR_HEIGHT, color, 0.0f, 1.0f,
    FLOOR_WIDTH * Multiplier, -FLOOR_HEIGHT, color, 1.0f * Multiplier, 1.0f * Multiplier,
    -FLOOR_WIDTH * Multiplier, FLOOR_HEIGHT, color, 0.0f, 0.0f);
  AEGfxTriAdd(
    FLOOR_WIDTH * Multiplier, -FLOOR_HEIGHT, color, 1.0f * Multiplier, 1.0f,
    FLOOR_WIDTH * Multiplier, FLOOR_HEIGHT, color, 1.0f * Multiplier, 0.0f,
    -FLOOR_WIDTH * Multiplier, FLOOR_HEIGHT, color, 0.0f, 0.0f);

  return AEGfxMeshEnd();
}

AEGfxVertexList* CreatePlatform(float MultiplierW, float MultiplierH, unsigned int color)
{
  AEGfxMeshStart();
  AEGfxTriAdd(
    -PLAT_WIDTH * MultiplierW, -PLAT_HEIGHT * MultiplierH, color, 0.0f, 1.0f * MultiplierH,
    PLAT_WIDTH * MultiplierW, -PLAT_HEIGHT * MultiplierH, color, 1.0f * MultiplierW, 1.0f * MultiplierW,
    -PLAT_WIDTH * MultiplierW, PLAT_HEIGHT * MultiplierH, color, 0.0f, 0.0f);
  AEGfxTriAdd(
    PLAT_WIDTH * MultiplierW, -PLAT_HEIGHT * MultiplierH, color, 1.0f * MultiplierW, 1.0f * MultiplierH,
    PLAT_WIDTH * MultiplierW, PLAT_HEIGHT * MultiplierH, color, 1.0f * MultiplierW, 0.0f,
    -PLAT_WIDTH * MultiplierW, PLAT_HEIGHT * MultiplierH, color, 0.0f, 0.0f);

  return AEGfxMeshEnd();
}

void RenderObject(const Object &pMesh, float R, float G, float B, float A)
{
  AEGfxSetRenderMode(pMesh.rm());
  AEGfxSetPosition(pMesh.posX(), pMesh.posY());
  AEGfxTextureSet(pMesh.tex(), pMesh.posU(), pMesh.posV());
  AEGfxSetTintColor(R, G, B, A);
  AEGfxMeshDraw(pMesh.mesh(), AE_GFX_MDM_TRIANGLES);
}

void Object::SetPosition(float posX, float posY)
{
  X = posX;
  Y = posY;
}
void Object::SetTex(const char* TexFile, float posU, float posV)
{
  Tex = AEGfxTextureLoad(TexFile);
  AE_ASSERT_MESG(Tex, "Failed to load texture");
  U = posU;
  V = posV;
  RM = AE_GFX_RM_TEXTURE;
}