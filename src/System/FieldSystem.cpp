#include "FieldSystem.h"
#include "Entity/GameObject.h"
#include "Entity/Level.h"
#include "Physics/State.h"

namespace Sun {

    FieldSystem::FieldSystem(const FieldCreateInfo& createInfo)
	{
		accel_ = createInfo.accel;
		level_ = createInfo.level;
	}

    void FieldSystem::prepare() {

    }

	void FieldSystem::tick(float dtime, const SEvent& ev) {
		static float time = 0;
		time += dtime;

		if (!level_) return;
		auto& objs = level_->getPhysicalObjects();
		
		GameObject* obj = nullptr;
		for (auto& it : objs) {
			obj = it.second;
			std::shared_ptr<PhysicalStateCom> state = obj->getComponent<PhysicalStateCom>();
			std::shared_ptr<TransformCom> trans = obj->getComponent<TransformCom>();
			Acceleration accel = getAcceleration(trans->getWorldTransform().getTranslation(),time);
			//根据加速度和初始速度，来更新物体的状态(后向欧拉迭代)
			state->velocity = state->velocity + accel * dtime;

			if (state->velocity.y > 0) {
				int xx = 0;
				int yy = xx;
			}

			//这里似乎只需更新速度，

			//vec3 dpos = state->velocity * dtime;
			//state->pos = state->pos + dpos;
			//更新transform
			/*std::shared_ptr<TransformCom> trans = obj->getTransformCom();
			trans->translateRelativeTransform(dpos);*/
		}
	}

	Acceleration FieldSystem::getAcceleration(const vec3& worldPos, float time) {
		return accel_;
	}
}