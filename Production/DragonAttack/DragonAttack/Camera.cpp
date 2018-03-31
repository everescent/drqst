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
  //Set the camera position, with offset
  AEGfxSetCamPosition(PositionX + OffsetX, PositionY + OffsetY);
}

//This function follows an object using its transform matrix, with an offset
void CamFollow(Transform const &PosMatrix, float OffsetX, float OffsetY, float Direction)
{
  //Moves the camera left or right
  static float Offset   = OffsetX                                         ;
  //Holds the previous Y poistion
  static float PosYold  = PosMatrix.GetTranslateMatrix().m[1][2] + OffsetY;
  //Flag for camera to move up
  static bool  GoUp     = false                                           ;
  //Flag for camera to move down
  static bool  GoDown   = false                                           ;
  //Flag for whether camera is moving
  static bool  Moving   = false                                           ;
  //Current position Y
  float        PosYCurr = PosMatrix.GetTranslateMatrix().m[1][2]          ;
  //To use a smooth animation curve
  float        Accel    = 4.0f                                            ;
  //Check direction
  if (Direction < 0.0f) //If facing left
  {
    //Check if camera needs to move up
    if (PosYCurr - PosYold > OffsetY + 100.0f && !Moving && PosYCurr != PosYold)
    {
      GoUp   = true ;
      GoDown = false;
      Moving = true ;
    }
    //Check if camera needs to move down
    else if (PosYCurr - PosYold < -OffsetY - 100.0f && !Moving && PosYCurr != PosYold)
    {
      GoUp   = false;
      GoDown = true ;
      Moving = true ;
    }
    //Move up if needed
    if (Moving && GoUp)
    {
      PosYold += Accel;
    }
    //Move down if needed
    else if (Moving && GoDown)
    {
      PosYold -= Accel;
    }
    //Stop moving
    if (Offset <= -OffsetX)
      Offset = -OffsetX;
    //Stop moving
    if (GoUp && PosYold >= PosYCurr + OffsetY)
    {
      PosYold = PosYCurr + OffsetY;
      GoUp    = false             ;
      Moving  = false             ;
    }
    //Stop moving
    else if (GoDown && PosYold <= PosYCurr + OffsetY)
    {
      PosYold = PosYCurr + OffsetY;
      GoDown  = false             ;
      Moving  = false             ;
    }
    //Set the final position
    AEGfxSetCamPosition(PosMatrix.GetTranslateMatrix().m[0][2] + Offset,
                        PosYold                                          );
    Offset += -Accel;
  }
  else //If facing right
  {
    //Check if camera needs to move up
    if (PosYCurr - PosYold > OffsetY + 100.0f && !Moving && PosYCurr != PosYold)
    {
      GoUp   = true ;
      GoDown = false;
      Moving = true ;
    }
    //Check if camera needs to move down
    else if (PosYCurr - PosYold < -OffsetY - 100.0f && !Moving && PosYCurr != PosYold)
    {
      GoUp   = false;
      GoDown = true ;
      Moving = true ;
    }
    //Move up if needed
    if (Moving && GoUp)
    {
      PosYold += Accel;
    }
    //Move down if needed
    else if (Moving && GoDown)
    {
      PosYold -= Accel;
    }
    //Stop moving
    if (Offset >= OffsetX)
      Offset = OffsetX;
    //Stop moving
    if (GoUp && PosYold >= PosYCurr + OffsetY)
    {
      PosYold = PosYCurr + OffsetY;
      GoUp    = false             ;
      Moving  = false             ;
    }
    //Stop moving
    else if (GoDown && PosYold <= PosYCurr + OffsetY)
    {
      PosYold = PosYCurr + OffsetY;
      GoDown  = false             ;
      Moving  = false             ;
    }
    //Set the final position
    AEGfxSetCamPosition(PosMatrix.GetTranslateMatrix().m[0][2] + Offset,
                        PosYold                                          );
    Offset += Accel;
  }
}

//This function sets the camera position
void CamStatic(float PositionX, float PositionY)
{
  //Set the camera position
  AEGfxSetCamPosition(PositionX, PositionY);
}
