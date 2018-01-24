#pragma once
#include "AEEngine.h"

#define FLOOR_WIDTH 640.0f
#define FLOOR_HEIGHT 100.0f
#define PLAT_WIDTH 64.0f
#define PLAT_HEIGHT 20.0f

class Object {

public:
	void SetMesh(AEGfxVertexList *pMesh);
	void SetPosition(float posX, float posY);
	void SetTex(AEGfxTexture *pTex, float posU, float posV);
	float posX() { return X; };
	float posY() { return Y; };
	float posU() { return U; };
	float posV() { return V; };
	AEGfxVertexList* mesh() { return Mesh; };
	AEGfxRenderMode rm() { return RM; };
	AEGfxTexture* tex() { return Tex; };
private:
	AEGfxVertexList *Mesh = nullptr;
	AEGfxRenderMode RM = AE_GFX_RM_COLOR;
	AEGfxTexture *Tex = nullptr;
	float X = 0.0f;
	float Y = 0.0f;
	float U = 0.0f;
	float V = 0.0f;
};

AEGfxVertexList* CreateSquare(float size, unsigned int color);

AEGfxVertexList* CreateRectangle(float width, float height, unsigned int color);

AEGfxVertexList* CreateFloor(float Multiplier, unsigned int color);

AEGfxVertexList* CreatePlatform(float MultiplierW, float MultiplierH, unsigned int color);

void RenderObject(Object pMesh);