#include "Test_Stage1.h"
#include "AEEngine.h"
#include "Sprite.h"
#include "Transform.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "Collision.h"
#include "GameStateManager.h"
#include "Dragon.h"
#include "Floor.h"
#include "Wall.h"
#include "AI_Data_Factory.h"
#include <utility>


namespace
{
	Dragon *player;
	//King_Arthur *enemy;
	Sprite *BG;
	Transform *M_BG;
	Floor *floor1;
	Wall *Lwall1, *Lwall2;
	//Mage *enemy;

	std::vector<Characters*> c;

	char num_of_mob = 1;
}

namespace Test_Stage1
{
	void Init(void)
	{
		for(char i = 0; i < num_of_mob; ++i)
		c[i]->SetActive(true);

		player->SetActive(true);
	}

	void Load(void)
	{
		BG = new Sprite{ CreateBG(1.0f, "floor.jpg") };
		M_BG = new Transform{};
		floor1 = new Floor{ 0.0f, -350.0f };

		Lwall1 = new Wall{ -620.0f, -160.0f };
		Lwall2 = new Wall{ -620.0f, 160.0f };

		player = new Dragon{};
		//enemy = new King_Arthur{};
		//enemy = new Mage{ AEVec2 {200.0f, 300.0f} };

		c.push_back(Create_Boss_AI(KING_ARTHUR));
	}
						 

	void Update(float dt)
	{
		floor1->Update(*player, dt);

		Lwall1->Update(*player);
		Lwall2->Update(*player);

		player->Update(*player, dt);

		for (char i = 0; i < num_of_mob; ++i)
			c[i]->Update(*player, dt);
			
	}

	void Draw(void)
	{

		if (AEInputCheckCurr(AEVK_LSHIFT))
		{
			CamFollow(player->Transform_, Cam_Offset_X, Cam_Offset_Y, player->GetFacing());
		}
		else
		{
			CamStatic();
		}

		BG->Render_Object(*M_BG);
		floor1->Render();

		player->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		player->Render();

		for (char i = 0; i < num_of_mob; ++i)
			c[i]->Render();
	}

	void Free(void)
	{
		delete BG;
		delete Lwall1;
		delete Lwall2;
		delete floor1;
		delete M_BG;
		delete player;
		//delete enemy;

		c.clear();
	}

	void Unload(void)
	{

	}
}