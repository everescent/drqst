/* Start Header ************************************************************************/
/*!
\file       Animation.h
\author     Jacob Lim
\par email: jacob.lim\@digipen.edu
\brief
  Animation class declared here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include <AEEngine.h>
#include <functional>
#include <vector>
#include "Sprite.h"

struct Range {
  //Constructs Range with start and end range for row and column
  Range(const float stX, const float edX, const float stY, const float edY)
  :startRow{ stY }, endRow{ edY }, startCol{ stX }, endCol{ edX }, 
   Rowcurr{ startRow }, Colcurr{ startCol }, Complete{ true }
  {}
  const float startRow; //Animation start frame
  const float endRow;   //Animation end frame
  const float startCol; //Animation start frame
  const float endCol;   //Animation end frame
  float       Rowcurr;  //Current row
  float       Colcurr;  //Current colum
  bool        Complete; //Checks if animation cycle is complete
};

class Animation
{
public:
  //Initializes the Animation module
  //Takes in the required number of states, and a function to initialize it
  Animation(unsigned stateNum, 
            const float row, const float col,
            //Const function to take in lambda functions
            const std::function <void (std::vector <Range>&)>& Init);
  //Copy assignment
  Animation(const Animation& rhs);
  //No assignment operator, have const members
  //Updates the animation frame
  void Update(Sprite &t_Sprite);
  //Get Complete status
  bool GetComplete(int state) const { return Animation_State[state].Complete; }
  //Reset state row and col current
  void ResetState(int t_state);
  //Set state to update
  void SetState(int t_state);
  //Clear Animation_State
  ~Animation();

private:
  float       Tex_Row;    //Number of rows
  float       Tex_Col;    //Number of columns
  const float offsetX;    //Value to increment column
  const float offsetY;    //Value to increment row
  int         State;      //Current state to update
  float       FrameCount; //Counts the number of frames so far, starts from frame 1
  //This holds different ranges to suit each sprite
  std::vector <Range> Animation_State;
};
