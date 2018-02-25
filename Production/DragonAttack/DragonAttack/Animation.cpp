/* Start Header ************************************************************************/
/*!
\file       Animation.cpp
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Animation class member functions defined here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Animation.h"
#include <iostream>

Animation::Animation(unsigned stateNum, const float &width, const float &height, const float &row, const float &col,
                     const std::function <void(std::vector <Range>&)>& Init)
  :Complete{ false }, Tex_Width{ width }, Tex_Height{ height }, Tex_Row{ row }, Tex_Col{ col },
   Animation_State {}
{
  Animation_State.reserve(stateNum);
  Init(Animation_State);
}

void Animation::Update(AEGfxTexture *Texture, const int& state, const bool &reset)
{
  static const float offsetX = Tex_Width / Tex_Col;
  static const float offsetY = Tex_Height / Tex_Row;
  static float startX = Animation_State[state].startRow;
  static float startY = Animation_State[state].startCol;
  //Check for state reset
  if (reset)
  {
    startX = Animation_State[state].startRow;
    startY = Animation_State[state].startCol;
  }
  //Check if texture state has reached the end column
  if (startX >= Animation_State[state].endCol)
  {
    //Reset startX position
    startX = Animation_State[state].startCol;
    //Increment the row
    startY += offsetY;
    //If both row and column reached the end
    if (startY >= Animation_State[state].endRow && 
        startX >= Animation_State[state].endCol)
      //Reset both, column reset should have been done above
      startY = Animation_State[state].startRow;
  }
  //Dont do anything at first frame
  if (startX == Animation_State[state].endCol);
  else
    //Increment the column
    startX += offsetX;
  //Set texture position
  AEGfxTextureSet(Texture, startX, startY);
}

void Animation::SetComplete(const bool& t_complete)
{
  Complete = t_complete;
}

Animation::~Animation()
{
  //Clear vector memory
  Animation_State.clear();
}
