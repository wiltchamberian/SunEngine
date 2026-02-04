#ifndef __AABB_COM_H
#define __AABB_COM_H

#include "BVCom.h"

namespace Sun {

	class AABBCom :public BoundingVolumeCom {
	public:
		AABBCom() {
			bvType_ = BoundingVolumeType::BV_AABB;
		}
		AABB getAABB() const override;
		void updateBoundingVolume(const vec3& ds) override;
		AABB aabb_;
	};
}

#endif