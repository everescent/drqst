/* Start Header ************************************************************************/
/*!
\file       Dragon.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Dragon class member function definitions here.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Dragon.h"

void Dragon::ApplyPowerUP()
{

}

void Dragon::Input()
{
  if (AEInputCheckCurr(AEVK_A))
    Dir.L = true;
  if (AEInputCheckCurr(AEVK_D))
    Dir.R = true;
  if (AEInputCheckCurr(AEVK_SPACE))
    if(PosY == 0.0f)
      Dir.UP = true;
  if (AEInputCheckCurr(AEVK_RETURN))
    Attack = true;
}

void Dragon::Update()
{
  Input();
  if (Dir.L)
  {
    PosX -= Speed;
    Transform_.SetScale(1.0f, 1.0f).SetTranslate(PosX, PosY);
  }
  if (Dir.R)
  {
    PosX += Speed;
    Transform_.SetScale(-1.0f, 1.0f).SetTranslate(PosX, PosY);
  }
  if (Dir.UP)
    PosY += 20.0f;
  Dir.L = Dir.R = false;
  PosY -= Gravity;
  if (PosY <= 0.0f)
    PosY = 0.0f;
  if (PosY >= 150.0f)
    Dir.UP = false;
}
