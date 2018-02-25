#pragma once
#include <AEEngine.h>
#include <functional>
#include <vector>

struct Range {
  Range(const float &stX, const float &edX, const float &stY, const float &edY)
  :startRow{ stX }, endRow{ edX }, startCol{ stY }, endCol{ edY }
  {}
  float startRow{ 0.0f }; //Animation start frame
  float endRow  { 0.0f }; //Animation end frame
  float startCol{ 0.0f }; //Animation start frame
  float endCol  { 0.0f }; //Animation end frame
};

class Animation
{
public:
  //Initializes the Animation module
  //Takes in the required number of states, and a function to initialize it
  Animation(unsigned stateNum, const float &width, const float &height, const float &row, const float &col,
            //Function wrapper to take in lambda functions
            const std::function <void (std::vector <Range>&)>& Init);
  void Update(AEGfxTexture *Texture, const int& state, const bool &reset);
  void SetComplete(const bool& Complete);
  bool GetComplete() const { return Complete; }
  ~Animation();

private:
  bool Complete; //Checks if animation cycle is complete
  float Tex_Width; //Texture width
  float Tex_Height;//Texture height
  float Tex_Row; //Number of rows
  float Tex_Col;//Number of columns
  std::vector <Range> Animation_State; //This holds different ranges to suit each sprite
};
