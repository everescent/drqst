/* Start Header ************************************************************************/
/*!
\file       Collision.cpp
\author     Javon Lee
\par email: xiongweijavon.lee\@digipen.edu
\brief
Collision class functions are defined here.

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Collision.h" 

bool Col_Comp::Rect_Rect(const Col_Comp &aabb_obj1, const Col_Comp &aabb_obj2)
{
	//check if the two objects are Rectangles 

	if (aabb_obj1.shape != Rect || aabb_obj2.shape != Rect)
	{
		return false; // no collision if wrong function used. 
	}


	f32 left_A = aabb_obj1.min.x, left_B = aabb_obj2.min.x;
	f32 right_A = aabb_obj1.max.x, right_B = aabb_obj2.max.x;
	f32 top_A = aabb_obj1.max.y, top_B = aabb_obj2.max.y;
	f32 btm_A = aabb_obj1.min.y, btm_B = aabb_obj2.min.y;


	//Check for NO collision using AABB between 2 objects 
	if (left_A > right_B) return false;
	if (left_B > right_A) return false;
	if (top_A < btm_B) return false;
	if (top_B < btm_A) return false;
	//else return false

	return true;
}

bool Col_Comp::Circle_Circle(const Col_Comp &objA, const Col_Comp &objB)
{
	int RadiusSumSq = (objA.radius + objB.radius)*(objA.radius + objB.radius);
	f32 DistCircleSq = (objA.mid.x - objB.mid.x)*(objA.mid.x - objB.mid.x) + (objA.mid.y - objB.mid.y)*(objA.mid.y - objB.mid.y);

	if (DistCircleSq <= RadiusSumSq) return true; // There's collision

	return false;

}

void Col_Comp::Update_Col_Pos(f32 min_x, f32 min_y, f32 max_x, f32 max_y)
{
	min.x = min_x;
	min.y = min_y;
	max.x = max_x;
	max.y = max_y;

}

void Col_Comp::Update_Col_Pos(f32 mid_x, f32 mid_y)
{
	mid.x = mid_x;
	mid.y = mid_y;
}
