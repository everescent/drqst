/* Start Header ************************************************************************/
/*!
\file       UI.cpp
\author     Javon Lee Xiong Wei
\par email: xiongweijavon.lee\@digipen.edu
\brief
UI class functions defined here.

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "UI.h"
#include <iostream>
// have to initialise static members in global namespace
Particle_System* UI::flame_particles = nullptr;

UI::UI(Dragon* dragon)
	:HP_Sprite{ new Sprite {S_CreateSquare(20.0f, "Textures/hp.png") } },
	Charge_Sprite{ new Sprite { S_CreateSquare(20.0f, "Textures/Fireball.png") } },
	icon_w{ 20.0f },
	HP_Trans{ new Transform {} },
	charge_icon { new GameObject{ Charge_Sprite, Col_Comp() } },
	Dragon_hp{ dragon->Get_HP() }, Fireball_charge{ dragon->Get_Charge() }
	{
	
	// put icons at {AEGfxWinMinX() + offsetX, AEGfxWinMaxY() - offsetY}
	//hp_icon1.Transform_.Concat();
	flame_particles = Effects_Get(MFIREBALL_PARTICLE);

	// BEHAVIOUR FOR MFIREBALL
	flame_particles->Emitter_.PPS_ = 15;
	flame_particles->Emitter_.Dist_Min_ = 10.f;
	flame_particles->Emitter_.Vol_Max = 800;
	flame_particles->Emitter_.Direction_ = 90.0f;
	flame_particles->Emitter_.Particle_Rand_.Spread_ = 360;
	flame_particles->Emitter_.Conserve_ = 0.80f;
	flame_particles->Emitter_.Size_ = 15.0f;
	flame_particles->Emitter_.Speed_ = 6.0f;
	flame_particles->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
	flame_particles->Emitter_.Lifetime_ = 1.0f;

	//flame_particles->Emitter_.Particle_Rand_.Life_Rand_ = 3;
	
	} 

UI::~UI()
	{
	delete HP_Sprite;
	delete Charge_Sprite;
	delete HP_Trans;
	delete charge_icon;
	}

	void UI::update_HP()
	{
		//int iterations;
		////Starting displacement to print the Left corner HP Sprite

		//AEVec2 displacement{ -550.0f , 300.0f };
		//for (iterations = 0; iterations < Dragon_hp ; ++iterations )
		//{
		//	HP_Trans->SetTranslate( CamPos.x + displacement.x,
		//							CamPos.y + displacement.y);
		//	HP_Trans->Concat();
		//	
		//	HP_Sprite->Render_Object(*HP_Trans);
		//	//Update the displacement to print next image,
		//	displacement.x += 50.0f;
		//}

	}

	void UI::UI_Update(Dragon* dragon, const float dt)
	{
		UNREFERENCED_PARAMETER (dt);

		AEGfxGetCamPosition(&(CamPos.x), &(CamPos.y));
		
		Dragon_hp = dragon->Get_HP();
		if (Dragon_hp == 0 || (AEInputCheckTriggered(AEVK_R)) ) SM::Set_Next(SS_RESTART);
		Fireball_charge = dragon->Get_Charge();
		/*hp_icon1.SetActive(true);
		hp_icon2.SetActive(true);
		hp_icon3.SetActive(true);*/
		
		charge_icon->SetActive(false);

		/*std::cout << "Dragon's HP:" << dragon->Get_HP() << std::endl;
		
		*/ //std::cout << "Dragon's HP:" << Dragon_hp << std::endl;
		
		/*if (Dragon_hp == 2)
		{
			hp_icon3.SetActive(false);
		}
		else if (Dragon_hp == 1)
		{
			hp_icon3.SetActive(false);
			hp_icon2.SetActive(false);
		}
		else if ( Dragon_hp < 1)
		{
			hp_icon3.SetActive(false);
			hp_icon2.SetActive(false);
			hp_icon1.SetActive(false);
		}*/

		if (Fireball_charge == Max_Charge)
		{
			charge_icon->SetActive(true);
		}
		else 
			charge_icon->SetActive(false);
		
		//std::cout << testX << " " << testY << std::endl;
		//update the icons to follow the camera , Translate it first

		/*hp_icon1.Transform_.SetTranslate(testX - 550 , testY + 300);
		hp_icon2.Transform_.SetTranslate(testX - 500, testY + 300);
		hp_icon3.Transform_.SetTranslate(testX - 450, testY + 300);*/

		//Concat the matrix for the icons
		/*hp_icon1.Transform_.Concat();
		hp_icon2.Transform_.Concat();
		hp_icon3.Transform_.Concat();*/


		if ( dragon->Get_Direction() == RIGHT )
		charge_icon->Transform_.SetTranslate(dragon->PosX -60 , dragon->PosY + 30 );
		if (dragon->Get_Direction() == LEFT)
			charge_icon->Transform_.SetTranslate(dragon->PosX + 60, dragon->PosY + 30);
		charge_icon->Transform_.Concat();

    if (dragon->Get_Direction() == RIGHT)
    {
      flame_particles->Emitter_.Pos_.Min_Max.Point_Max.x = dragon->PosX - 30;
      flame_particles->Emitter_.Pos_.Min_Max.Point_Min.x = dragon->PosX - 90;
      flame_particles->Emitter_.Pos_.Min_Max.Point_Max.y = dragon->PosY + 50;
      flame_particles->Emitter_.Pos_.Min_Max.Point_Min.y = dragon->PosY + 30;
    }
    if (dragon->Get_Direction() == LEFT)
    {
      flame_particles->Emitter_.Pos_.Min_Max.Point_Max.x = dragon->PosX + 90;
      flame_particles->Emitter_.Pos_.Min_Max.Point_Min.x = dragon->PosX + 40;
      flame_particles->Emitter_.Pos_.Min_Max.Point_Max.y = dragon->PosY + 50;
      flame_particles->Emitter_.Pos_.Min_Max.Point_Min.y = dragon->PosY + 30;
    }

    flame_particles->UpdateEmission();
    flame_particles->Turbulence(0.2f);
    flame_particles->TransRamp_Exp();

    if (dragon->Get_Direction() == RIGHT)
      flame_particles->Newton({ dragon->PosX - 60,dragon->PosY +10 }, 0.2f);

    if (dragon->Get_Direction() == LEFT)
      flame_particles->Newton({ dragon->PosX + 60,dragon->PosY + 10}, 0.2f);

		flame_particles->TransRamp_Exp();
		//flame_particles->ColorRamp_Life();
		flame_particles->UpdateEmission();

		if (flame_particles->GetParticleCount())
		{
			flame_particles->Update(dt);
		}

		///////////////////////////////////////////////////// MMUST REEEMMOOOVVVEEE FOR LEVEL TESTING /////////////////////////////////////////////
		if (AEInputCheckTriggered(AEVK_RSHIFT))
		{
			dragon->Set_HP(8);
		}
		///////////////////////////////////////////////////// MMUST REEEMMOOOVVVEEE FOR LEVEL TESTING /////////////////////////////////////////////


	}

	void UI::Render()
	{
			/*hp_icon1.Render();
			hp_icon1.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
			hp_icon2.Render();
			hp_icon2.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
			hp_icon3.Render();
			hp_icon3.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);*/
		int iterations;
		//Starting displacement to print the Left corner HP Sprite

		AEVec2 displacement{ -550.0f , 300.0f };
		for (iterations = 0; iterations < Dragon_hp; ++iterations)
		{
			HP_Trans->SetTranslate(CamPos.x + displacement.x,
				CamPos.y + displacement.y);
			HP_Trans->Concat();

			HP_Sprite->Render_Object(*HP_Trans);
			HP_Sprite->SetAlphaTransBM (1.0f, 1.0f, AE_GFX_BM_BLEND);
			//Update the displacement to print next image,
			displacement.x += 50.0f;
		}
		

		if (Fireball_charge == Max_Charge)
		{
			flame_particles->Render();
			charge_icon->Render();
			charge_icon->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
			
		}
		
	}