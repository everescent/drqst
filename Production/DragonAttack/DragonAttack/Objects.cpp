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

AEGfxVertexList* CreateFloor(float Multiplier, unsigned int color)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-FLOOR_WIDTH * Multiplier, -FLOOR_HEIGHT * Multiplier, color, 0.0f, 0.0f,
		FLOOR_WIDTH * Multiplier, -FLOOR_HEIGHT * Multiplier, color, 0.0f, 0.0f,
		-FLOOR_WIDTH * Multiplier, FLOOR_HEIGHT * Multiplier, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		FLOOR_WIDTH * Multiplier, -FLOOR_HEIGHT * Multiplier, color, 0.0f, 0.0f,
		FLOOR_WIDTH * Multiplier, FLOOR_HEIGHT * Multiplier, color, 0.0f, 0.0f,
		-FLOOR_WIDTH * Multiplier, FLOOR_HEIGHT * Multiplier, color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

AEGfxVertexList* CreatePlatform(float MultiplierW, float MultiplierH, unsigned int color)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-PLAT_WIDTH * MultiplierW, -PLAT_HEIGHT * MultiplierH, color, 0.0f, 0.0f,
		PLAT_WIDTH * MultiplierW, -PLAT_HEIGHT * MultiplierH, color, 0.0f, 0.0f,
		-PLAT_WIDTH * MultiplierW, PLAT_HEIGHT * MultiplierH, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		PLAT_WIDTH * MultiplierW, -PLAT_HEIGHT * MultiplierH, color, 0.0f, 0.0f,
		PLAT_WIDTH * MultiplierW, PLAT_HEIGHT * MultiplierH, color, 0.0f, 0.0f,
		-PLAT_WIDTH * MultiplierW, PLAT_HEIGHT * MultiplierH, color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

void RenderObject(Object pMesh)
{
	AEGfxSetRenderMode(pMesh.rm());
	AEGfxSetPosition(pMesh.posX(), pMesh.posY());
	AEGfxTextureSet(pMesh.tex(), pMesh.posU(), pMesh.posV());
	AEGfxMeshDraw(pMesh.mesh(), AE_GFX_MDM_TRIANGLES);
}

void Object::SetMesh(AEGfxVertexList *pMesh)
{
	Mesh = pMesh;
}
void Object::SetPosition(float posX, float posY)
{
	X = posX;
	Y = posY;
}
void Object::SetTex(AEGfxTexture *pTex, float posU, float posV)
{
	Tex = pTex;
	U = posU;
	V = posV;
}