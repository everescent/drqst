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

//Initializes the Animation module.
//Takes in the required number of states, and a function to initialize it.
Animation::Animation(unsigned stateNum, 
                     const float row, const float col,
                     const std::function <void(std::vector <Range>&)>& Init)
  :Tex_Row{ row            }, Tex_Col{ col            },
   //UV is 0.0f to 1.0f so get the inverse of the column and row
   //and increment by that
   offsetX{ 1.0f / Tex_Col }, offsetY{ 1.0f / Tex_Row },
   /*   Start at state 0  */
   State  { 0              }, 
   /*Update every 5 frames*/
   FrameCount{ (float)AEFrameRateControllerGetFrameTime() * 5.0f },
   Animation_State {}
{
  //Reserve the memory needed
  Animation_State.reserve(stateNum);
  //Intialize the Animation_States, by the user
  Init(Animation_State)            ;
}

//Copy constructor
Animation::Animation(const Animation& rhs)
  :Tex_Row{ rhs.Tex_Row    }, Tex_Col   { rhs.Tex_Col    },
   offsetX{ 1.0f / Tex_Col }, offsetY   { 1.0f / Tex_Row },
   State  { rhs.State      }, FrameCount{ rhs.FrameCount },
   Animation_State{ rhs.Animation_State }
{}

void Animation::ResetState(int t_state)
{
  //Reset animation cycle
  Animation_State[t_state].Rowcurr = Animation_State[t_state].startRow;
  Animation_State[t_state].Colcurr = Animation_State[t_state].startCol;
}

void Animation::SetState(int t_state)
{
  State = t_state                          ;
  Animation_State[t_state].Complete = false;
}

void Animation::Update(Sprite &t_Sprite)
{
  //Update every 5 frames
  if (FrameCount <= 0.0f)
  {
    //Check if texture state has reached the end column
    if (Animation_State[State].Colcurr >= Animation_State[State].endCol)
    {
      //Reset startX position
      Animation_State[State].Colcurr = Animation_State[State].startCol;
      //Increment the row
      Animation_State[State].Rowcurr += offsetY;
      //If both row and column reached the end
      if (Animation_State[State].Rowcurr >= Animation_State[State].endRow)
        //Reset both, column reset should have been done above
        Animation_State[State].Rowcurr = Animation_State[State].startRow;
    }
    //else if (Animation_State[State].startCol == Animation_State[State].endCol)
    //{
    //  if (Animation_State[State].Rowcurr != Animation_State[State].endRow &&
    //      Animation_State[State].Colcurr >= Animation_State[State].startCol + 1.0f)
    //  {
    //    //Reset startX position
    //    Animation_State[State].Colcurr = Animation_State[State].startCol;
    //    //Increment the row
    //    Animation_State[State].Rowcurr += offsetY;
    //  }
    //  if (Animation_State[State].Rowcurr >= Animation_State[State].endRow)
    //    //Reset both, column reset should have been done above
    //    Animation_State[State].Rowcurr = Animation_State[State].startRow;
    //}
    //Check if animation cycle is complete
    if (Animation_State[State].Rowcurr >= Animation_State[State].endRow &&
      Animation_State[State].Colcurr >= Animation_State[State].endCol)
      Animation_State[State].Complete = true;
    else
      Animation_State[State].Complete = false;
    //Increment the column
    Animation_State[State].Colcurr += offsetX;
    FrameCount = 0.083f;
  }
  //Set texture position first, so that first frame renders
  t_Sprite.SetTexPos(Animation_State[State].Colcurr, Animation_State[State].Rowcurr);
  //Increment frame counter
  FrameCount -= 0.016f;
}

Animation::~Animation()
{
  //Clear vector memory
  Animation_State.clear();
}
