/*****************************************************************************
* @brief : CmPolygon
* 碰撞模型多边形
* @author : acedtang
* @date : 2021/9/22
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_CM_POLYGON_H
#define __SUN_CM_POLYGON_H

#include "Math/Plane.h"
#include "Math/Box.h"

namespace Sun {

	struct CmPolygon {
	public:
		AABB box;
		Plane plane;
		int numEdgeNos;
		//变长
		int edgeNos[1];
	};

	//引用结构体，引用一个CmPolygon,同时内部有个next指针，支持串成链表结构
	struct CmPolygonRef {
		CmPolygon* polygon;
		CmPolygonRef* next;
	};

}

#endif