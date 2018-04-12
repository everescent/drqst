/* Start Header ************************************************************************/
/*!
\file       Score_Page.cpp
\project    Dragon Attack
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Cpp file for Score_Page

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/* End Header **************************************************************************/
#include "Score_Page.h"     // header file
#include "AEEngine.h"       // AEGfxCreateFont, AEGfxSetBackgroundColor, AEGfxSetCamPosition
#include "Characters.h"     // Get_Enemies_Killed, Get_Score
#include "PickUp.h"         // GetCoin
#include "StageManager.h"   // set_Next
#include "Load_Screen.h"

#define EFFECT_NUM 8 // number of fireworks in the score page

// for global variables
namespace
{
    u32 fontID;                                   // stores the font type
    Particle_System* score_effects[EFFECT_NUM];   // stores the fireworks
    const float offset = 2.f;                     // offset for emitter box

    // type to keep track on where the fireworks explode and how long they last
    struct Fireworks
    {
        float dist;
        float distEnd;
        float lifeTime;
        bool transit;
    };

    // fireworks array to keep track of the firework effects
    Fireworks fireworks[EFFECT_NUM];

}

// function declarations for this file
static float RNG(const float min, const float max);
static void  Reset_Effects(const int num);
static void  Transit_Fireworks(const int num);

/**************************************************************************************
//
// Loads the variables that are needed 
//
**************************************************************************************/
void Load_Score_Page(void)
{
    // initialize font
    fontID = AEGfxCreateFont("calibri", 32, true, false);
}

