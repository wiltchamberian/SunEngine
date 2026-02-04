#include "Character.h"
#include "Missile.h"
#include "buildIn/buildin.h"
#include "Entity/Level.h"
#include "Entity/World.h"

namespace Sun {

	GameObject* Character::spawn() {
		if (world_) {
			SLevel* level = world_->getCurLevel();
			if (level) {
				MissileCreateInfo createInfo;
				createInfo.sphere.center = this->getPosition();
				createInfo.sphere.r = 0.1;

				createInfo.state.invMass = 10;//1/(0.1kg)
				createInfo.state.pos = createInfo.sphere.center;
				createInfo.state.speed = 5.0; //5m/s

				float yaw = getYaw()+A_PI;
				float pitch = - getPitch();
				createInfo.state.velocity = vec3::fromEulerianAngles(yaw,pitch)*createInfo.state.speed;
				
				Missile* missile = createMissile(createInfo);
				level->addActor(missile);
			}		
		}

		return nullptr;
	}

	bool Character::supportCollideQuery() const {
		return true;
	}

	bool Character::needAddToBVH() const {
		return false;
	}
}