/* Start Header ************************************************************************/
/*!
\file       Collision.h
\author     Javon Lee Xiong Wei
\par email: xiongweijavon.lee\@digipen.edu
\brief
Collision component class declared here

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include "Transform.h"

// Meant to be shared to determine if you want which side of the ling you want to check
enum HalfPlane 
{
	INSIDE = 1,
	OUTSIDE
};

namespace
{
	enum Shape
	{
		NoShape,
		Point,
		Rect,
		Circle
	};

	enum CollisionFlag
	{
		Collided,
		Not_Collided
	};



	//enum Obj_T //an enumeration that includes all object types for easy comparison between object to object collision. 
	//{
	//	Obj_Dragon,
	//	Obj_Fireball,
	//	Obj_Megafireball,
	//	Obj_Enemy,
	//	Obj_KA,
	//};
	
}

class Col_Comp
{
public:
	Col_Comp(f32 mid_x, f32 mid_y, Shape shape) // constructor for point
		: mid{ mid_x, mid_y }, shape{ shape }
	{
		if (shape != Point)
			shape = NoShape;
	}

	Col_Comp(f32 min_x, f32 min_y, f32 max_x, f32 max_y, Shape shape) // constructor for rectangles/squares 
		: min{ min_x , min_y }, max{ max_x, max_y }, shape{ shape } 
	{
		if (shape != Rect)
			shape = NoShape;
	} //empty body constructor

	Col_Comp(f32 mid_x, f32 mid_y, int radius, Shape shape) // constructor for circles 
		: mid{ mid_x, mid_y }, radius{ radius }, shape{ shape }
	{
		if (shape != Circle)
			shape = NoShape;
	}
  //Jacob Suggested Edit START//////////////////////////////////////////////////////////////////////////
  Col_Comp(Col_Comp&& t_Col)
    :min{ t_Col.min.x, t_Col.min.y }, max{ t_Col.max.x, t_Col.max.y }, mid{ t_Col.mid.x, t_Col.mid.y },
    radius{ t_Col.radius }, shape{ t_Col.shape }, colflag{ t_Col.colflag }
  {
    //This allows me to construct Col_Comp by doing this:
    //Col_Comp test{Col_Comp{1.0f,1.0f,10,Circle}};
    //OR
    //Col_Comp test{Col_Comp{0.0f,0.0f,5.0f5.0f,Rectangle}};
    //Both will achieve the combined effect of all three constructors
    //This ALSO means that the classes inheriting your class or using it won't have
    //to dedicate 3 different types of constructors to cater to your 3 constructors
    //ON TOP of the constructers that they may need
  }
  //Jacob Suggested Edit END////////////////////////////////////////////////////////////////////////////

	Col_Comp() {} // call this constructor for non-collidable objects

	~Col_Comp() {}; //default dstor

	void Update_Col_Pos(f32 min_x, f32 min_y, f32 max_x, f32 max_y); // update for rectangle objects 

	void Update_Col_Pos(f32 mid_x, f32 mid_y); //update for Circle objects 

	bool St_Rect_Rect( const Col_Comp &aabb_obj2); //static collision for Rectangle to Rectangle

	bool St_Circle_Circle( const Col_Comp &aabb_obj2); //static collision for Circle to Circle

	bool St_Rect_Point(float point_x, float point_y);

	bool Dy_Rect_Rect(const Col_Comp &B, const AEVec2 &velocity_A, const AEVec2 &velocity_B, float dt);

	Shape GetShape(); 

	//William Edit START//////////////////////////////////////////////////////////////////////////
	bool Line_Point(Col_Comp &A, Col_Comp &B, AEVec2 & A_Pos, HalfPlane);

	bool Point_Rect(Col_Comp &A, Col_Comp &B);

	AEVec2 Get_Point(void) const;
    AEVec2 Get_MaxPoint(void) const;
    AEVec2 Get_MinPoint(void) const;
	//William Edit END////////////////////////////////////////////////////////////////////////////
private:

	AEVec2 min = { 0, 0 }; //represents btm left of a rect/sq

	AEVec2 max = { 0, 0 }; //represents top right of a rect/sq

	AEVec2 mid = { 0, 0 }; //represents the mid of a circle 

	int radius = 0; //represents the radius of a circle 

	Shape shape = NoShape;

	AEVec2 velocity = { 0, 0 }; //represents the current position of the object 

	CollisionFlag colflag = Not_Collided;

};




