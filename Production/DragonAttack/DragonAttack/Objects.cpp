#include "Objects.h"
#include "AEEngine.h"

AEGfxVertexList* CreateSquare(float size, unsigned int color)
{
  AEGfxMeshStart();
  AEGfxTriAdd(
    -size, -size, color, 0.0f, 0.0f,
    size, -size, color, 0.0f, 0.0f,
    -size, size, color, 0.0f, 0.0f);
  AEGfxTriAdd(
    size, -size, color, 0.0f, 0.0f,
    size, size, color, 0.0f, 0.0f,
    -size, size, color, 0.0f, 0.0f);

  return AEGfxMeshEnd();
}

AEGfxVertexList* CreateRectangle(float width, float height, unsigned int color)
{
  AEGfxMeshStart();
  AEGfxTriAdd(
    -width, -height, color, 0.0f, 0.0f,
    width, -height, color, 0.0f, 0.0f,
    -width, height, color, 0.0f, 0.0f);
  AEGfxTriAdd(
    width, -height, color, 0.0f, 0.0f,
    width, height, color, 0.0f, 0.0f,
    -width, height, color, 0.0f, 0.0f);

  return AEGfxMeshEnd();
}

