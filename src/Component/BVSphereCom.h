#ifndef __BV_SPHERE_H
#define __BV_SPHERE_H

#include "BVCom.h"
#include "Math/Sphere.h"
#include "Physics/HitRecord.h"

namespace Sun {

	class AABBCom;

	class BVSphereCom : public BoundingVolumeCom
	{
	public:
		BVSphereCom(){
			bvType_ = BoundingVolumeType::BV_SPHERE;
		}
		BVSphereCom(const Sphere& sphere) :sphere_(sphere) {
			bvType_ = BoundingVolumeType::BV_SPHERE;
		}
		AABB getAABB() const override;
		void updateBoundingVolume(const vec3& ds) override;
		void collide(const vec3& v, const vec3& vTarget, const AABBCom& aabb, HitRecord& hitRecord);

		Sphere sphere_;
	};


}



#endif