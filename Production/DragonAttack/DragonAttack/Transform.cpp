/* Start Header ************************************************************************/
/*!
\file    Transform.cpp
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Transform class functions are defined here.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "AEEngine.h"
#include "Transform.h"

//Sets the translation matrix and applies it to the matrix
void Transform::SetTranslate(float posX, float posY)
{
  AEMtx33Trans(&T_Matrix, posX, posY);
}

//Sets the degrees to rotate and applies it to the matrix
void Transform::SetRotation(const float Deg)
{
  Degree = Deg;
  AEMtx33RotDeg(&R_Matrix, Degree);
}

//Sets the scale matrix and applies it to the matrix
void Transform::SetScale(float scaleX, float scaleY)
{
  AEMtx33Scale(&S_Matrix, scaleX, scaleY);
}
