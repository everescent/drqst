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

UI::UI(Dragon* dragon)
	:icon_w{ 20.0f },
	hp_icon1{ S_CreateSquare(icon_w,"Textures/hp.png"), Col_Comp() },
	hp_icon2{ S_CreateSquare(icon_w,"Textures/hp.png"), Col_Comp() },
	hp_icon3{ S_CreateSquare(icon_w,"Textures/hp.png"), Col_Comp() },
	charge_icon{ S_CreateSquare(icon_w,"Textures/Fireball.png"), Col_Comp() },
	Dragon_hp{ dragon->Get_HP() }, Fireball_charge{ dragon->Get_Charge() }
	{
		
	// put icons at {AEGfxWinMinX() + offsetX, AEGfxWinMaxY() - offsetY}
	hp_icon1.Transform_.Concat();
	} //empty ctor body 

	void UI::UI_Update(Dragon* dragon)
	{
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
	}

	void UI::Render()
	{
			hp_icon1.Render();
			hp_icon1.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
			hp_icon2.Render();
			hp_icon2.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
			hp_icon3.Render();
			hp_icon3.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);

		if (Fireball_charge == Max_Charge)
		{
			charge_icon.Render();
			charge_icon.Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		}
		
	}