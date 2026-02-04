/*****************************************************************************
* @brief : CmNode
* 模型节点，每个模型会用一个小型bsp对其多面体做分类，从而加快检索
* @author : acedtang
* @date : 2021/9/22
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

namespace Sun {

	struct CmNode {
		int						planeType;			
		float					planeDist;			
		CmPolygonRef*          polygons;			
		CmNode* parent_;				// parent of this node
		CmNode* children_[2];		// node children
	};

}