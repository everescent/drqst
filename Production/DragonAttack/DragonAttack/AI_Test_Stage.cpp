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

	AEGfxVertexList *pMesh; // the mesh the particles will use

	Particle_System test(nullptr, {0.f, 0.f});

	//Floor *floor1;


}

namespace AI_Test_Stage
{
	void Init(void)
	{
		d->SetActive(true);
		d->Sprite_->SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		
		//floor1->Collision_.Update_Col_Pos(-600.0f, -360.0f, 600.0f, -300.0f);
	

		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.1f, -0.1f, 0xFFFFC0CB, 0.0f, 1.0f,
			0.1f, -0.1f,  0xFFFFC0CB, 1.0f, 1.0f,
			-0.1f, 0.1f,  0xFFFFC0CB, 0.0f, 0.0f);

		AEGfxTriAdd(
			0.1f, -0.1f, 0xFFFFC0CB, 1.0f, 1.0f,
			0.1f, 0.1f,  0xFFFFC0CB, 1.0f, 0.0f,
			-0.1f, 0.1f, 0xFFFFC0CB, 0.0f, 0.0f);


		pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pMesh, "fail to create object!!");

		test.Emitter_.pMesh_ = pMesh;
		test.Emitter_.Pos_ = {0.0f, 0.0f};
		test.Emitter_.PPS_ = 8;
		test.Emitter_.Vol_Max = 4096;
		test.Emitter_.Direction_ = 90.0f;
		test.Emitter_.Particle_Rand_.Spread_ = 360;
		test.Emitter_.Conserve_ = 0.80f;
		test.Emitter_.Size_ = 10.0f;
		test.Emitter_.Speed_ = 4.0f;
		test.Emitter_.Particle_Rand_.Sp_Rand_ = 3;
		test.Emitter_.Lifetime_ = 4.0f;
		test.Emitter_.Particle_Rand_.Life_Rand_ = 3;
	}

	void Load(void)
	{
		BG = new Sprite{ CreateBG(1.0f, 2.0f, "Textures/Cobblestone.png") };
		//floor1 = new Floor{ -200.0f, -350.0f }; //ok
		M_BG = new Transform;
		d = dynamic_cast<Dragon*> (Create_Basic_AI(DRAGON));
		c.push_back( Create_Boss_AI(LANCELOT));
	}

	void Update(float dt)
	{
		d->Update(*d, dt);
		c[0]->Update(*d, dt);
		
		//floor1->Update(*d, dt);

		//Create the particles for emission
		test.UpdateEmission();
		//Turbulence simulates brownian motion
		//Passing in equations for phase x and y
		test.Turbulence(0.2f);
		//Simulate an upward force
		test.Force(0.2f, false, true);
		//Add gravity
		test.Gravity(0.01f);
		//Set exposure as a factor of lifetime
		//test.ColorRamp_Life();
		//Set transparency as a factor of Exposure
		//test.TransRamp_Exp();
		//Updates all particles
		test.Update(dt);
	}

	void Draw(void)
	{
		BG->Render_Object(*M_BG);
		//floor1->Render();
	
		d->Render();
		c[0]->Render();

		//Render Particles
		test.Render();
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