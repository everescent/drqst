#include "AEEngine.h"
#include "Objects.h"

#define R_WIDTH 1280
#define R_HEIGHT 720

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
  UNREFERENCED_PARAMETER(instanceH);
  UNREFERENCED_PARAMETER(prevInstanceH);
  UNREFERENCED_PARAMETER(command_line);
  UNREFERENCED_PARAMETER(show);

  AESysInit(instanceH, show, R_WIDTH, R_HEIGHT, true, 60, NULL);
  AESysSetWindowTitle("My New Demo!");
  AESysReset();
  //AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
  AESysFrameStart();

  AEGfxVertexList *pMesh1;
  pMesh1 = CreateRectangle(100.0f, 50.0f, 0);
  AEGfxSetRenderMode(AE_GFX_RM_COLOR);
  // Set position for object 1
  AEGfxSetPosition(0.0f, 0.0f);
  // No texture for object 1
  AEGfxTextureSet(NULL, 0, 0);
  // Drawing the mesh (list of triangles)
  AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);

  AESysFrameEnd();
  AESysExit();
  system("pause");
  return 0;
}
