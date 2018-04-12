/* Start Header ************************************************************************/
/*!
\file    Camera.cpp
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Camera functions definition.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Camera.h"
#include "Transform.h"
#include "AEEngine.h"
#include <cmath>

//This function follows an object with an offset
void CamFollow(float PositionX, float PositionY, float OffsetX, float OffsetY)
{
  //Set the camera position, with offset
  AEGfxSetCamPosition(PositionX + OffsetX, PositionY + OffsetY);
}

//This function follows an object using its transform matrix, with an offset
void CamFollow(Transform const &PosMatrix, float OffsetX, float OffsetY, float Direction, const bool Reset)
{
  //Moves the camera left or right
  static float Offset   = OffsetX                                             ;
  //Holds the previous Y poistion                                             
  static float PosYold  = PosMatrix.GetTranslateMatrix().m[1][2] + OffsetY    ;
  //Flag for camera to move up                                                
  static bool  GoUp     = false                                               ;
  //Flag for camera to move down                                              
  static bool  GoDown   = false                                               ;
  //Flag for whether camera is moving                                         
  static bool  Moving   = false                                               ;
  //Current position Y                                                        
  float        PosYCurr = PosMatrix.GetTranslateMatrix().m[1][2]              ;
  //To use a smooth animation curve                                           
  float        Accel    = 6.0f                                                ;
  PosYold = Reset ? PosMatrix.GetTranslateMatrix().m[1][2] + OffsetY : PosYold;
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

//Adds camera shake
void CamShake()
{
  //Get a random number from 0 to 359 degrees
  float phaseRot = AEDegToRad((float)(rand() % 360));
  //Offset camera position
  AEGfxSetCamPosition(cosf(phaseRot) * 7, sinf(phaseRot) * 7);
}
