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
#include "King_Arthur.h"
#include "Floor.h"
#include "Wall.h"
#include <utility>


namespace
{
	Dragon *player;
	King_Arthur *enemy;
	Sprite *BG;
	Transform *M_BG;
	Floor *floor1;
	Wall *Lwall1, *Lwall2;
}

namespace Test_Stage1
{
	void Init(void)
	{

	}

	void Load(void)
	{
		BG = new Sprite{ CreateBG(1.0f, "floor.jpg") };
		M_BG = new Transform{};
		floor1 = new Floor{ 0.0f, -350.0f };

		Lwall1 = new Wall{ -620.0f, -160.0f };
		Lwall2 = new Wall{ -620.0f, 160.0f };

		player = new Dragon{};
		enemy = new King_Arthur{};
	}

	void Update(float dt)
	{
		floor1->Update(*player, dt);

		Lwall1->Update(*player);
		Lwall2->Update(*player);

		player->SetActive(true);
		player->Update(dt);
		enemy->SetActive(true);
		enemy->Update(dt, *player);
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
		enemy->Render();
	}

	void Free(void)
	{
		delete BG;
		delete Lwall1, Lwall2;
		delete floor1;

		delete player;
		delete enemy;
	}

	void Unload(void)
	{

	}
}