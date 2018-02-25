#include "Animation.h"



Animation::Animation(unsigned stateNum, void(*InitState)())
  :Complete{ false }, Animation_State{}
{
  Animation_State.reserve(stateNum);
  *InitState;
}

void Animation::Update(const int& state)
{

}

void Animation::SetComplete(const bool& Complete)
{
  this->Complete = Complete;
}

Animation::~Animation()
{
  //Clear vector memory
  Animation_State.clear();
}
