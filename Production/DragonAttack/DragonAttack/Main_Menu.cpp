#include "Main_Menu.h"
#include "AEEngine.h"
#include "Sprite.h"
#include "Transform.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "Collision.h"
#include "GameStateManager.h"
#include "King_Arthur.h"
#include "Dragon.h"
#include "Grunt.h"
#include <utility>

namespace
{

	King_Arthur *ka = nullptr;
	Dragon *d = nullptr;
	Grunt *g = nullptr;
	
}

namespace Main_Menu
{
	void Init(void)
	{
		
	}

	void Load(void)
	{
		ka = new King_Arthur{};
		d = new Dragon{};
		g = new Grunt{};
	}

	void Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		d->SetActive(true);
		ka->Update(2.0f, *d);
		d->Update();

		g->Update(dt, g);
	}

	void Draw(void)
	{
		CamFollow(d->Transform_, Cam_Offset_X, Cam_Offset_Y, d->GetFacing());
		ka->Render();
		d->Sprite_.SetAlphaTransBM(1.0f, 1.0f, AE_GFX_BM_BLEND);
		d->Render();

		g->Render();
	}

	void Free(void)
	{
		delete ka;
		delete d;
		delete g;
	}

	void Unload(void)
	{
	
	}
}