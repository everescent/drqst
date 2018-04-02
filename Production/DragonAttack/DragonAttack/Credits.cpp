/* Start Header ************************************************************************/
/*!
\file       Credits.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
Credits page

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Credits.h"            // header file
#include "AEEngine.h"           // for font and frame time
#include "Camera.h"             // camposition
#include "GameStateManager.h"	// to change game states
#include "Particle_Effects.h"	// CREDIT_PARTICLE

// for global variables
namespace
{
    u32 fontID[2]; // fontid that will be used

	// the people that will be in the credit screen
    char* president  [2] = { "President"      , "Claude Comair"                                               };
    char* instructors[3] = { "Instructors"    , "Elie Hosry"    , "Michael David Thompson"                    };
    char* creators   [8] = { "Lead Designer"  , "Andrew Chong"  , "Technical Lead"          , "Javon Lee",
                             "Producer"       , "Jacob Lim"     , "Product Manager"         , "William Yoong" };
    // particle effects
	Particle_System* credit_effects;

	// update the particles in the system
    void Update_Particle();

	auto& g_dt = AEFrameRateControllerGetFrameTime;
}

namespace Credits
{
	/**************************************************************************************
	//
	// Loads the variables that are needed for the credit screen
	//
	**************************************************************************************/
	void Load(void)
    {
        // creating the font ids
        fontID[0] = AEGfxCreateFont("calibri", 32, true, false);
        fontID[1] = AEGfxCreateFont("calibri", 32, true, false);
        
        // reset camera position
        AEGfxSetCamPosition(0.0f, 0.0f);

		//set background color
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);   

        // initializing the particle variables
        credit_effects = Effects_Get(CREDIT_PARTICLE);
        credit_effects->Emitter_.Pos_.Min_Max.Point_Max = { AEGfxGetWinMaxX(), AEGfxGetWinMaxY() };
        credit_effects->Emitter_.Pos_.Min_Max.Point_Min = { AEGfxGetWinMinX(), AEGfxGetWinMinY() };
        credit_effects->Emitter_.Particles_.reserve(1000);
        credit_effects->Emitter_.PPS_ = 8;
        credit_effects->Emitter_.Vol_Max = 2000;
        credit_effects->Emitter_.Direction_ = 270.0f;
        credit_effects->Emitter_.Conserve_ = 0.8f;
        credit_effects->Emitter_.Size_ = 10.0f;
        credit_effects->Emitter_.Speed_ = 8.0f;
        credit_effects->Emitter_.Lifetime_ = 4.f;
    }
    
	/**************************************************************************************
	//
	// Init the variables that are needed for the credit screen
	//
	**************************************************************************************/
    void Init(void)
	{
        // warm up the particle systme before showing it
		credit_effects->WarmUp((float)g_dt(), 2, Update_Particle);
	}

	/**************************************************************************************
	//
	// Updates the timer and particle effects
	//
	**************************************************************************************/
	void Update(float dt)
	{
		static float time = 5.f;

		time > 0.f ? time -= dt : GSM::next = GS_QUIT;

		Update_Particle();
		credit_effects->UpdateEmission();
		credit_effects->Update(dt);
		
	}

	/**************************************************************************************
	//
	// Render the variables that are needed for the credit screen
	//
	**************************************************************************************/
	void Draw(void)
	{		
		// render the particle system
		credit_effects->Render();

		AEGfxSetRenderMode(AE_GFX_RM_COLOR); // render with color
		AEGfxTextureSet(NULL, 0, 0);		 // no texture needed
		AEGfxSetTransparency(1.0f);

		// print president on screen
		AEGfxPrint(fontID[0], president[0], -60, 215, 1.0f, 0.0f, 0.0f); 
		AEGfxPrint(fontID[1], president[1], -90, 188, 1.0f, 0.0f, 0.0f); 

		// print instructos on the screen
		AEGfxPrint(fontID[0],instructors[0], -60, 130, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID[1],instructors[1], -60, 100, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID[1],instructors[2], -140, 70, 1.0f, 1.0f, 1.0f);

		// print the creators on the left
		AEGfxPrint(fontID[0], creators[0], -370, -50, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID[1], creators[1], -370, -80, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID[0], creators[2], -370, -130, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID[1], creators[3], -370, -160, 1.0f, 1.0f, 1.0f);

		// print the creators on the right
		AEGfxPrint(fontID[0], creators[4],  150,  -50, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID[1], creators[5],  150,  -80, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID[0], creators[6],  150, -130, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID[1], creators[7],  150, -160, 1.0f, 1.0f, 1.0f);

	}

	/**************************************************************************************
	//
	// NOT USED
	//
	**************************************************************************************/
	void Free(void)
	{
		// empty
	}

	/**************************************************************************************
	//
	// Unloads the variables that were needed for the credit screen
	//
	**************************************************************************************/
	void Unload(void)
	{
        for (u32& i : fontID)
        {
            AEGfxDestroyFont(i);
        }
	}
}

namespace
{
	/**************************************************************************************
	//
	// updates the behaviour of the particle system
	//
	**************************************************************************************/
	void Update_Particle(void)
    {
        //Simulate an right side force
        credit_effects->Force(0.4f, true, false);
        //Add gravity
        credit_effects->Gravity(0.3f);

    }
}