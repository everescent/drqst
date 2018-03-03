#include "AI_Test_Stage.h"
#include "AI_Data_Factory.h"
#include "Floor.h"


namespace
{
	Dragon *d;
	
	std::vector<Characters*> c;
	char num_of_mob = 1;

	Sprite *BG;
	Transform *M_BG;

	Floor *floor1;
}

namespace AI_Test_Stage
{
	void Init(void)
	{
		d->SetActive(true);
		d->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
	}

	void Load(void)
	{
		BG = new Sprite{ CreateBG(1.0f, "../../Illustrations/Environment/Cobblestone.png") };
		floor1 = new Floor{ 0.0f, -350.0f }; //ok
		M_BG = new Transform;
		d = new Dragon;
		c.push_back( Create_Boss_AI(LANCELOT));
	}

	void Update(float dt)
	{
		d->Update(*d, dt);
		c[0]->Update(*d, dt);
		
		floor1->Update(*d, dt);
	}

	void Draw(void)
	{
		BG->Render_Object(*M_BG);
		floor1->Render();
	
		d->Render();
		c[0]->Render();
	}

	void Free(void)
	{
		delete d;
		c.clear();
	}

	void Unload(void)
	{
	}
}