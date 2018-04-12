/* Start Header ************************************************************************/
/*!
\file       Load_Screen.cpp
\project    Dragon Attack
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Cpp file for Load screen

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Load_Screen.h"    // header file
#include "AEEngine.h"       // AEGfxSetCamPosition, AEGfxSetBackgroundColor, AEGfxCreateFont, AEGfxDestroyFont
#include "StageManager.h"   // set_Next, set_After_Score


// for global variables
namespace
{
  // get the size of array
  #define SIZE(x)  (sizeof(x) / sizeof((x)[0]))
  
  u32 fontID;  // for the font
  float timer; // timer to display loading message
}

namespace Load_Screen
{
  /**************************************************************************************
  //
  // Loads the variables that are needed for the loading screen
  //
  **************************************************************************************/
  void Load(void)
  {
    // reset camera position
    AEGfxSetCamPosition(0.0f, 0.0f);

    // set background color
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    // set the font
    fontID = AEGfxCreateFont("calibri", 32, true, false);
  }

  /**************************************************************************************
  //
  // Initialize the variables that are needed for the credit screen
  //
  **************************************************************************************/
  void Init(void)
  {
    // renew the timer
    timer = 3.0f;
  }

  /**************************************************************************************
  //
  // Update the variables that are needed for the credit screen
  //
  **************************************************************************************/
  void Update(float dt)
  {
    // reduce the timer by dt until it is less than 0, then move to the next stage
    timer > 0.f ? timer -= dt : SM::Set_Next(SM::Get_After_Score());
  }

  /**************************************************************************************
  //
  // Draw the variables that are needed for the credit screen
  //
  **************************************************************************************/
  void Draw(void)
  {
    // message to print
    char message[13];

    // print different message at different timing
    switch (static_cast<int>(timer))
    {
    case 2:   strcpy_s(message, SIZE(message), "Loading.");
      break;
    case 1:   strcpy_s(message, SIZE(message), "Loading..");
      break;
    case 0:   strcpy_s(message, SIZE(message), "Loading...");
      break;
    default: break;
    }

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);                   // render with color
    AEGfxTextureSet(NULL, 0, 0);                           // no texture needed
    AEGfxSetTransparency(1.0f);                            // opaque
    AEGfxPrint(fontID, message, -90, 0, 1.0f, 1.0f, 1.0f); // print message on screen
  }

  /**************************************************************************************
  //
  // does nothing
  //
  **************************************************************************************/
  void Free(void)
  {
    // does nothing
  }

  /**************************************************************************************
  //
  // Unload the variables that are needed for the credit screen
  //
  **************************************************************************************/
  void Unload(void)
  {
    // Destroy font
    AEGfxDestroyFont(fontID);
  }
}