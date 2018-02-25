#pragma once
#include <AEEngine.h>
#include <vector>

class Animation
{
public:
  Animation(unsigned stateNum, void(*InitState)());
  void Update(const int& state);
  void SetComplete(const bool& Complete);
  ~Animation();

private:
  bool Complete; //Checks if animation cycle is complete
  
  struct Range {
    float start{ 0.0f }; //Animation start frame
    float end  { 0.0f }; //Animation end frame
  };
  std::vector <Range> Animation_State; //This holds different ranges to suit each sprite
};

