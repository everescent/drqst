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

	Col_Comp() {} // call default constructor if no parameters given. 

	~Col_Comp() {}; //default dstor

	void Update_Col_Pos(f32 min_x, f32 min_y, f32 max_x, f32 max_y); // update for rectangle objects 

	void Update_Col_Pos(f32 mid_x, f32 mid_y); //update for Circle objects 

	bool Rect_Rect(const Col_Comp &aabb_obj1, const Col_Comp &aabb_obj2);

	bool Circle_Circle(const Col_Comp &aabb_obj1, const Col_Comp &aabb_obj2);





private:

	AEVec2 min = { 0, 0 }; //represents btm left of a rect/sq

	AEVec2 max = { 0, 0 }; //represents top right of a rect/sq

	AEVec2 mid = { 0, 0 }; //represents the mid of a circle 

	int radius = 0; //represents the radius of a circle 

	Shape shape = NoShape;

	CollisionFlag colflag = Not_Collided;

};