/**************************************************************************************
//
// Init the variables that are needed 
//
**************************************************************************************/
void Init_Score_Page(void)
{
    // initialize particle system
    score_effects[0] = Effects_Get(SCORE_PARTICLE);

    // update the particle system variables
    score_effects[0]->Emitter_.PPS_ = 2;
    score_effects[0]->Emitter_.Vol_Max = 800;
    score_effects[0]->Emitter_.Direction_ = 90.0f;
    score_effects[0]->Emitter_.Conserve_ = 0.8f;
    score_effects[0]->Emitter_.Size_ = 25.0f;
    score_effects[0]->Emitter_.Speed_ = 4.0f;
    score_effects[0]->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
    score_effects[0]->Emitter_.Lifetime_ = 1.f;
    score_effects[0]->Emitter_.Pos_.Min_Max.Point_Max = {};
    score_effects[0]->Emitter_.Pos_.Min_Max.Point_Min = {};

    // shallow copy over the emitter
    for (char i = 1; i < EFFECT_NUM; ++i)
    {
        score_effects[i] = new Particle_System(score_effects[0]->Emitter_.pMesh_, {}, BOX);
        score_effects[i]->Emitter_ = score_effects[0]->Emitter_;
    }


}
/**************************************************************************************
//
// Update the fireworks effect
//
**************************************************************************************/
void Update_Score_Page(const float dt)
{
    // reset camera position
    AEGfxSetCamPosition(0.0f, 0.0f);

    //set background color
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    //update particle effects
    for (char i = 0; i < EFFECT_NUM; ++i)
    {
        if (fireworks[i].dist == 0)
        {
            float minX;
            float minY;

            // get the minimum point of x and y
            minX = RNG(AEGfxGetWinMinX() + 80.f, AEGfxGetWinMaxX() - 80.f);
            minY = RNG(AEGfxGetWinMinY() + 40.f, AEGfxGetWinMaxY() - 300.f);
            
            // update the emiter box
            score_effects[i]->Emitter_.Pos_.Min_Max.Point_Min.x = minX;
            score_effects[i]->Emitter_.Pos_.Min_Max.Point_Min.y = minY;
            score_effects[i]->Emitter_.Pos_.Min_Max.Point_Max.x = minX + offset;
            score_effects[i]->Emitter_.Pos_.Min_Max.Point_Max.y	= minY + offset;

            
            fireworks[i].distEnd = RNG(80.f, 100.f);  // randomize the end position between 60-80
            fireworks[i].lifeTime = 3.f;              // lifetime of the fireworks
            ++fireworks[i].dist;
        }
        // move the emitter to its final destination
        else if (fireworks[i].dist < fireworks[i].distEnd)
        {
            // shoots the fireworks/emitter up
            score_effects[i]->Emitter_.Pos_.Min_Max.Point_Min.y += 2.f;
            score_effects[i]->Emitter_.Pos_.Min_Max.Point_Max.y += 2.f;
            ++fireworks[i].dist;

            // cause the particles to become smaller as time passes
            score_effects[i]->ScaleRamp(0.99f);
            score_effects[i]->Turbulence(0.3f);
            score_effects[i]->UpdateEmission();
        }

        // Reset the emitter
        else if(fireworks[i].lifeTime < 0)
        { 
            Reset_Effects(i);
        }

        // change emitter to an explosion/fireworks
        else
        {
            if (!fireworks[i].transit)
            {
                Transit_Fireworks(i);
                score_effects[i]->UpdateEmission();
            }

            // apply particle behaviour for fireworks
            score_effects[i]->Turbulence(0.2f);
            score_effects[i]->Gravity(1.f);
            score_effects[i]->Force(0.2f, false, true);
            score_effects[i]->ColorRamp_Life();
            score_effects[i]->TransRamp_Exp();
            fireworks[i].lifeTime -= dt;
        }

        // particle behaviours
        score_effects[i]->TransRamp_Exp();
        score_effects[i]->Update(dt);
    }

    // go to the next state if enter was pressed
    if (AEInputCheckTriggered(AEVK_RETURN))
    {
        SM::Set_Next(SS_LOAD);
    }

}
/**************************************************************************************
//
// Render the score page
//
**************************************************************************************/
void Render_Score_Page(void)
{
    // char array to store the things to be rendered in screen
    char score[50]           = {};
    char enemies_killed[50]  = {};
    char coins_collected[50] = {};

    // get the variables to render on screen
    sprintf_s(score,           "Score: %d",           Characters::Get_Score());
    sprintf_s(enemies_killed,  "Enemies Killed: %d",  Characters::Get_Enemies_Killed());
    sprintf_s(coins_collected, "Coins Collected: %d", PickUp::GetCoin());

    // render particle effects
    for (auto& elem : score_effects)
        elem->Render();

    AEGfxSetRenderMode(AE_GFX_RM_COLOR); // render with color
    AEGfxTextureSet(NULL, 0, 0);         // no texture needed
    AEGfxSetTransparency(1.0f);
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

    // print text on screen
    AEGfxPrint(fontID, score,           -60, 215, 1.0f, 1.0f, 1.0f);
    AEGfxPrint(fontID, enemies_killed,  -60, 188, 1.0f, 1.0f, 1.0f);
    AEGfxPrint(fontID, coins_collected, -60, 158, 1.0f, 1.0f, 1.0f);
    AEGfxPrint(fontID, "Press Enter ",  -60, 128, 1.0f, 1.0f, 1.0f);

}
/**************************************************************************************
//
// DOES NOTHING
//
**************************************************************************************/
void Free_Score_Page(void)
{
    // does nothing
}
/**************************************************************************************
//
// Unload the font, reset the scores for next state
//
**************************************************************************************/
void Unload_Score_Page(void)
{
    Characters::Reset_Score();
    Characters::Reset_Enemy_Killed();
    PickUp::ResetCoin();
    AEGfxDestroyFont(fontID);

    // free the memory that was allocated for the fireworks
    for (char i = 1; i < EFFECT_NUM; ++i)
    {
        delete score_effects[i];
    }

    // reseting the fireworks variables that keep tracks of the particle system
    for (Fireworks &elem : fireworks)
    {
        elem.dist    = 0;
        elem.transit = false;
    }
}
/**************************************************************************************
//
// randomize a number between two numbers
//
**************************************************************************************/
float RNG(const float min, const float max)
{
    return (min + rand() % (int)(max - min + 1));
}
/**************************************************************************************
//
// Reset the emitter to shoot fireworks
//
**************************************************************************************/
void Reset_Effects(const int num)
{
    score_effects[num]->Emitter_.PPS_ = 2;
    score_effects[0]->Emitter_.Conserve_ = 0.8f;
    score_effects[num]->Emitter_.Dist_Min_ = 0.f;
    score_effects[num]->Emitter_.Particle_Rand_.Spread_ = 0;
    score_effects[num]->Emitter_.Size_ = 25.0f;
    score_effects[num]->Emitter_.Speed_ = 4.0f;
    score_effects[num]->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
    score_effects[num]->Emitter_.Lifetime_ = 1.f;

    fireworks[num].dist = 0;
    fireworks[num].transit = false;
}
/**************************************************************************************
//
// Change the emitter to explode like fireworks
//
**************************************************************************************/
void Transit_Fireworks(const int num)
{
    score_effects[num]->Emitter_.PPS_ = 300;
    score_effects[0]->Emitter_.Conserve_ = 0.85f;
    score_effects[num]->Emitter_.Dist_Min_ = 0.f;
    score_effects[num]->Emitter_.Particle_Rand_.Spread_ = 360;
    score_effects[num]->Emitter_.Size_ = 20.0f;
    score_effects[num]->Emitter_.Speed_ = 25.0f;
    score_effects[num]->Emitter_.Particle_Rand_.Sp_Rand_ = 20;
    score_effects[num]->Emitter_.Lifetime_ = 3.f;

    fireworks[num].transit = true;
}
