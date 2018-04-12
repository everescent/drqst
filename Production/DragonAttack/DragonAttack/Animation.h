/* Start Header ************************************************************************/
/*!
\file    Animation.h
\project Dragon Attack
\author  Jacob Lim
\email   jacob.lim@digipen.edu
\brief
  Animation class declared here. 

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#pragma once
#include <AEEngine.h>
#include <functional> //function
#include <vector>     //vector
#include "Sprite.h"

/****************************************************************************************
Description:
  Range holds the end and start of an animation sequence.
  It also takes care of its current frame and whether or not the animation cycle is
  complete. 
****************************************************************************************/
struct Range {
  /**************************************************************************************
  Description:
    Constructs Range with start and end range for row and column.
  stX:
    Start column of animation cycle. 
  edX:
    End column of the animation cycle. 
  stY:
    Start row of the animation cycle. 
  edY:
    End row of the animation cycle. 
  **************************************************************************************/
  Range(const float stX, const float     edX, const float  stY, const float edY)
  :startRow{ stY      }, endRow { edY      }, startCol{ stX  }, endCol{ edX    },
   Rowcurr { startRow }, Colcurr{ startCol }, Complete{ true }
  {}
  const float startRow; //Animation start frame
  const float endRow  ; //Animation end frame
  const float startCol; //Animation start frame
  const float endCol  ; //Animation end frame
  float       Rowcurr ; //Current row
  float       Colcurr ; //Current colum
  bool        Complete; //Checks if animation cycle is complete
};

/****************************************************************************************
Description:
  Animation takes care of the animation, given a sprite sheet/atlas. 
****************************************************************************************/
class Animation
{
public:
  /**************************************************************************************
  Description:
    Initializes the Animation module. 
    Takes in the required number of states, and a function to initialize it.
  stateNum: 
    The number of animation states. 
  row: 
    Number of rows the spritesheet has. 
  col:
    Number of columns the spritesheet has. 
  Init:
    The function to initialize the vector of Ranges for each animation state. 
  **************************************************************************************/
  Animation (unsigned stateNum, const float row, const float col,
             const std::function <void (std::vector <Range>&)>& Init);
  /**************************************************************************************
  Description:
    Copy constructor.
  rhs:
    The Animation module to copy from. 
  **************************************************************************************/
  Animation (const Animation& rhs );
  /**************************************************************************************
  Description:
    Clear Animation_State. 
  **************************************************************************************/                     
  ~Animation(                     );
  /**************************************************************************************
  Description:
    Updates the animation frame. 
  t_Sprite:
    The sprite to take the texture from. 
  **************************************************************************************/
  void Update    (Sprite &t_Sprite);
  /**************************************************************************************
  Description:
    Reset state row and col current.
  t_state:
    The state to reset.
  **************************************************************************************/
  void ResetState(int     t_state );
  /**************************************************************************************
  Description:
    Set state to update.
  t_state:
    The state to update.
  **************************************************************************************/                     
  void SetState  (int     t_state );
  /**************************************************************************************
  Description:
    Get Complete status. 
  state:
    The state to check if animation cycle is complete. 
  **************************************************************************************/
  bool GetComplete(int state) const { return Animation_State[state].Complete; }

private:
  float       Tex_Row   ; //Number of rows
  float       Tex_Col   ; //Number of columns
  const float offsetX   ; //Value to increment column
  const float offsetY   ; //Value to increment row
  int         State     ; //Current state to update
  float       FrameCount; //Timer to achieve a 5FPS animation
  /*This holds different ranges to suit each sprite*/
  std::vector <Range> Animation_State;
};
