#include "BVSphereCom.h"
#include "Math/Geometry.h"
#include "Physics/DynamicHintTest.h"
#include "AABBCom.h"

namespace Sun {

	AABB BVSphereCom::getAABB() const {
		return transformSphereToAABB(sphere_);
	}

	void BVSphereCom::updateBoundingVolume(const vec3& ds) {
		sphere_.center += ds;
	}

	void BVSphereCom::collide(const vec3& ds, const vec3& dsTarget, const AABBCom& aabb, HitRecord& hitRecord) {
		vec3 nv = ds * hitRecord.frac;
		HitRecord rec = dhitSphereAABB(sphere_, ds-dsTarget, aabb.aabb_);
		if (rec.frac < hitRecord.frac) {
			hitRecord = rec;
		}
	}
}
