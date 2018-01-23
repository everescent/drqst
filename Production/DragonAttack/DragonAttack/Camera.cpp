#include "Camera.h"
#include "AEEngine.h"

void SetPlayerCam(int PositionX, int PositionY, int Buffer, int Offset)
{
  AEGfxSetCamPosition(PositionX + Offset, PositionY + Offset);
}