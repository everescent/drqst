#include "Score_Page.h"
#include "AEEngine.h"
#include "Characters.h"
#include "PickUp.h"

#define EFFECT_NUM 8

// for global variables
namespace
{
	u32 fontID;
	Particle_System* score_effects[EFFECT_NUM];

	const float offset = 2.f;

	struct Fireworks
	{
		float dist;
		float distEnd;
		float lifeTime;
		bool transit;
	};

	Fireworks fireworks[EFFECT_NUM];

}

// function declarations for this file
static float RNG(const float min, const float max);
static void  Reset_Effects(const int num);
static void  Transit_Fireworks(const int num);

void Load_Score_Page(void)
{
	// initialize font
	fontID = AEGfxCreateFont("calibri", 32, true, false);
}

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
	score_effects[0]->Emitter_.Lifetime_ = 4.f;
	score_effects[0]->Emitter_.Pos_.Min_Max.Point_Max = {};
	score_effects[0]->Emitter_.Pos_.Min_Max.Point_Min = {};

	// shallow copy over
	for (char i = 1; i < EFFECT_NUM; ++i)
	{
		score_effects[i] = new Particle_System(score_effects[0]->Emitter_.pMesh_, {}, BOX);
		score_effects[i]->Emitter_ = score_effects[0]->Emitter_;
	}
}

void Print_Score_Page(const float dt)
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
			minY = RNG(AEGfxGetWinMinY() + 80.f, AEGfxGetWinMaxY() - 80.f);
			
			// update the emiter box
			score_effects[i]->Emitter_.Pos_.Min_Max.Point_Min.x = minX;
			score_effects[i]->Emitter_.Pos_.Min_Max.Point_Min.y = minY;
			score_effects[i]->Emitter_.Pos_.Min_Max.Point_Max.x = minX + offset;
			score_effects[i]->Emitter_.Pos_.Min_Max.Point_Max.y	= minY + offset;

			
			fireworks[i].distEnd = RNG(60.f, 80.f);  // randomize the end position between 60-80
			fireworks[i].lifeTime = 1.5f;            // lifetime of the fireworks
			++fireworks[i].dist;
		}
		// move the emitter to its final destination
		else if (fireworks[i].dist < fireworks[i].distEnd)
		{
			score_effects[i]->Emitter_.Pos_.Min_Max.Point_Min.y += 1.f;
			score_effects[i]->Emitter_.Pos_.Min_Max.Point_Max.y += 1.f;
			++fireworks[i].dist;

			// cause the particles to become smaller as time passes
			score_effects[i]->ScaleRamp(0.99f);
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
			}

			// apply particle behaviour for fireworks
			score_effects[i]->Gravity(0.5f);
			score_effects[i]->Force(0.2f, false, true);
			fireworks[i].lifeTime -= dt;
		}

		// particle behaviours
		score_effects[i]->Turbulence(0.3f);
		score_effects[i]->TransRamp_Exp();
		score_effects[i]->UpdateEmission();
		score_effects[i]->Update(dt);
	}



}

void Render_Score_Page(void)
{
	char score[50]           = {};
	char enemies_killed[50]  = {};
	char coins_collected[50] = {};

	sprintf_s(score,           "Score: %d",           Characters::Get_Score());
	sprintf_s(enemies_killed,  "Enemies Killed: %d",  Characters::Get_Enemies_Killed());
	sprintf_s(coins_collected, "Coins Collected: %d", PickUp::GetCoin());

	// render particle effects
	for (auto& elem : score_effects)
		elem->Render();

	AEGfxSetRenderMode(AE_GFX_RM_COLOR); // render with color
	AEGfxTextureSet(NULL, 0, 0);		 // no texture needed
	AEGfxSetTransparency(1.0f);

	// print text on screen
	AEGfxPrint(fontID, score,           -60, 215, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, enemies_killed,  -60, 188, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, coins_collected, -60, 158, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "Press Enter ",  -60, 128, 1.0f, 1.0f, 1.0f);

}

void Free_Score_Page(void)
{

	//Transit_Fireworks(0);
	//Reset_Effects(0);

}

void Unload_Score_Page(void)
{
	Characters::Reset_Score();
	Characters::Reset_Enemy_Killed();
	PickUp::ResetCoin();
	AEGfxDestroyFont(fontID);


	for (char i = 1; i < EFFECT_NUM; ++i)
	{
		delete score_effects[i];
	}
}

float RNG(const float min, const float max)
{
	return (min + rand() % (int)(max - min + 1));
}

void Reset_Effects(const int num)
{
	score_effects[num]->Emitter_.PPS_ = 2;
	score_effects[num]->Emitter_.Dist_Min_ = 0.f;
	score_effects[num]->Emitter_.Particle_Rand_.Spread_ = 0;
	score_effects[num]->Emitter_.Size_ = 25.0f;
	score_effects[num]->Emitter_.Speed_ = 4.0f;
	score_effects[num]->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
	score_effects[num]->Emitter_.Lifetime_ = 1.f;

	fireworks[num].dist = 0;
	fireworks[num].transit = false;
}

void Transit_Fireworks(const int num)
{
	score_effects[num]->Emitter_.PPS_ = 5;
	score_effects[num]->Emitter_.Dist_Min_ = 20.f;
	score_effects[num]->Emitter_.Particle_Rand_.Spread_ = 360;
	score_effects[num]->Emitter_.Size_ = 15.0f;
	score_effects[num]->Emitter_.Speed_ = 12.0f;
	score_effects[num]->Emitter_.Lifetime_ = 1.f;

	fireworks[num].transit = true;
}
