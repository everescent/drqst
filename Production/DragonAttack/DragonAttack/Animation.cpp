/* Start Header ************************************************************************/
/*!
\file    Animation.cpp
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Animation class member functions defined here.

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Animation.h"

//The name is just too long. 
//It looks very disgusting to put in my member initializer list.
//So I am shortening the name here. 
auto& g_dt = AEFrameRateControllerGetFrameTime;

//Initializes the Animation module.
//Takes in the required number of states, and a function to initialize it.
Animation::Animation(unsigned stateNum, 
                     const float row, const float col,
                     const std::function <void(std::vector <Range>&)>& Init)
  :Tex_Row{ row            }, Tex_Col{ col                     },
   //UV is 0.0f to 1.0f so get the inverse of the column and row
   //and increment by that
   offsetX{ 1.0f / Tex_Col }, offsetY   { 1.0f / Tex_Row       },
   /*   Start at state 0  */  /*     Update every 5 frames    */
   State  { 0              }, FrameCount{ (float)g_dt() * 5.0f },
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

//Reset state row and col current
void Animation::ResetState(int t_state)
{
  //Reset animation cycle
  Animation_State[t_state].Rowcurr = Animation_State[t_state].startRow;
  Animation_State[t_state].Colcurr = Animation_State[t_state].startCol;
}

//Set state to update
void Animation::SetState(int t_state)
{
  //Set state to update
  State = t_state                          ;
  //Reset Complete
  Animation_State[t_state].Complete = false;
}

//Updates the animation frame
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
    #if 0
    else if (Animation_State[State].startCol == Animation_State[State].endCol)
    {
      if (Animation_State[State].Rowcurr != Animation_State[State].endRow &&
          Animation_State[State].Colcurr >= Animation_State[State].startCol + 1.0f)
      {
        //Reset startX position
        Animation_State[State].Colcurr = Animation_State[State].startCol;
        //Increment the row
        Animation_State[State].Rowcurr += offsetY;
      }
      if (Animation_State[State].Rowcurr >= Animation_State[State].endRow)
        //Reset both, column reset should have been done above
        Animation_State[State].Rowcurr = Animation_State[State].startRow;
    }
    #endif
    //Check if animation cycle is complete
    if (Animation_State[State].Rowcurr >= Animation_State[State].endRow &&
        Animation_State[State].Colcurr >= Animation_State[State].endCol    )
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

//Clear Animation_State
Animation::~Animation()
{
  //Clear vector memory
  Animation_State.clear();
}
