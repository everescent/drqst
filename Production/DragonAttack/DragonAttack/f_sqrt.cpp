/* Start Header ************************************************************************/
/*!
\file       f_sqrt.cpp
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

#include "f_sqrt.h"

//Using babylonian method of square root
float f_sqrt(const float &x)
{
  //Throw anything less than 0
  assert(x >= 0.0f);
  //Return zero is value is small
  if (x <= FLT_EPSILON)
  {
    return 0.0f;
  }
  float result = x;
  float tmp = x / 2.0f;
  for (char i = 0; i < 5; ++i)
  {
    result = (tmp + (x / tmp)) / 2.0f;
    tmp = result;
  }
  return result;
}
