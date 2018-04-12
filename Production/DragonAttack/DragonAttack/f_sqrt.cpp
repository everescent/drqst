/* Start Header ************************************************************************/
/*!
\file    f_sqrt.cpp
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  A faster square root function.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "f_sqrt.h"

//Using babylonian method of square root
float f_sqrt(const float x)
{
  //Throw anything less than 0
  assert(x >= 0.0f);
  //Return zero is value is small
  if (x <= FLT_EPSILON)
  {
    return 0.0f;
  }
  float result = x       ;
  float tmp    = x * 0.5f;
  //Averages the results of previous and current iteration
  for (char i = 0; i < 5; ++i)
  {
    result = (tmp + (x / tmp)) * 0.5f;
    tmp    = result                  ;
  }
  return result;
}
