#include "Camera.h"
#include "AEEngine.h"

void CamFollow(float PositionX, float PositionY, float Offset)
{
  AEGfxSetCamPosition(PositionX + Offset, PositionY + Offset);
}

void CamStatic(float PositionX, float PositionY)
{
  AEGfxSetCamPosition(PositionX, PositionY);
}
