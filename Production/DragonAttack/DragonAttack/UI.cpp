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
// have to initialise static members in global namespace
Particle_System* UI::flame_particles = nullptr;

UI::UI(Dragon* dragon)
	:HP_Sprite{ S_CreateSquare(20.0f, "Textures/hp.png") },
	Charge_Sprite{ S_CreateSquare(20.0f, "Textures/Fireball.png") },
	icon_w{ 20.0f },
	hp_icon1{ &HP_Sprite, Col_Comp() },
	hp_icon2{ &HP_Sprite, Col_Comp() },
	hp_icon3{ &HP_Sprite, Col_Comp() },
	charge_icon{ &Charge_Sprite, Col_Comp() },
	Dragon_hp{ dragon->Get_HP() }, Fireball_charge{ dragon->Get_Charge() }
	{
	
	// put icons at {AEGfxWinMinX() + offsetX, AEGfxWinMaxY() - offsetY}
	hp_icon1.Transform_.Concat();
	flame_particles = Effects_Get(MFIREBALL_PARTICLE);

	// BEHAVIOUR FOR MFIREBALL
	flame_particles->Emitter_.PPS_ = 5;
	flame_particles->Emitter_.Dist_Min_ = 10.f;
	flame_particles->Emitter_.Vol_Max = 2000;
	flame_particles->Emitter_.Direction_ = 90.0f;
	flame_particles->Emitter_.Particle_Rand_.Spread_ = 360;
	flame_particles->Emitter_.Conserve_ = 0.80f;
	flame_particles->Emitter_.Size_ = 10.0f;
	flame_particles->Emitter_.Speed_ = 4.0f;
	flame_particles->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
	flame_particles->Emitter_.Lifetime_ = 3.0f;
	flame_particles->Emitter_.Particle_Rand_.Life_Rand_ = 3;
	
	} 

	void UI::UI_Update(Dragon* dragon, const float dt)
	{
		UNREFERENCED_PARAMETER (dt);
		Dragon_hp = dragon->Get_HP();
		Fireball_charge = dragon->Get_Charge();
		hp_icon1.SetActive(true);
		hp_icon2.SetActive(true);
		hp_icon3.SetActive(true);
		charge_icon.SetActive(false);
		
	
		if (Dragon_hp == 2)
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
		}


		if (Fireball_charge == Max_Charge)
		{
			charge_icon.SetActive(true);
		}
		else 
			charge_icon.SetActive(false);

		f32 testX, testY;
		AEGfxGetCamPosition(&testX, &testY);
		//std::cout << testX << " " << testY << std::endl;
		//update the icons to follow the dragon , Translate it first
		/*hp_icon1.Transform_.SetTranslate(dragon->PosX - 50, dragon->PosY + 90);
		hp_icon2.Transform_.SetTranslate(dragon->PosX, dragon->PosY + 90);
		hp_icon3.Transform_.SetTranslate(dragon->PosX + 50, dragon->PosY + 90);*/
		hp_icon1.Transform_.SetTranslate(testX - 550 , testY + 300);
		hp_icon2.Transform_.SetTranslate(testX - 500, testY + 300);
		hp_icon3.Transform_.SetTranslate(testX - 450, testY + 300);
		//Concat the matrix for the icons
		hp_icon1.Transform_.Concat();
		hp_icon2.Transform_.Concat();
		hp_icon3.Transform_.Concat();
		if ( dragon->Get_Direction() == RIGHT )
		charge_icon.Transform_.SetTranslate(dragon->PosX -60 , dragon->PosY + 30 );
		if (dragon->Get_Direction() == LEFT)
			charge_icon.Transform_.SetTranslate(dragon->PosX + 60, dragon->PosY + 30);
		charge_icon.Transform_.Concat();

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
      flame_particles->Newton({ dragon->PosX - 60,dragon->PosY + 50 }, 0.2f);
    if (dragon->Get_Direction() == LEFT)
      flame_particles->Newton({ dragon->PosX + 60,dragon->PosY + 50 }, 0.2f);
		flame_particles->TransRamp_Exp();
		flame_particles->ColorRamp_Life();
		flame_particles->UpdateEmission();

		if (flame_particles->GetParticleCount())
		{
			flame_particles->Update(dt);
		}
	}

	void UI::Render()
	{
			hp_icon1.Render();
			hp_icon1.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
			hp_icon2.Render();
			hp_icon2.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
			hp_icon3.Render();
			hp_icon3.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		if (Fireball_charge == Max_Charge)
		{
			charge_icon.Render();
			charge_icon.Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
			flame_particles->Render();
		}
		
	}