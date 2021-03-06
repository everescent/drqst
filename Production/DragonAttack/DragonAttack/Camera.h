/* Start Header ************************************************************************/
/*!
\file    Camera.h
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Camera functions declaration.

All content � 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include "Transform.h"

/*************************************************************************
Description:
  Make the camera follow an object. 

Parameters:
  PositionX : Set the camera's X position.
  PositionY : Set the camera's Y position.
  OffsetX   : Camera offset in the X direction.
  OffsetY   : Camera offset in the Y direction.

Return:
  void
*************************************************************************/
void CamFollow(float PositionX = 0.0f, float PositionY = 0.0f, 
               float   OffsetX = 0.0f, float   OffsetY = 0.0f  );

/*************************************************************************
Description:
  Make the camera follow an object.

Parameters:
  PosMatrix : Transform matrix to follow. 
  OffsetX   : Camera offset in the X direction.
  OffsetY   : Camera offset in the Y direction.
  dir       : Direction camera target is facing.
  Reset     : Resets the camera position. 
Return:
  void
*************************************************************************/
void CamFollow(Transform const &PosMatrix,
               float              OffsetX, float OffsetY, float dir,
               const bool Reset = false);

/*************************************************************************
Description:
  Set camera position with no offset. 

Parameters:
  PositionX : Set the camera's X position.
  PositionY : Set the camera's Y position.

Return:
  void
*************************************************************************/
void CamStatic(float PositionX = 0.0f, float PositionY = 0.0f);

/*************************************************************************
Description:
  Adds camera shake.

Return:
  void
*************************************************************************/
void CamShake();
