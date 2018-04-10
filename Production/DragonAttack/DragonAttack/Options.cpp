/* Start Header ************************************************************************/
/*!
\file       Options.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Options.h"
#include "Create_Object.h"
#include "Camera.h"
#include "Sprite.h"
#include "Transform.h"
#include "GameStateManager.h"
#include "GameObject.h"
#include "Audio_Engine.h"

// for global variables
namespace
{
#define CHECKBOX_W 20.0f
#define CHECKBOX_H 20.0f
    
    Sprite     BG;                                     // background texture
    Transform  BG_M;                                   // background matrix
    Sprite     checkbox_texture[2];                    // checkbox   
    u32        fontID;                                 // fontID
    Sprite     text[2];                                // stores the option text
    Transform  text_m[2];                              // transform matrix for the text
    bool       fullscreen;                             // check current state
    float      x, y;                                   // x and y coordinate of camera
    GameObject* checkbox[2];                           // for  check boxes
}

/**************************************************************************************
//
// Initialize the various variables used in the options menu
// CALLED ONLY ONCE AT THE START OF THE GAME
//
**************************************************************************************/
void Init_Options()
{
    // initialize the background and its transformation matrix
    BG = S_CreateRectangle(128.f, 72.0f, ".//Textures/Main_Menu_BG.png");
    BG_M.SetScale(5.f, 5.f);
    BG_M.Concat();

    // initializing the textures for check boxes 
    checkbox_texture[0] = S_CreateRectangle(20.0f, 20.0f, ".//Textures/TickBox_NoTick.png");
    checkbox_texture[1] = S_CreateRectangle(20.0f, 20.0f, ".//Textures/TickBox_Tick.png");
    checkbox_texture[0].SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    checkbox_texture[1].SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

    // initialize the sprites for option text
    text[0] = S_CreateRectangle(102.4f, 51.2f, ".//Textures/Mute_Button.png");
    text[1] = S_CreateRectangle(102.4f, 51.2f, ".//Textures/Fullscreen_Button.png");
    text[0].SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    text[1].SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    text_m[1].SetScale(1.2f, 1.2f);
    text_m[1].Concat();

    // create game objects to store the checkbox
    checkbox[0] = new GameObject(&checkbox_texture[0],
                      Col_Comp(0.f, 0.f, 0.f, 0.f, Rect));
    checkbox[1] = new GameObject(&checkbox_texture[1],
                      Col_Comp(0.f, 0.f, 0.f, 0.f, Rect));
    checkbox[0]->SetActive(true);
    checkbox[1]->SetActive(true);


}

/**************************************************************************************
//
// Updates the options menu 
//
**************************************************************************************/
void Update_Options()
{
    // shorter representation of x and y of check boxes
    float &x0 = checkbox[0]->PosX; 
    float &y0 = checkbox[0]->PosY; 
    float &x1 = checkbox[1]->PosX; 
    float &y1 = checkbox[1]->PosY; 

    // mouse coordinates
    int mouseX, mouseY;      

    AEGfxGetCamPosition(&x, &y);   // get camera coordinates to render options menu

    // translate the background wall paper
    BG_M.SetTranslate(x, y);
    BG_M.Concat();

    // Getting the mouse coordinates
    AEInputGetCursorPosition(&mouseX, &mouseY);
    mouseX = mouseX - (int)AEGfxGetWinMaxX();
    mouseY = (int)AEGfxGetWinMaxY() - mouseY;

    // update the collision box of the first check box
    x0 = x + 150.f;
    y0 = y - 30.f;
    checkbox[0]->Collision_.Update_Col_Pos(x0 - CHECKBOX_W, y0 - CHECKBOX_H,
                                           x0 + CHECKBOX_W, y0 + CHECKBOX_H);
    
    // update the collision box of the second check box
    x1 = x + 150.f;
    y1 = y - 80.f;
    checkbox[1]->Collision_.Update_Col_Pos(x1 - CHECKBOX_W, y1 - CHECKBOX_H,
                                           x1 + CHECKBOX_W, y1 + CHECKBOX_H);

    // check if player clicked the left mouse button
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // getting the float version of mouse coordinates
        float mouse_x = static_cast<float> (mouseX);
        float mouse_y = static_cast<float> (mouseY);
        
        // if cursor was within first check box
        if (checkbox[0]->Collision_.St_Rect_Point(mouse_x, mouse_y))
        {
            // change boolean
            Audio_Engine::MUTE_ = Audio_Engine::MUTE_ ? false : true;
        }
        
        // if cursor was within second check box
        if (checkbox[1]->Collision_.St_Rect_Point(mouse_x, mouse_y))
        {
            // change the boolean and toggle the screen depending on current status
            fullscreen = fullscreen ? false : true;
            AEToogleFullScreen(fullscreen);
        }
    }

}

/**************************************************************************************
//
// Render the options menu. Options are toggling fullscreen and muting sound
//
**************************************************************************************/
void Render_Options()
{
    // shorter representation of x and y of check boxes
    float &x0 = checkbox[0]->PosX;
    float &y0 = checkbox[0]->PosY;
    float &x1 = checkbox[1]->PosX;
    float &y1 = checkbox[1]->PosY;

    // only render the background if its in the main menu
    if(GSM::current == GS_MAIN)
        BG.Render_Object(BG_M);

    // print the ticked box or empty box depending on state
    if (Audio_Engine::MUTE_) // sound is muted currently
    {
        // translate and render the box with tick
        checkbox[1]->Transform_.SetTranslate(x0, y0);
        checkbox[1]->Transform_.Concat();
        checkbox[1]->Render();
    }
    else // sound is not muted currently
    {
        // translate and render the box without tick
        checkbox[0]->Transform_.SetTranslate(x0, y0);
        checkbox[0]->Transform_.Concat();
        checkbox[0]->Render();
    }
    
    // print the ticked box or empty box depending on state
    if (fullscreen) // currently fullscreen
    {
        // translate and render the box with tick
        checkbox[1]->Transform_.SetTranslate(x1, y1);
        checkbox[1]->Transform_.Concat();
        checkbox[1]->Render();
    }
    else // not in fullscreen
    {
        // translate and render the box without tick
        checkbox[0]->Transform_.SetTranslate(x1, y1);
        checkbox[0]->Transform_.Concat();
        checkbox[0]->Render();
    }

    // translate the the text MUTE to its location adn render it
    text_m[0].SetTranslate(x - 150, y - 30);
    text_m[0].Concat();
    text[0].Render_Object(text_m[0]);

    // translate the the text FULLSCREEN to its location adn render it
    text_m[1].SetTranslate(x - 150, y - 90);
    text_m[1].Concat();
    text[1].Render_Object(text_m[1]);
}

/**************************************************************************************
//
// Deletes the resources that was used in the option menu
// CALLED ONLY ONCE AT THE END OF THE GAME
//
**************************************************************************************/
void Cleanup_Options()
{
    // free the memory that was allocated
    delete checkbox[0];
    delete checkbox[1];
}


