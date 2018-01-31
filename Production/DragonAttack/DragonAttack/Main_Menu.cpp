#include "Main_Menu.h"
#include "AEEngine.h"
#include "Sprite.h"
#include "Transform.h"
#include "Create_Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "Collision.h"
#include "GameStateManager.h"
#include <utility>


namespace
{
	class testCol : public GameObject {
	public:
		void Pos() {/*Do something*/ }
		void Update() {/*Do something*/ }
		testCol()
			:colli{ 0.0f,0.0f, 50, Circle }, GameObject{ S_CreateSquare(50.0f,1.0f,1.0f,"floor.jpg") }
		{}

		Col_Comp colli;
	};
	static float aX = -300.0f;
	static float aY = 0.0f;
	static float bX = 300.0f;
	static float bY = 0.0f;

	testCol *a = nullptr;
	testCol *b = nullptr;
}

namespace Main_Menu
{
	void Init(void)
	{
		
	}

	void Load(void)
	{
		a = new testCol;
		b = new testCol;
	}

	void Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		aX += 2.0f;
		bX -= 2.0f;
		a->Transform_.SetTranslate(aX, aY);
		b->Transform_.SetTranslate(bX, bY);
		a->colli.Update_Col_Pos(aX, aY);
		b->colli.Update_Col_Pos(bX, bY);
		if (a->colli.Circle_Circle(a->colli, b->colli))
			GSM::current = GS_QUIT;
	}

	void Draw(void)
	{
	    a->Render();
		b->Render();
	}

	void Free(void)
	{
		delete a;
		delete b;
	}

	void Unload(void)
	{
	
	}
}