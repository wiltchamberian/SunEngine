#ifndef __COLLISION_DECT_SYSTEM_H
#define __COLLISION_DECT_SYSTEM_H

#include "System/SystemBase.h"
#include "Physics/BVH.h"
#include "Physics/DynamicHintTest.h"
#include "Physics/HitRecord.h"
#include "Entity/GameObject.h"

namespace Sun {

	class SWorld;

	class CollideCallBack {
	public:
		bool operator() (GameObject* cur, GameObject* target);
		HitRecord* record_ = nullptr;
		float dtime_;
	};

	class CollisionDectSystem :public SystemBase {
	public:
		CollisionDectSystem(SWorld* world = nullptr);
        void prepare() override;
        void tick(float dtime, const SEvent& ev) override;

		void addGameObject(GameObject* obj);
	protected:
		SWorld* world_;
		BVHTree<GameObject*> bvhTree_;
	};
}

#endif