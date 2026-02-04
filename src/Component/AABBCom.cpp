#include "AABBCom.h"

namespace Sun {

	AABB AABBCom::getAABB() const {
		return aabb_;
	}

	void AABBCom::updateBoundingVolume(const vec3& ds) {
		aabb_ = aabb_.translateAABB(ds);
	}
}