#include "CollisionDectSystem.h"
#include "Entity/Level.h"
#include "Entity/World.h"
#include "Component/BVSphereCom.h"
#include "Component/AABBCom.h"

namespace Sun {

	CollisionDectSystem::CollisionDectSystem(SWorld* world)
	: world_(world)
	{

	}

    void CollisionDectSystem::prepare() {

    }

    void CollisionDectSystem::tick(float dtime, const SEvent& ev) {
        if (world_) {
            SLevel* level = world_->getCurLevel();
            if (level) {
                //{
                //	auto& objs = level->getCollideQueryObjects();
                //	GameObject* obj = nullptr;
                //	//更新状态
                //	HitRecord rec;
                //	rec.isHit = false;
                //	for (auto& it : objs) {	
                //		obj = it.second;
                //		obj->hitResponse(rec, nullptr);
                //	}

                //	return;
                //}




                auto& objs = level->getCollideQueryObjects();
                GameObject* obj = nullptr;
                AABB dAabb;

                int k = 0;
                for (auto& it : objs) {
                    obj = it.second;
                    CollideCallBack callback;
                    HitRecord record;
                    callback.record_ = &record;
                    callback.dtime_ = dtime;
                    AABB aabb = obj->getAABB();
                    dAabb = obj->getExpectDynamicAABB();
                    vec3 lastV = obj->getPosition();
                    bvhTree_.query(callback, obj, dAabb);
                    if (obj->rigidTransform_.translate.y > 0) {
                        int x = 0;
                        int yy = x;
                    }
                    printf("rigidTranslate:%f\n", obj->rigidTransform_.translate.y);
                    printf("times_:%i:y_value:%f\n", k, lastV.y);
                    //更新状态
                    obj->hitResponse(record, nullptr);

                    obj->lastAABB = aabb;
                    obj->extendAABB = dAabb;
                    obj->lastV = lastV;
                    k += 1;
                }
            }
        }


    }

	void CollisionDectSystem::addGameObject(GameObject* obj) {
		obj->proxyId_ = bvhTree_.createProxy(obj->getAABB(), obj);
	}

	bool CollideCallBack::operator() (GameObject* cur, GameObject* target) {
		if (cur == nullptr || target == nullptr) return false;
		auto bvCur = cur->getComponent<BoundingVolumeCom>();
		auto bvTarget = target->getComponent<BoundingVolumeCom>();
		if (bvCur == nullptr || bvTarget == nullptr) return false;
		if (bvCur->getBoundingVolumeType() == BV_SPHERE && bvTarget->getBoundingVolumeType() == BV_AABB) {
			SharedPtr<BVSphereCom> sphere = std::dynamic_pointer_cast<BVSphereCom>(bvCur);
			SharedPtr<AABBCom> aabb = std::dynamic_pointer_cast<AABBCom>(bvTarget);

			SharedPtr<PhysicalStateCom> state = cur->getComponent<PhysicalStateCom>();
			SharedPtr<PhysicalStateCom> targetState = target->getComponent<PhysicalStateCom>();
			//sphere->collide(state->velocity*dtime_, targetState->velocity*dtime_,*aabb ,record_);
			sphere->collide(cur->rigidTransform_.translate, target->rigidTransform_.translate, *aabb, *record_);
		}
	}

#if 0
	void CollideCallBack::hitResponse(GameObject* cur, GameObject* target, HitRecord& record) {
		if (cur) {
			cur->hitResponse(record, target);
		}
	}
#endif

}