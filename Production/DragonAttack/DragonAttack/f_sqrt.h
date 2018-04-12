/* Start Header ************************************************************************/
/*!
\file    f_sqrt.h
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  A faster square root function.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include <cfloat>  //FLT_EPSILON
#include <cassert> //assert

/**************************************************************************************
Description:
  Faster square root.
x:
  Value to square root.
**************************************************************************************/
float f_sqrt(const float x);
