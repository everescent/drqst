/* Start Header ************************************************************************/
/*!
\file       Particle_Effects.cpp
\author     William Yoong
\par email: william.yoong\@digipen.edu
\brief
This file stores the different particle effects for the game

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Particle_Effects.h"


namespace
{
    Particle_System* MFireball_Effects;
    Particle_System* Coin_Effects;
    Particle_System* ME_Effects;
    Particle_System* Arondight_Effects;
	Particle_System* KA_Healing_Effects;
    Particle_System* KA_Sword_Effects;
    Particle_System* KA_Slash_Effects;
	Particle_System* Credit_Effects;

    AEGfxVertexList* MFireball_Mesh;
    AEGfxVertexList* Coin_Mesh;
    AEGfxVertexList* ME_Mesh;
    AEGfxVertexList* Arondight_Mesh;
	AEGfxVertexList* KA_Healing_Mesh;
    AEGfxVertexList* KA_Sword_Mesh;
    AEGfxVertexList* KA_Slash_Mesh;
	AEGfxVertexList* Credit_Mesh;

    void Particle_Mesh_Init();
    void Particle_Effects_Init();
}

void Effects_Init(void)
{
    Particle_Mesh_Init();
    Particle_Effects_Init();
}

Particle_System* Effects_Get(EFFECTS type)
{
    switch (type)
    {
    case MFIREBALL_PARTICLE:  return MFireball_Effects;
        break;
    case COIN_PARTICLE:       return Coin_Effects;
        break;
    case ME_PARTICLE:         return ME_Effects;
        break;
    case ARONDIGHT_PARTICLE:  return Arondight_Effects;
        break;
    case KA_SLASH_PARTICLE:   return KA_Slash_Effects;
        break;
	case KA_HEALING_PARTICLE: return KA_Healing_Effects;
		break;
    case KA_SWORD_PARTICLE:   return KA_Sword_Effects;
        break;
	case CREDIT_PARTICLE:     return Credit_Effects;
		break;
    default: return nullptr;
        break;
    }
}

void Effects_Cleanup(void)
{
    delete   MFireball_Effects;
    delete   Coin_Effects;
    delete   ME_Effects;
	delete   Arondight_Effects;
	delete   KA_Healing_Effects;
    delete   KA_Sword_Effects;
    delete   KA_Slash_Effects;
	delete   Credit_Effects;

    AEGfxMeshFree(MFireball_Mesh);
    AEGfxMeshFree(Coin_Mesh);
    AEGfxMeshFree(ME_Mesh);
	AEGfxMeshFree(Arondight_Mesh);
	AEGfxMeshFree(KA_Healing_Mesh);
    AEGfxMeshFree(KA_Sword_Mesh);
    AEGfxMeshFree(KA_Slash_Mesh);
	AEGfxMeshFree(Credit_Mesh);
}

namespace
{
    void Particle_Mesh_Init()
    {
        // Mesh for mega fireball particles
        AEGfxMeshStart();
        AEGfxTriAdd(
            -0.1f, -0.1f, 0xFFE25822, 0.0f, 1.0f,
            0.1f, -0.1f,  0xFFE25822, 1.0f, 1.0f,
            -0.1f, 0.1f,  0xFFE25822, 0.0f, 0.0f);

        AEGfxTriAdd(
            0.1f, -0.1f, 0xFFFF7F4D, 1.0f, 1.0f,
            0.1f, 0.1f,  0xFFFF7F4D, 1.0f, 0.0f,
            -0.1f, 0.1f, 0xFFFF7F4D, 0.0f, 0.0f);


        MFireball_Mesh = AEGfxMeshEnd();
        AE_ASSERT_MESG(MFireball_Mesh, "fail to create object!!");

        // mesh for coin particle
        AEGfxMeshStart();
        AEGfxTriAdd(
            -0.1f, -0.1f, 0xFFFFD700, 0.0f, 1.0f,
            0.1f, -0.1f,  0xFFFFD700, 1.0f, 1.0f,
            -0.1f, 0.1f,  0xFFFFD700, 0.0f, 0.0f);

        AEGfxTriAdd(
            0.1f, -0.1f, 0xFFFFD700, 1.0f, 1.0f,
            0.1f, 0.1f,  0xFFFFD700, 1.0f, 0.0f,
            -0.1f, 0.1f, 0xFFFFD700, 0.0f, 0.0f);


        Coin_Mesh = AEGfxMeshEnd();
        AE_ASSERT_MESG(Coin_Mesh, "fail to create object!!");

        // mesh for mad enhancement particle
        AEGfxMeshStart();
        AEGfxTriAdd(
            -0.1f, -0.1f, 0xFFFFB7C5, 0.0f, 1.0f,
            0.1f, -0.1f,  0xFFFFB7C5, 1.0f, 1.0f,
            -0.1f, 0.1f,  0xFFFFB7C5, 0.0f, 0.0f);

        AEGfxTriAdd(
            0.1f, -0.1f, 0xFFFFB7C5, 1.0f, 1.0f,
            0.1f, 0.1f,  0xFFFFB7C5, 1.0f, 0.0f,
            -0.1f, 0.1f, 0xFFFFB7C5, 0.0f, 0.0f);


        ME_Mesh = AEGfxMeshEnd();
        AE_ASSERT_MESG(ME_Mesh, "fail to create object!!");

        // mesh for arondight particle
        AEGfxMeshStart();
        AEGfxTriAdd(
            -0.1f, -0.1f, 0xFFFFC0CB, 0.0f, 1.0f,
            0.1f, -0.1f, 0xFFFFC0CB, 1.0f, 1.0f,
            -0.1f, 0.1f, 0xFFFFC0CB, 0.0f, 0.0f);

        AEGfxTriAdd(
            0.1f, -0.1f, 0xFFFFC0CB, 1.0f, 1.0f,
            0.1f, 0.1f, 0xFFFFC0CB, 1.0f, 0.0f,
            -0.1f, 0.1f, 0xFFFFC0CB, 0.0f, 0.0f);


		Arondight_Mesh = AEGfxMeshEnd();
        AE_ASSERT_MESG(Arondight_Mesh, "fail to create object!!");

        // mesh for king arthur slash 
        AEGfxMeshStart();
        AEGfxTriAdd(
            -0.1f, -0.1f, 0xFFFFFF00, 0.0f, 1.0f,
            0.1f, -0.1f,  0xFFFFFF00, 1.0f, 1.0f,
            -0.1f, 0.1f,  0xFFFFFF00, 0.0f, 0.0f);

        AEGfxTriAdd(
            0.1f, -0.1f, 0xFFFFFF00, 1.0f, 1.0f,
            0.1f, 0.1f,  0xFFFFFF00, 1.0f, 0.0f,
            -0.1f, 0.1f, 0xFFFFFF00, 0.0f, 0.0f);


        KA_Slash_Mesh = AEGfxMeshEnd();
        AE_ASSERT_MESG(KA_Slash_Mesh, "fail to create object!!");

		// mesh for king arthur healing effect
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.1f, -0.1f, 0xFFFFF569, 0.0f, 1.0f,
			0.1f, -0.1f,  0xFFFFF569, 1.0f, 1.0f,
			-0.1f, 0.1f,  0xFFFFF569, 0.0f, 0.0f);

		AEGfxTriAdd(
			0.1f, -0.1f, 0xFFFFF569, 1.0f, 1.0f,
			0.1f, 0.1f,  0xFFFFF569, 1.0f, 0.0f,
			-0.1f, 0.1f, 0xFFFFF569, 0.0f, 0.0f);


		KA_Healing_Mesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(KA_Healing_Mesh, "fail to create object!!");

        // mesh for king arthur sword in phase 3
        AEGfxMeshStart();
        AEGfxTriAdd(
            -0.1f, -0.1f,0xFF00FF7F, 0.0f, 1.0f,
            0.1f, -0.1f, 0xFF00FF7F, 1.0f, 1.0f,
            -0.1f, 0.1f, 0xFF00FF7F, 0.0f, 0.0f);

        AEGfxTriAdd(
            0.1f, -0.1f, 0xFF00FF7F, 1.0f, 1.0f,
            0.1f, 0.1f,  0xFF00FF7F, 1.0f, 0.0f,
            -0.1f, 0.1f, 0xFF00FF7F, 0.0f, 0.0f);


        KA_Sword_Mesh = AEGfxMeshEnd();
        AE_ASSERT_MESG(KA_Sword_Mesh, "fail to create object!!")

		// mesh for credits screen
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.1f, -0.1f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.1f, -0.1f,  0xFFFFFFFF, 1.0f, 1.0f,
			-0.1f, 0.1f,  0xFFFFFFFF, 0.0f, 0.0f);

		AEGfxTriAdd(
			0.1f, -0.1f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.1f, 0.1f,  0xFFFFFFFF, 1.0f, 0.0f,
			-0.1f, 0.1f, 0xFFFFFFFF, 0.0f, 0.0f);


		Credit_Mesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(Credit_Mesh, "fail to create object!!")
        
}

    void Particle_Effects_Init()
    {
        MFireball_Effects = new Particle_System(MFireball_Mesh, {}, BOX);
        MFireball_Effects->Emitter_.Pos_.Point_Min_Max[0].x = -3.f;
        MFireball_Effects->Emitter_.Pos_.Point_Min_Max[0].y = -3.f;
        MFireball_Effects->Emitter_.Pos_.Point_Min_Max[1].x =  3.f;
        MFireball_Effects->Emitter_.Pos_.Point_Min_Max[1].y =  3.f;
        MFireball_Effects->Emitter_.Particles_.reserve(500);

        Coin_Effects = new Particle_System(Coin_Mesh, { 0.f, 0.f }, CENTER);
        Coin_Effects->Emitter_.Particles_.reserve(150);

        ME_Effects = new Particle_System(ME_Mesh, {}, BOX);
        ME_Effects->Emitter_.Particles_.reserve(150);

        Arondight_Effects = new Particle_System(Arondight_Mesh, {}, BOX);
        Arondight_Effects->Emitter_.Pos_.Point_Min_Max[0].x = 12.f;
        Arondight_Effects->Emitter_.Pos_.Point_Min_Max[0].y = -20.f;
        Arondight_Effects->Emitter_.Pos_.Point_Min_Max[1].x = 18.f;
        Arondight_Effects->Emitter_.Pos_.Point_Min_Max[1].y = 20.f;
        Arondight_Effects->Emitter_.Particles_.reserve(4096);

		KA_Healing_Effects = new Particle_System(KA_Healing_Mesh, {}, BOX);
		KA_Healing_Effects->Emitter_.Particles_.reserve(500);

        KA_Sword_Effects = new Particle_System(KA_Sword_Mesh, {}, CENTER);
        KA_Sword_Effects->Emitter_.Particles_.reserve(2000);

        KA_Slash_Effects = new Particle_System(KA_Slash_Mesh, {}, BOX);
        KA_Slash_Effects->Emitter_.Particles_.reserve(2048);

		Credit_Effects = new Particle_System(Credit_Mesh, {}, BOX);
        
    }
}




/* BEHAVIOUR FOR MFIREBALL

test->Emitter_.PPS_ = 5;
test->Emitter_.Dist_Min_ = 10.f;
test->Emitter_.Vol_Max = 2000;
test->Emitter_.Direction_ = 90.0f;
test->Emitter_.Particle_Rand_.Spread_ = 360;
test->Emitter_.Conserve_ = 0.80f;
test->Emitter_.Size_ = 10.0f;
test->Emitter_.Speed_ = 4.0f;
test->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
test->Emitter_.Lifetime_ = 3.0f;
test->Emitter_.Particle_Rand_.Life_Rand_ = 3;

test->UpdateEmission();
test->Turbulence(0.2f);
test->TransRamp_Exp();
test->Newton({0.f, 0.0f}, 0.3f);
*/

/*
BEHAVIOUR FOR COIN
test->Emitter_.PPS_ = 150;
test->Emitter_.Dist_Min_ = 10.f;
test->Emitter_.Vol_Max = 150;
test->Emitter_.Direction_ = 90.0f;
test->Emitter_.Particle_Rand_.Spread_ = 360;
test->Emitter_.Conserve_ = 0.9f;
test->Emitter_.Size_ = 20.0f;
test->Emitter_.Speed_ = 50.0f;
test->Emitter_.Particle_Rand_.Sp_Rand_ = 3;
test->Emitter_.Lifetime_ = 1.f;

test->TransRamp_Exp();
test->ColorRamp_Life();

*/