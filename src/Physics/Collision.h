/*****************************************************************************
* @brief : Collision.h
* @author : acedtang
* ÕýÊ½
* @date : 2021/8/2
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __COLLISION_H
#define __COLLISION_H

#include "Math/Vector3D.h"

namespace Sun {

	//The ray extends from p1 to p1 + fraction * (p2 - p1).
	struct RayCastInput
	{
		vec3 p0;
		vec3 p1;
		float fraction;
	};

}



#endif