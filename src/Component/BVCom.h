/*****************************************************************************
* @brief : BVCom
* bounding Volum组件基类
* @author : acedtang
* @date : 2021/8/8
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __BVCOM_H
#define __BVCOM_H

#include "Math/Box.h"
#include "Component/IComponent.h"

namespace Sun {

	enum BoundingVolumeType {
		BV_NULL,
		BV_AABB,
		BV_OBB,
		BV_SPHERE,
		BV_POINT,
	};

	class BoundingVolumeCom : public IComponent{
	public:
		BoundingVolumeCom() :bvType_(BV_NULL) {}
		BoundingVolumeType getBoundingVolumeType() const {
			return bvType_;
		}
		virtual AABB getAABB() const = 0;
		//根据位移分量ds更新aabb
		virtual void updateBoundingVolume(const vec3& ds) = 0;
		unsigned int proxyId = -1;
		BoundingVolumeType bvType_;
	};
}


#endif