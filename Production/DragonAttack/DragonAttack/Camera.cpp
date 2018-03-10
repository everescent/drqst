/* Start Header ************************************************************************/
/*!
\file       Camera.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Camera functions definition.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Camera.h"
#include "Transform.h"
#include "AEEngine.h"

//This function follows an object with an offset
void CamFollow(float PositionX, float PositionY, float OffsetX, float OffsetY)
{
  AEGfxSetCamPosition(PositionX + OffsetX, PositionY + OffsetY);
}

//This function follows an object using its transform matrix, with an offset
void CamFollow(Transform const &PosMatrix, float OffsetX, float OffsetY, float Direction)
{
  //Moves the camera left or right
  static float Offset = OffsetX;
  static bool UP = false;
  static bool DOWN = false;
  static float Cam_posY = PosMatrix.GetTranslateMatrix().m[1][2] + OffsetY;
  static float New_Y = PosMatrix.GetTranslateMatrix().m[1][2] + OffsetY;
  //To use a smooth animation curve
  float Accel = 2.0f;
  if (PosMatrix.GetTranslateMatrix().m[1][2] + OffsetY > Cam_posY + 200.0f)
  {
    New_Y = PosMatrix.GetTranslateMatrix().m[1][2] + OffsetY;
    UP = true;
  }
  else if (PosMatrix.GetTranslateMatrix().m[1][2] + OffsetY < Cam_posY - 200.0f)
  {
    New_Y = PosMatrix.GetTranslateMatrix().m[1][2] + OffsetY;
    DOWN = true;
  }
  if (UP)
  {
    Cam_posY += Accel;
    if (Cam_posY >= New_Y)
    {
      Cam_posY = New_Y;
      UP = false;
    }
  }
  else if (DOWN)
  {
    Cam_posY -= Accel;
    if (Cam_posY <= New_Y)
    {
      Cam_posY = New_Y;
      DOWN = false;
    }
  }
  if (Direction < 0.0f)
  {
    if (Offset <= -OffsetX)
      Offset = -OffsetX;
    AEGfxSetCamPosition(PosMatrix.GetTranslateMatrix().m[0][2] + Offset,
                        Cam_posY);
    Accel *= Accel;
    Offset += -Accel;
  }
  else
  {
    if (Offset >= OffsetX)
      Offset = OffsetX;
      AEGfxSetCamPosition(PosMatrix.GetTranslateMatrix().m[0][2] + Offset,
        PosMatrix.GetTranslateMatrix().m[1][2] + OffsetY);
      Accel *= Accel;
      Offset += Accel;
  }
}

//This function sets the camera position
void CamStatic(float PositionX, float PositionY)
{
  AEGfxSetCamPosition(PositionX, PositionY);
}
