/*

*/

#ifndef __SUN_CM_MODEL_H
#define __SUN_CM_MODEL_H

#include <string>
#include "Math/Box.h"
#include "Util/QuickVector.h"
#include "CmVertex.h"
#include "CmEdge.h"
#include "CmPolygon.h"
#include "CmNode.h"

using String = std::string;

template<typename _T>
using Block = Vector<_T>;

namespace Sun {

	class CmModel {
	public:
		String name_;

		AABB bound_;
		
		//模型所有顶点
		Vector<CmVertex> vertices_;

		//模型所有边
		Vector<CmEdge> edges_;

		//模型所有节点
		Vector<CmNode> nodes_;

		//模型所有多边形
		Vector<CmPolygon> polygons_;

		//每个模型节点会引用多个多边形，不同节点可能会引用相同的多边形，所有引用信息存放在这里
		Block<CmPolygonRef> polygonRefBlocks_;

		CmNode* rootNode_ = nullptr;
	};




}

#endif