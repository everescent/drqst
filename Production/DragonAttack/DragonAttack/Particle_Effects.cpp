#include "Particle_Effects.h"


namespace
{
    Particle_System* MFireball_Effects;
    Particle_System* Coin_Effects;
    Particle_System* Arondight_Effects;

    AEGfxVertexList* MFireball_Mesh;
    AEGfxVertexList* Coin_Mesh;
    AEGfxVertexList* Ardongidht_Mesh;

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
    case MFIREBALL_PARTICLE: return MFireball_Effects;
        break;
    case COIN_PARTICLE:      return Coin_Effects;
        break;
    case ARONDIGHT_PARTICLE: return Arondight_Effects;
        break;
    default: return nullptr;
        break;
    }
}

void Effects_Cleanup(void)
{
    delete MFireball_Effects;
    delete Coin_Effects;

    AEGfxMeshFree(MFireball_Mesh);
    AEGfxMeshFree(Coin_Mesh);
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


        Ardongidht_Mesh = AEGfxMeshEnd();
        AE_ASSERT_MESG(Ardongidht_Mesh, "fail to create object!!");
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
        MFireball_Effects->Emitter_.Particles_.reserve(150);

        Arondight_Effects = new Particle_System(Ardongidht_Mesh, {}, BOX);
        Arondight_Effects->Emitter_.Pos_.Point_Min_Max[0].x = -3.f;
        Arondight_Effects->Emitter_.Pos_.Point_Min_Max[0].y = -3.f;
        Arondight_Effects->Emitter_.Pos_.Point_Min_Max[1].x = 3.f;
        Arondight_Effects->Emitter_.Pos_.Point_Min_Max[1].y = 300.f;
        Arondight_Effects->Emitter_.Particles_.reserve(4096);
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