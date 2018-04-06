#include "Options.h"
#include "Create_Object.h"
#include "Camera.h"
#include "Sprite.h"
#include "Transform.h"
#include "GameObject.h"

#include <iostream>
// for global variables
namespace
{
#define CHECKBOX_W 20.0f
#define CHECKBOX_H 20.0f
    
    Sprite     BG;                                     // background texture
    Transform  BG_M;                                   // background matrix
    Sprite     checkbox_texture[2];                    // checkbox   
    u32        fontID;                                 // fontID
    char*      options[2] = { "Sound" , "Fullscreen"}; // Strings to print
    bool       mute;                                   // check current state
    bool       fullscreen;                             // check current state
    float      x, y;                                   // x and y coordinate of camera
    GameObject* checkbox[2];                           // for  check boxes

    void toggle_Screen();
}

void Init_Options()
{
    BG = S_CreateRectangle(128.f, 72.0f, ".//Textures/Main_Menu_BG.png");
    BG_M.SetScale(5.f, 5.f);
    BG_M.Concat();

    checkbox_texture[0] = S_CreateRectangle(20.0f, 20.0f, ".//Textures/TickBox_NoTick.png");
    checkbox_texture[1] = S_CreateRectangle(20.0f, 20.0f, ".//Textures/TickBox_Tick.png");
    checkbox_texture[0].SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
    checkbox_texture[1].SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

    checkbox[0] = new GameObject(&checkbox_texture[0],
                      Col_Comp(0.f, 0.f, 0.f, 0.f, Rect));
    checkbox[1] = new GameObject(&checkbox_texture[1],
                      Col_Comp(0.f, 0.f, 0.f, 0.f, Rect));
    checkbox[0]->SetActive(true);
    checkbox[1]->SetActive(true);

    fontID = AEGfxCreateFont("calibri", 48, true, false);

}

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

    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // getting the float version of mouse coordinates
        float mouse_x = static_cast<float> (mouseX);
        float mouse_y = static_cast<float> (mouseY);
        
        if (checkbox[0]->Collision_.St_Rect_Point(mouse_x, mouse_y))
        {
            // change boolean
            mute = mute ? false : true;
        }
        
        if (checkbox[1]->Collision_.St_Rect_Point(mouse_x, mouse_y))
        {
            fullscreen = fullscreen ? false : true;
            toggle_Screen();
        }
    }

}

void Render_Options()
{
    // shorter representation of x and y of check boxes
    float &x0 = checkbox[0]->PosX;
    float &y0 = checkbox[0]->PosY;
    float &x1 = checkbox[1]->PosX;
    float &y1 = checkbox[1]->PosY;
    
    int camX = static_cast<int>(x);
    int camY = static_cast<int>(y);

    BG.Render_Object(BG_M);

    if (mute)
    {
        checkbox[1]->Transform_.SetTranslate(x0, y0);
        checkbox[1]->Transform_.Concat();
        checkbox[1]->Render();
    }
    else
    {
        checkbox[0]->Transform_.SetTranslate(x0, y0);
        checkbox[0]->Transform_.Concat();
        checkbox[0]->Render();
    }
    
    if (fullscreen)
    {
        checkbox[1]->Transform_.SetTranslate(x1, y1);
        checkbox[1]->Transform_.Concat();
        checkbox[1]->Render();
    }
    else
    {
        checkbox[0]->Transform_.SetTranslate(x1, y1);
        checkbox[0]->Transform_.Concat();
        checkbox[0]->Render();
    }

    AEGfxSetRenderMode(AE_GFX_RM_COLOR); // render with color
    AEGfxTextureSet(NULL, 0, 0);		 // no texture needed
    AEGfxSetTransparency(1.0f);
   
    AEGfxPrint(fontID, options[0], camX - 150, camY - 40,  255.f, 140.f / 255.0f, 0.0f);
    AEGfxPrint(fontID, options[1], camX - 150, camY - 100, 255.f, 140.f / 255.0f, 0.0f);
}

void Cleanup_Options()
{
    AEGfxDestroyFont(fontID);
    delete checkbox[0];
    delete checkbox[1];
}

void Set_Mute(bool status)
{
    mute = status;
}

bool Check_Mute()
{
    return mute;
}

GameObject **Get_Checkbox0()
{
    return checkbox;
}

namespace 
{
    void toggle_Screen()
    {
        AEToogleFullScreen(fullscreen);
    }
}
