#include "AI_Test_Stage.h"
#include "AI_Data_Factory.h"
#include "Particle_Effects.h"
#include "Floor.h"


namespace
{
	Dragon *d;
	
	std::vector<Characters*> c;
	char num_of_mob = 1;

	Sprite *BG;
	Transform *M_BG;

	AEGfxVertexList *pMesh; // the mesh the particles will use

    Particle_System *test;// (nullptr, { 0.f, 0.f }, CENTER);

	//Floor *floor1;

    float angle;
}

namespace AI_Test_Stage
{
	void Init(void)
	{
		d->SetActive(true);
		d->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		
		//floor1->Collision_.Update_Col_Pos(-600.0f, -360.0f, 600.0f, -300.0f); 0xFFFFC0CB
	    
        //Effects_Init();
        //test = Effects_Get(ARONDIGHT_PARTICLE);
		//
		//test->Emitter_.PPS_ = 5;
        //test->Emitter_.Dist_Min_ = 10.f;
		//test->Emitter_.Vol_Max = 2048;
		//test->Emitter_.Direction_ = 90.0f;
		//test->Emitter_.Particle_Rand_.Spread_ = 180;
		//test->Emitter_.Conserve_ = 0.8f;
		//test->Emitter_.Size_ = 10.0f;
		//test->Emitter_.Speed_ = 5.0f;
		////test->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
		//test->Emitter_.Lifetime_ = 4.f;
		////test->Emitter_.Particle_Rand_.Life_Rand_ = 1;




        //test->Emitter_.Pos_.Point_Min_Max[0].x = 0.f;
        //test->Emitter_.Pos_.Point_Min_Max[0].y = -20.f;
        //test->Emitter_.Pos_.Point_Min_Max[1].x = 18.f;
        //test->Emitter_.Pos_.Point_Min_Max[1].y = -15.f;
	}

	void Load(void)
	{
		BG = new Sprite{ CreateBG(1.0f, 2.0f, "Textures/Cobblestone.png") };
		//floor1 = new Floor{ -200.0f, -350.0f }; //ok
		M_BG = new Transform;
		d = dynamic_cast<Dragon*> (Create_Basic_AI(DRAGON));
		c.push_back(Create_Basic_AI(MAGE, {}));
		//c.push_back(Create_Boss_AI(LANCELOT));
	}

	void Update(float dt)
	{
		d->Update(*d, dt);
		c[0]->Update(*d, dt);
		
        //static bool PPS = false;

        //if (PPS)
        //  test->Emitter_.PPS_ = 0;

		//floor1->Update(*d, dt);

		//Create the particles for emission
		//test->UpdateEmission();
		//Turbulence simulates brownian motion
		//Passing in equations for phase x and y
		//test->Turbulence(0.4f);
		//Simulate an upward force
		//test->Force(0.5f, false, true);
		//Add gravity
		//test->Gravity(0.2f);
       // test->Drag(0.5f);
		//Set exposure as a factor of lifetime
		//test->ColorRamp_Life();
		//Set transparency as a factor of Exposure
		//test->TransRamp_Exp();
		//Updates all particles
        //test->Newton({15.f, 50.0f}, 0.3f);
        //test->Emitter_.Pos_.Min_Max.Point_Max.x += 1;
        //test->Emitter_.Pos_.Min_Max.Point_Min.x += 1;
        //test->Emitter_.Pos_.Min_Max.Point_Max.y += 1;
        //test->Emitter_.Pos_.Min_Max.Point_Min.y += 1;
       // test->Emitter_.Pos_.Min_Max.Angle_ += PI * 0.00002f;

		//test->Update(dt);
       
        
        //PPS = true;
	}

	void Draw(void)
	{
		BG->Render_Object(*M_BG);
		//floor1->Render();
	
		d->Render();
		c[0]->Render();

		//Render Particles
		//test->Render();
	}

	void Free(void)
	{
		delete d;
		delete c[0];
		delete BG;
		c.clear();
	}

	void Unload(void)
	{
	}
}