/* Start Header ************************************************************************/
/*!
\file       f_sqrt.h
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  A faster square root function.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include <cfloat>  //FLT_EPSILON
#include <cassert> //assert

//Faster square root
float f_sqrt(const float &x);
