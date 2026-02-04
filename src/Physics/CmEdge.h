/*****************************************************************************
* @brief : CmEdge
* Åö×²Ä£ÐÍ±ß
* @author : acedtang
* @date : 2021/9/22
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_CM_EDGE_H
#define __SUN_CM_EDGE_H

#include "Math/Vector3D.h"

namespace Sun {

	struct CmEdge{
	public:
		int vertexNo[2];
		vec3 normal;
	};

}

#endif