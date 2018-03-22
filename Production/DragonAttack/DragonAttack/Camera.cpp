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
  static float PosYold = PosMatrix.GetTranslateMatrix().m[1][2] + OffsetY;
  static bool GoUp = false;
  static bool GoDown = false;
  static bool Moving = false;
  float PosYCurr = PosMatrix.GetTranslateMatrix().m[1][2];
  //To use a smooth animation curve
  float Accel = 2.0f;
  if (Direction < 0.0f)
  {
    if (PosYCurr - PosYold > OffsetY + 100.0f && !Moving && PosYCurr != PosYold)
    {
      GoUp = true;
      GoDown = false;
      Moving = true;
    }
    else if (PosYCurr - PosYold < -OffsetY - 100.0f && !Moving && PosYCurr != PosYold)
    {
      GoUp = false;
      GoDown = true;
      Moving = true;
    }
    if (Moving && GoUp)
    {
      PosYold += Accel;
    }
    else if (Moving && GoDown)
    {
      PosYold -= Accel;
    }
    if (Offset <= -OffsetX)
      Offset = -OffsetX;
    if (GoUp && PosYold >= PosYCurr + OffsetY)
    {
      PosYold = PosYCurr + OffsetY;
      GoUp = false;
      Moving = false;
    }
    else if (GoDown && PosYold <= PosYCurr + OffsetY)
    {
      PosYold = PosYCurr + OffsetY;
      GoDown = false;
      Moving = false;
    }
    AEGfxSetCamPosition(PosMatrix.GetTranslateMatrix().m[0][2] + Offset,
      PosYold);
    Accel *= Accel;
    Offset += -Accel;
  }
  else
  {
    if (PosYCurr - PosYold > OffsetY + 100.0f && !Moving && PosYCurr != PosYold)
    {
      GoUp = true;
      GoDown = false;
      Moving = true;
    }
    else if (PosYCurr - PosYold < -OffsetY - 100.0f && !Moving && PosYCurr != PosYold)
    {
      GoUp = false;
      GoDown = true;
      Moving = true;
    }
    if (Moving && GoUp)
    {
      PosYold += Accel;
    }
    else if (Moving && GoDown)
    {
      PosYold -= Accel;
    }
    if (Offset >= OffsetX)
      Offset = OffsetX;
    if (GoUp && PosYold >= PosYCurr + OffsetY)
    {
      PosYold = PosYCurr + OffsetY;
      GoUp = false;
      Moving = false;
    }
    else if (GoDown && PosYold <= PosYCurr + OffsetY)
    {
      PosYold = PosYCurr + OffsetY;
      GoDown = false;
      Moving = false;
    }
    AEGfxSetCamPosition(PosMatrix.GetTranslateMatrix().m[0][2] + Offset,
      PosYold);
    Accel *= Accel;
    Offset += Accel;
  }
}

//This function sets the camera position
void CamStatic(float PositionX, float PositionY)
{
  AEGfxSetCamPosition(PositionX, PositionY);
}
